//
// string.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "string.hpp"

using namespace foundation::types;

Type::Kind String::typeKind() {
	return Type::Kind::string;
}

String String::fromCString(
	function<size_t(char*,size_t)> todo,
	const size_t size
){
	return String(Data<uint8_t>::fromCBuffer([&todo](void* buffer, size_t length) {
		return todo((char*)buffer, length);
	}, size));
}

String::String(
) : String(Data<uint32_t>()) { }

String::String(
	const char* string
) noexcept(false) : String() {
	if (string != nullptr) {
		_storage.append(_decodeUTF8((const uint8_t*)string, strlen(string)));
	}
}

String::String(
	const Data<uint8_t>& data
) : String() {
	_storage.append(_decodeUTF8(data.items(), _lengthWithoutNullTerminator<uint8_t>(data)));
}

String::String(
	const Data<uint16_t>& data,
	const Endian::Variant& endian
) noexcept(false)  : String() {
	_storage.append(_decodeUTF16(data.items(), _lengthWithoutNullTerminator<uint16_t>(data), endian));
}

String::String(
	const Data<uint16_t>& data
) : String() {

	Strong<Data<uint16_t>> parseData(data);

	Endian::Variant endian = Endian::Variant::big;

	if (data.length() > 1) {
		Strong<Data<uint8_t>> potentialMarker = data.subdata(0, 2)->as<uint8_t>();
		if (potentialMarker->equals(bigEndianBOM) || potentialMarker->equals(littleEndianBOM)) {
			endian = potentialMarker->equals(bigEndianBOM) ? Endian::Variant::big : Endian::Variant::little;
			parseData = parseData->subdata(2);
		}
	}

	_storage.append(_decodeUTF16(parseData->items(), parseData->length(), endian));

}

String String::format(
	const char* format,
	...
)  {

	va_list args;

	va_start(args, format);

	size_t size = vsnprintf(nullptr, 0, format, args) + 1;

	va_end(args);

	va_start(args, format);

	char buffer[size];

	vsnprintf(buffer, size, format, args);

	va_end(args);

	return String((const char*)buffer);

}

Strong<String> String::fromHex(
	const Data<uint8_t>& data
) {
	return Strong<String>(_decodeHex(data));
}

Strong<String> String::join(
	const Array<String>& strings
) {
	return Strong<String>(
		Data<uint32_t>::join(
			strings
				.map<Data<uint32_t>>(
					[](const String& current) {
						return current._storage;
					})));
}

Strong<String> String::join(
	const Array<String>& strings,
	const String& separator
) {
	return Strong<String>(
		Data<uint32_t>::join(
			strings.map<Data<uint32_t>>(
				[](const String& current) {
					return current._storage;
				}), separator._storage));
}

String::String(
	const Data<uint32_t>& store
) : _storage(store) { }

String::String(
	const uint32_t character
) : _storage(&character, 1) { }

String::String(
	const String& other
) : String(other._storage) { }

String::String(
	String&& other
) : _storage(std::move(other._storage)) { }

String::~String() { }

size_t String::length() const {
	return _storage.length();
}

void String::withCString(
	const function<void(const char*)>& todo
) const {
	todo((const char*)this->UTF8Data(true)->items());
}

void String::print(
	bool newLine
) const {
	this->appending(newLine ? "\n" : "")->withCString(printf);
}

Strong<Data<uint8_t>> String::UTF8Data(
	bool nullTerminate
) const {
	return _encodeUTF8(_storage, nullTerminate);
}

Strong<Data<uint16_t>> String::UTF16Data(
	Endian::Variant endian,
	bool includeBOM
) const {
	return _encodeUTF16(_storage, endian, includeBOM);
}

Strong<Data<uint32_t>> String::UTF32Data() const {
	return Strong<Data<uint32_t>>(this->_storage);
}

Strong<Data<uint8_t>> String::hexData() const {
	return _encodeHex(_storage);
}

void String::append(
	const String& other
)  {
	_storage.append(other._storage);
}

void String::append(
	uint32_t character
) {
	_storage.append(character);
}

Strong<String> String::appending(
	const String& other
) const {
	return String(this->_storage.appending(other._storage));
}

Strong<Array<String>> String::split() const {
	return _storage.split()->map<String>([](Data<uint32_t>& data) {
		return Strong<String>(data);
	});
}

Strong<Array<String>> String::split(
	const String& separator,
	IncludeSeparator includeSeparator,
	size_t max
) const {
	return _storage.split(separator._storage, includeSeparator, max)->map<String>([](const Data<uint32_t>& current) {
		return Strong<String>(current);
	});
}

Strong<Array<String>> String::split(
	const Array<String>& separators,
	IncludeSeparator includeSeparator,
	size_t max
) const {

	auto stores = separators.map<Data<uint32_t>>(
		[](const String& current) {
			return current._storage;
		});

	return _storage.split(
		stores,
		includeSeparator,
		max)
		->map<String>(
			[](const Data<uint32_t>& current) {
				return Strong<String>(current);
			});

}

Strong<String> String::replacing(
	const String& needle,
	const String& replacement
) const {
	return String::join(this->split(needle), replacement);
}

double String::doubleValue(
	size_t startIndex,
	size_t* consumed,
	bool allowLeadingZero
) const noexcept(false) {

	double integerMultiplier = 1;
	bool integerMultiplierParsed = false;
	bool integerHadLeadingZero = false;
	bool integerHadDigits = false;
	size_t integerStartIndex = NotFound;
	double fractionMultiplier = 10;
	bool fractionHadDigits = false;
	size_t fractionStartIndex = NotFound;
	double exponentMultiplier = 1;
	bool exponentMultiplierParsed = false;
	bool exponentHadDigits = false;
	size_t exponentStartIndex = NotFound;

	double integer = 0;
	double fraction = 0;
	double exponent = 0;

	size_t idx = startIndex;
	while (idx < length()) {

		uint32_t chr = _storage[idx];

		if (chr == '+' || chr == '-') {

			if (fractionStartIndex != NotFound && exponentStartIndex == NotFound) throw DecoderException(idx);
			if (exponentStartIndex == NotFound && integerMultiplierParsed) throw DecoderException(idx);
			if (exponentStartIndex != NotFound && exponentMultiplierParsed) throw DecoderException(idx);

			if (exponentStartIndex != NotFound) {
				if (exponentHadDigits) throw DecoderException(idx);
				exponentMultiplier = chr == '+' ? 1 : -1;
				exponentMultiplierParsed = true;
			}
			else {
				if (integerHadDigits) throw DecoderException(idx);
				integerMultiplier = chr == '+' ? 1 : -1;
				integerMultiplierParsed = true;
			}

		}

		else if (chr == '.') {
			if (!integerHadDigits) throw DecoderException(idx);
			if (exponentStartIndex != NotFound || fractionStartIndex != NotFound) throw DecoderException(idx);
			fractionStartIndex = idx;
		}

		else if (chr == 'e' || chr == 'E') {
			if (exponentStartIndex != NotFound) throw DecoderException(idx);
			exponentStartIndex = idx;
		}

		else if (chr >= '0' && chr <= '9') {

			if (exponentStartIndex != NotFound && !exponentHadDigits) exponentHadDigits = true;
			else if (fractionStartIndex != NotFound && !fractionHadDigits) fractionHadDigits = true;
			else if (!integerHadDigits) {
				integerHadDigits = true;
				integerHadLeadingZero = chr == '0';
			}

			double value = chr - '0';

			if (exponentStartIndex != NotFound) {
				exponent = exponent * 10 + value;
			} else if (fractionStartIndex != NotFound) {
				fraction = fraction + (value / fractionMultiplier);
				fractionMultiplier *= 10;
			} else {
				integer = integer * 10 + value;
			}

		} else {
			break;
		}

		idx++;

	}

	if (consumed != nullptr) *consumed = idx - startIndex;

	if (exponentStartIndex != NotFound && !exponentHadDigits) throw DecoderException(exponentStartIndex);
	if (fractionStartIndex != NotFound && !fractionHadDigits) throw DecoderException(fractionStartIndex);
	if (!integerHadDigits) throw DecoderException(startIndex);

	double result = ((integer * integerMultiplier) + fraction) * (pow(10, exponent) * exponentMultiplier);

	if (!allowLeadingZero && integer != 0 && integerHadLeadingZero) throw DecoderException(integerStartIndex);

	return result;

}

size_t String::indexOf(
	const String& other,
	size_t offset
) const {
	return this->_storage.indexOf(other._storage, offset);
}

size_t String::indexOf(
	uint32_t chr
) const {
	return this->_storage.indexOf(chr);
}

size_t String::lastIndexOf(
	const String& other
) const {
	return this->_storage.lastIndexOf(other._storage);
}

bool String::contains(
	const String& other
) const {
	return this->indexOf(other) != NotFound;
}

bool String::hasSuffix(
	const String& other
) const {
	return this->_storage.hasPrefix(other._storage);
}

bool String::hasPrefix(
	const String& other
) const {
	return this->_storage.hasSuffix(other._storage);
}

Strong<String> String::substring(
	size_t offset,
	size_t length
) const {
	return Strong<String>(_storage.subdata(offset, length));
}

Strong<String> String::slicing(
	ssize_t start,
	ssize_t end
) const {
	return Strong<String>(_storage.slicing(start, end));
}

Strong<String> String::uppercased() const {
	return Strong<String>(this->_storage.map<uint32_t>(Unicode::lowerToUpper));
}

Strong<String> String::lowercased() const {
	return String(this->_storage.map<uint32_t>(Unicode::upperToLower));
}

Strong<String> String::capitalized() const {
	if (this->length() == 0) return Strong<String>();
	if (this->length() == 1) return this->uppercased();
	return this->substring(0, 1)->uppercased()->appending(this->substring(1)->lowercased());
}

Strong<String> String::trimmedStart() const {

	ssize_t start = 0;

	while (start < (ssize_t)this->_storage.length() && Unicode::isWhitespace(this->_storage[start])) {
		start++;
	}

	return this->substring(start);

}

Strong<String> String::trimmedEnd() const {

	ssize_t end = this->_storage.length() - 1;

	while (end > 0 && Unicode::isWhitespace(this->_storage[end])) {
		end--;
	}

	return this->substring(0, end + 1);

}

Strong<String> String::trimmed() const {
	return this->trimmedStart()->trimmedEnd();
}

uint64_t String::hash() const {
	return _storage.hash();
}

Type::Kind String::kind() const {
	return Kind::string;
}

Strong<Type> String::clone() const {
	return Strong<String>(*this)
		.as<Type>();
}

bool String::equals(
	const Type& other
) const {
	if (!other.is(Type::Kind::string)) return false;
	return _storage.equals(((const String&)other)._storage);
}

bool String::equals(
	const char* other
) const {
	return this->equals(String(other));
}

bool String::greaterThan(
	const String& other
) const {
	return this->lowercased()->_storage.greaterThan(other.lowercased()->_storage);
}

uint32_t String::operator[](
	size_t idx
) const {
	return _storage[idx];
}

String& String::operator=(
	const String& other
) {
	Type::operator=(other);
	_storage = other._storage;
	return *this;
}

Data<uint32_t> String::_decodeUTF8(
	const uint8_t* buffer,
	size_t length
) noexcept(false) {

	size_t offset = 0;

	if (length > 2 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF) {
		offset += 3;
	}

	Data<uint32_t> ret;

	for (size_t idx = offset ; idx < length ; idx++) {

		const uint8_t firstByte = buffer[0];
		uint32_t codePoint = 0;
		uint8_t charWidth = 1;

		buffer++;

		if ((firstByte & 0x80) == 0) {
			codePoint = firstByte & 0x7F;
		} else if ((firstByte & 0xE0) == 0xC0) {
			charWidth = 2;
			codePoint = firstByte & 0x1F;
		} else if ((firstByte & 0xF0) == 0xE0) {
			charWidth = 3;
			codePoint = firstByte & 0x0F;
		} else if ((firstByte & 0xF8) == 0xF0) {
			charWidth = 4;
			codePoint = firstByte & 0x07;
		} else { // Malformed.
			throw DecoderException(idx);
		}

		// Malformed.
		if (length < charWidth) {
			throw DecoderException(idx);
		}

		for (int64_t i = 1 ; i < charWidth ; i++) {
			if ((buffer[0] & 0xC0) == 0x80) {
				codePoint = (codePoint << 6) | (buffer[0] & 0x3F);
			} else {
				// Malformed.
				throw DecoderException(idx);
			}
			buffer++;
			idx++;
		}

		ret.append(codePoint);

	}

	return ret;

}

Data<uint8_t> String::_encodeUTF8(
	const Data<uint32_t>& buffer,
	bool nullTerminate
) noexcept(false) {

	Data<uint8_t> ret;

	for (size_t idx = 0 ; idx < buffer.length() ; idx++) {

		uint8_t chr[4];

		uint32_t codePoint = buffer[idx];
		if (codePoint < 0x80) {
			ret.append(codePoint);
		} else if (codePoint < 0x800) {
			uint8_t chr[2];
			chr[0] = 0xC0 | (codePoint >> 6);
			chr[1] = 0x80 | (codePoint & 0x3F);
			ret.append(chr, 2);
		} else if (codePoint <= 0xFFFF) {
			chr[0] = 0xE0 | (codePoint >> 12);
			chr[1] = 0x80 | ((codePoint >> 6) & 0x3F);
			chr[2] = 0x80 | (codePoint & 0x3F);
			ret.append(chr, 3);
		} else if (codePoint <= 0x1FFFFF) {
			chr[0] = 0xF0 | (codePoint >> 18);
			chr[1] = 0x80 | ((codePoint >> 12) & 0x3F);
			chr[2] = 0x80 | ((codePoint >> 6) & 0x3F);
			chr[3] = 0x80 | (codePoint & 0x3F);
			ret.append(chr, 4);
		}

	}

	if (nullTerminate) ret.append('\0');

	return ret;

}

Data<uint32_t> String::_decodeUTF16(
	const uint16_t* buffer,
	size_t length,
	Endian::Variant endian
) noexcept(false) {

	Data<uint32_t> ret;

	for (size_t idx = 0 ; idx < length ; idx++) {

		uint16_t high = Endian::toSystemVariant(buffer[idx], endian);

		if (!(high >= 0xD800 && high <= 0xD8FF) || idx + 1 == length) ret.append(high);
		else {

			uint16_t low = Endian::toSystemVariant(buffer[idx + 1], endian);

			if (low >= 0xDC00 && low <= 0xDFFF) {
				ret.append((((high & 0x3FF) << 10) | (low & 0x3FF)) + 0x10000);
				idx++;
			}

		}

	}

	return ret;

}

Data<uint16_t> String::_encodeUTF16(
	const Data<uint32_t>& buffer,
	Endian::Variant endian,
	bool includeBOM
) noexcept(false) {

	Data<uint16_t> ret;

	if (includeBOM) {
		ret.append((endian == Endian::Variant::big ? bigEndianBOM : littleEndianBOM).as<uint16_t>());
	}

	for (size_t idx = 0 ; idx < buffer.length() ; idx++) {

		uint32_t chr = buffer[idx];

		if (chr <= 0xFFFF) ret.append(Endian::fromSystemVariant((uint16_t)chr, endian));
		else {
			chr -= 0x10000;
			ret.append(Endian::fromSystemVariant(0xD800 + ((chr >> 10) & 0x3FF), endian));
			ret.append(Endian::fromSystemVariant(0xDC00 + (chr & 0x3FF), endian));
		}

	}

	return ret;

}

Data<uint32_t> String::_decodeUTF32(
	const Data<uint32_t>& buffer,
	Endian::Variant endian
) {
	return buffer.map<uint32_t>([&endian](uint32_t character) {
		return Endian::convert(character, endian, Endian::systemVariant());
	});
}

Data<uint32_t> String::_encodeUTF32(
	const Data<uint32_t>& buffer,
	Endian::Variant endian
) {
	return buffer.map<uint32_t>([&endian](uint32_t character) {
		return Endian::convert(character, Endian::systemVariant(), endian);
	});
}

uint8_t String::_valueFromHex(
	uint8_t chr,
	size_t idx
) noexcept(false) {
	if (chr >= 'a' && chr <= 'f') return chr - ('a' - 'A');
	if (chr >= 'A' && chr <= 'F') return (chr - 'A') + 10;
	else if (chr >= '0' && chr <= '9') return chr - '0';
	else throw DecoderException(idx);
}

uint8_t String::_valueToHex(
	uint8_t value,
	size_t idx
) noexcept(false) {
	if (value < 10) return '0' + value;
	else if (value < 16) return 'A' + (value - 10);
	else throw EncoderException(idx);
}

Data<uint32_t> String::_decodeHex(
	const Data<uint8_t>& buffer
) noexcept(false) {

	if (buffer.length() % 2 != 0) throw OutOfBoundException(buffer.length() + 1);

	Data<uint32_t> ret;

	for (size_t idx = 0 ; idx < buffer.length() ; idx++) {
		auto byte = buffer[idx];
		ret.append(_valueToHex((byte & 0xF0) >> 4, idx));
		ret.append(_valueToHex(byte & 0x0F, idx));
	}

	return ret;

}

Data<uint8_t> String::_encodeHex(
	const Data<uint32_t> &buffer
) noexcept(false) {

	if (buffer.length() % 2 != 0) throw OutOfBoundException(buffer.length() + 1);

	Data<uint8_t> ret;

	for (size_t idx = 0 ; idx < buffer.length() ; idx += 2) {
		ret.append(_valueFromHex(buffer[idx], idx) << 4 | _valueFromHex(buffer[idx + 1], idx + 1));
	}

	return ret;

}
