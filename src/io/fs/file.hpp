//
// file.hpp
// foundation
//
// Created by Kristian Trenskow on 2020/04/12.
// See license in LICENSE.
//

#ifndef file_hpp
#define file_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "../../memory/object.hpp"
#include "../../exceptions/exception.hpp"
#include "../../tools/math.hpp"
#include "../../types/types.hpp"

using namespace foundation::memory;
using namespace foundation::types;
using namespace foundation::exceptions::io::fs;
using namespace foundation::tools;

namespace foundation::io::fs {

	class File : public Object {

		friend class Strong<File>;

	public:

		enum class Mode {
			asRead,
			asWrite,
			asAppend
		};

		enum class Position {
			beginning,
			current,
			end
		};

		static Strong<File> open(const String& filename, Mode mode);
		static bool exists(const String& filename);
		static String resolve(const String& filename);
		static Array<String> directoryContent(const String& filename);

		virtual ~File();

		template<typename T = uint8_t>
		Data<T> read(size_t count) {
			if (this->_mode == Mode::asWrite) throw FileModeException();
			size_t toRead = math::min(sizeof(T) * count, this->_size - this->_position);
			auto read = Data<T>::fromCBuffer([this](void* buffer, size_t length){
				fread(buffer, sizeof(T), length / sizeof(T), this->_stream);
				return length;
			}, toRead);
			this->_position += toRead;
			return read;
		}

		template<typename T>
		const T read() {
			this->_position += sizeof(T);
			if (this->_position > this->_size) throw PositionIsOutsideFileRange(this->_position);
			T result;
			fread(&result, sizeof(T), 1, this->_stream);
			return result;
		}

		template<typename T = uint8_t>
		Data<T> readToEnd() {
			Data<T> buffer;
			while (this->_position < this->_size) {
				buffer.append(this->read(16384));
			}
			return buffer;
		}

		template<typename T = uint8_t>
		void write(const Data<T>& data) {
			if (this->_mode == Mode::asRead) throw FileModeException();
			this->_position += data.count() * sizeof(T);
			this->_size = math::max(this->_position, this->_size);
			fwrite(data.items(), sizeof(T), data.count(), this->_stream);
		}

	private:

		File(const String& filename, Mode mode);

		void _determineSize();

		Mode _mode;
		FILE* _stream;
		size_t _size;
		size_t _position;

	};

}

#endif /* file_hpp */
