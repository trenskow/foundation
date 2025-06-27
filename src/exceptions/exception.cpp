//
// exception.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include <signal.h>

#include "./exception.hpp"

using namespace foundation::exceptions;

NotImplementedException::NotImplementedException()  {
#ifdef DEBUG
	raise(SIGTRAP);
#endif
}

const char* NotImplementedException::description() const {
	return "Not implemented.";
}

NotImplementedException* NotImplementedException::clone() const {
	return new NotImplementedException();
}

const char* io::fs::FileNotFoundException::description() const {
	return "File does not exist.";
}

io::fs::FileNotFoundException* io::fs::FileNotFoundException::clone() const {
	return new FileNotFoundException();
}

const char* io::fs::CannotOpenFileException::description() const {
	return "Could not open file.";
}

io::fs::CannotOpenFileException* io::fs::CannotOpenFileException::clone() const {
	return new CannotOpenFileException();
}

io::fs::PositionIsOutsideFileRange::PositionIsOutsideFileRange(
	ssize_t position
) : _position(position) { }

ssize_t io::fs::PositionIsOutsideFileRange::position() const {
	return this->_position;
}

const char* io::fs::PositionIsOutsideFileRange::description() const {
	return "Position is outside file range.";
}

io::fs::PositionIsOutsideFileRange* io::fs::PositionIsOutsideFileRange::clone() const {
	return new PositionIsOutsideFileRange(this->_position);
}

const char* io::fs::FileModeException::description() const {
	return "Operation not allowed in file's mode.";
}

io::fs::FileModeException* io::fs::FileModeException::clone() const {
	return new FileModeException();
}

serialization::JSONMalformedException::JSONMalformedException(
	size_t line,
	size_t character
) : _line(line),
	_character(character) { }

const char* serialization::JSONMalformedException::description() const {
	return "JSON is malformed.";
}

serialization::JSONMalformedException* serialization::JSONMalformedException::clone() const {
	return new JSONMalformedException(this->_line, this->_character);
}

size_t serialization::JSONMalformedException::line() const {
	return this->_line;
}

size_t serialization::JSONMalformedException::character() const {
	return this->_character;
}

const char* serialization::JSONEncodingCircularReferenceException::description() const {
	return "Circular reference detected.";
}

serialization::JSONEncodingCircularReferenceException* serialization::JSONEncodingCircularReferenceException::clone() const {
	return new JSONEncodingCircularReferenceException();
}

memory::AllocationException::AllocationException(
	const size_t size
) : _size(size) { }

memory::AllocationException::AllocationException(
	const AllocationException& other
) : _size(other._size) { }

const char* memory::AllocationException::description() const {
	return "Could not allocate memory.";
}

memory::AllocationException* memory::AllocationException::clone() const {
	return new AllocationException(this->_size);
}

size_t memory::AllocationException::size() const {
	return this->_size;
}

types::TypeConversionException::TypeConversionException() { }

const char* types::TypeConversionException::description() const {
	return "Could not convert type.";
}

types::TypeConversionException* types::TypeConversionException::clone() const {
	return new TypeConversionException();
}

types::RangeParserException::RangeParserException() { }

const char* types::RangeParserException::description() const {
	return "Could not parse range.";
}

types::RangeParserException* types::RangeParserException::clone() const {
	return new RangeParserException();
}

types::ISO8601Exception::ISO8601Exception() { }

const char* types::ISO8601Exception::description() const {
	return "Could not parse ISO8601 date.";
}

types::ISO8601Exception* types::ISO8601Exception::clone() const {
	return new ISO8601Exception();
}

types::DecoderException::DecoderException(
	const size_t characterIndex
) : _characterIndex(characterIndex) { }

types::DecoderException::DecoderException(
	const DecoderException& other
) : _characterIndex(other._characterIndex) { }

const char* types::DecoderException::description() const {
	return "Could not decode character.";
}

types::DecoderException* types::DecoderException::clone() const {
	return new DecoderException(this->_characterIndex);
}

size_t types::DecoderException::characterIndex() const {
	return this->_characterIndex;
}

types::EncoderException::EncoderException(
	const size_t characterIndex
) : _characterIndex(characterIndex) { }

const char* types::EncoderException::description() const {
	return "Could not encode character.";
}

types::EncoderException* types::EncoderException::clone() const {
	return new EncoderException(this->_characterIndex);
}

size_t types::EncoderException::characterIndex() const {
	return this->_characterIndex;
}

types::EncoderTypeException::EncoderTypeException() { }

const char* types::EncoderTypeException::description() const {
	return "Could not encode type.";
}

types::EncoderTypeException* types::EncoderTypeException::clone() const {
	return new EncoderTypeException();
}

types::OutOfBoundException::OutOfBoundException(
	const size_t index
) : _index(index) { }

types::OutOfBoundException::OutOfBoundException(
	const OutOfBoundException& other
) : _index(other._index) { }

const char* types::OutOfBoundException::description() const {
	return "Index is out of bounds.";
}

types::OutOfBoundException* types::OutOfBoundException::clone() const {
	return new OutOfBoundException(this->_index);
}

size_t types::OutOfBoundException::index() const {
	return this->_index;
}

types::KeyNotFoundException::KeyNotFoundException() { }

const char* types::KeyNotFoundException::description() const {
	return "Key not found.";
}

types::KeyNotFoundException* types::KeyNotFoundException::clone() const {
	return new KeyNotFoundException();
}

types::NotFoundException::NotFoundException() { }

const char* types::NotFoundException::description() const {
	return "Not found.";
}

types::NotFoundException* types::NotFoundException::clone() const {
	return new NotFoundException();
}

types::DurationParserException::DurationParserException() { }

const char* types::DurationParserException::description() const {
	return "Could not parse duration.";
}

types::DurationParserException* types::DurationParserException::clone() const {
	return new DurationParserException();
}

types::UUIDMalformedException::UUIDMalformedException() { }

const char* types::UUIDMalformedException::description() const {
	return "UUID is malformed.";
}

types::UUIDMalformedException* types::UUIDMalformedException::clone() const {
	return new UUIDMalformedException();
}

types::URLMalformedException::URLMalformedException() { }

const char* types::URLMalformedException::description() const {
	return "URL is malformed.";
}

types::URLMalformedException* types::URLMalformedException::clone() const {
	return new URLMalformedException();
}

types::UrlDecodingException::UrlDecodingException(
	uint8_t chr
) : _character(chr) {}

types::UrlDecodingException::UrlDecodingException(
	const UrlDecodingException& other
) : _character(other._character) {}

const char* types::UrlDecodingException::description() const {
	return "Could not decode URL character.";
}

types::UrlDecodingException* types::UrlDecodingException::clone() const {
	return new UrlDecodingException(this->_character);
}

uint8_t types::UrlDecodingException::character() const {
	return this->_character;
}

tools::EmptyException::EmptyException() { }

const char* tools::EmptyException::description() const {
	return "Is empty.";
}

tools::EmptyException* tools::EmptyException::clone() const {
	return new EmptyException();
}

tools::InvalidPatternException::InvalidPatternException() { }

const char* tools::InvalidPatternException::description() const {
	return "Pattern is invalid.";
}

tools::InvalidPatternException* tools::InvalidPatternException::clone() const {
	return new InvalidPatternException();
}
