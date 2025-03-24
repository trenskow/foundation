//
// file.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/03/24.
// See license in LICENSE.
//

#include "file.hpp"

using namespace foundation::io::fs;
using namespace foundation::exceptions::io::fs;

Strong<File> File::open(const String& filename, Mode mode) {
	return Strong<File>(filename, mode);
}

bool File::exists(const String& filename) {
	return filename.mapCString<bool>([](const char* filename){
		return access(filename, F_OK) == 0;
	});
}

String File::expand(const String& filename) {
	return filename.mapCString<String>([](const char* filename) {
		wordexp_t exp_result;
		wordexp(filename, &exp_result, 0);
		size_t filenameLength = strlen(exp_result.we_wordv[0]);
		auto ret = String::fromCString([&exp_result,&filenameLength](char* buffer, size_t) {
			memcpy(buffer, exp_result.we_wordv[0], filenameLength);
			return filenameLength;
		}, filenameLength);
		wordfree(&exp_result);
		return ret;
	});
}

String File::resolve(const String& filename) {
	return filename.mapCString<String>([](const char* filename) {
		return String::fromCString([filename](char* buffer, size_t length) {
			ssize_t result = readlink(filename, buffer, length);
			if (result < 0) strncpy(buffer, filename, length);
			return strlen(buffer);
		}, 4096);
	});
}

Array<String> File::directoryContent(const String& filename) {

	if (!File::exists(filename)) throw FileNotFoundException();

	DIR* dp;
	dirent* ep;

	dp = filename.mapCString<DIR*>([](const char* filename) {
		return opendir(filename);
	});

	if (dp == nullptr) throw FileNotFoundException();

	Array<String> result;

	while ((ep = readdir(dp))) {
		result.append(Strong<String>(ep->d_name));
	}

	closedir(dp);

	return result;

}

File::~File() {
	fclose(this->_stream);
}

File::File(const String& filename, Mode mode) {
	File::resolve(File::expand(filename)).withCString([this,&mode](const char* filename) {

		switch (mode) {
			case Mode::asRead:
				this->_stream = fopen(filename, "r");
				break;
			case Mode::asWrite:
				this->_stream = fopen(filename, "w");
				break;
			case Mode::asAppend:
				this->_stream = fopen(filename, "a");
				break;
		}

		if (!this->_stream) throw CannotOpenFileException();
		this->_mode = mode;
		this->_determineSize();

	});
}

void File::_determineSize() {
	this->_position = ftell(this->_stream);
	fseek(this->_stream, 0, SEEK_END);
	this->_size = ftell(this->_stream);
	fseek(this->_stream, this->_position, SEEK_SET);
}
