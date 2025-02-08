//
// json.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "../exceptions/exception.hpp"
#include "../types/array.hpp"
#include "../types/number.hpp"
#include "../types/null.hpp"
#include "../types/string.hpp"
#include "../types/dictionary.hpp"
#include "../types/duration.hpp"
#include "../types/date.hpp"
#include "../types/uuid.hpp"
#include "../system/endian.hpp"

#include "./json.hpp"

using namespace foundation::serialization;
using namespace foundation::exceptions::serialization;

bool JSON::_hasLength(
	const String& string,
	size_t* idx,
	size_t length
) {
	return *idx + length <= string.length();
}

void JSON::_ensureLength(
	const String& string,
	size_t* idx,
	size_t length,
	size_t line,
	size_t character
) {
	if (!this->_hasLength(string, idx, length)) {
		throw JSONMalformedException(line, character);
	}
}

void JSON::_ensureData(
	const String& string,
	size_t* idx,
	size_t line,
	size_t character
) {
	return this->_ensureLength(string, idx, 1, line, character);
}

void JSON::_offsetWhiteSpaces(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character,
	bool ensure
) {
	// First we ignore any whitespaces.
	while (string.length() > *idx && (string[*idx] == 0x20 || string[*idx] == 0x09 || string[*idx] == 0x0A || string[*idx] == 0x0D)) {
		if (string[*idx] == 0x0A) {
			(*line)++;
			(*character) = 0;
		} else {
			(*character)++;
		}
		(*idx)++;
	}

	if (ensure) {
		this->_ensureData(string, idx, *line, *character);
	}

}

Strong<Type> JSON::_parseDictionary(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character,
	size_t level
) {

	if (string[*idx] != '{') throw JSONMalformedException(*line, *character);

	(*idx)++;
	(*character)++;

	this->_ensureData(string, idx, *line, *character);

	Strong<Dictionary<String, Type>> result;

	while (true) {
		this->_offsetWhiteSpaces(string, idx, line, character);
		if (string[*idx] == '}') break;
		if (string[*idx] == ',') {
			if (result->count() == 0) throw JSONMalformedException(*line, *character);
			(*idx)++;
			(*character)++;
			this->_offsetWhiteSpaces(string, idx, line, character);
		}
		Strong<String> key = this->_parseString(string, idx, line, character).as<String>();
		this->_offsetWhiteSpaces(string, idx, line, character);
		if (string[*idx] != ':') throw JSONMalformedException(*line, *character);
		(*idx)++;
		(*character)++;
		this->_offsetWhiteSpaces(string, idx, line, character);
		Strong<Type> value = this->_parse(string, idx, line, character, level + 1);
		result->set(key, value);
		this->_offsetWhiteSpaces(string, idx, line, character);
		if (string[*idx] != ',' && string[*idx] != '}') throw JSONMalformedException(*line, *character);
	}

	(*idx)++;
	(*character)++;

	return result.as<Type>();

}

Strong<Type> JSON::_parseArray(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character,
	size_t level
) {

	if (string[*idx] != '[') throw JSONMalformedException(*line, *character);

	(*idx)++;
	(*character)++;

	this->_ensureData(string, idx, *line, *character);

	Strong<Array<Type>> result;

	while (true) {
		this->_offsetWhiteSpaces(string, idx, line, character);
		if (string[*idx] == ']') break;
		if (string[*idx] == ',') {
			if (result->count() == 0) throw JSONMalformedException(*line, *character);
			(*idx)++;
			(*character)++;
			this->_offsetWhiteSpaces(string, idx, line, character);
		} else if (result->count() > 0) throw JSONMalformedException(*line, *character);
		result->append(_parse(string, idx, line, character, level + 1));
		this->_offsetWhiteSpaces(string, idx, line, character);
		if (string[*idx] != ',' && string[*idx] != ']') throw JSONMalformedException(*line, *character);
	}

	(*idx)++;
	(*character)++;

	return result.as<Type>();

}

Strong<Type> JSON::_parseNumber(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character
) {

	size_t consumed = 0;

	double value;
	try {
		value = string.doubleValue(*idx, &consumed, false);
		(*idx) += consumed;
		(*character) += consumed;
	} catch (const DecoderException& exception) {
		throw JSONMalformedException(*line, *character + exception.characterIndex());
	}

	if (round(value) == value) {
		return Strong<Integer>((int64_t)value)
			.as<Type>();
	}

	return Strong<Float>(value)
		.as<Type>();

}

Strong<Type> JSON::_parseString(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character
) {

	if (string[*idx] != '"') throw JSONMalformedException(*line, *character);

	(*idx)++;
	(*character)++;

	Data<uint16_t> stringBytes;

	do {

		if (string.length() == *idx) throw JSONMalformedException(*line, *character);
		if (string[*idx] == '"') break;

		switch (string[*idx]) {
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
				throw JSONMalformedException(*line, *character);
			case '\\':
				(*idx)++;
				(*character)++;
				this->_ensureData(string, idx, *line, *character);
				switch (string[*idx]) {
					case 'b':
						stringBytes.append('\b');
						break;
					case 'f':
						stringBytes.append('\f');
						break;
					case 'n':
						stringBytes.append('\n');
						break;
					case 'r':
						stringBytes.append('\r');
						break;
					case 't':
						stringBytes.append('\t');
						break;
					case '"':
						stringBytes.append('"');
						break;
					case '\\':
						stringBytes.append('\\');
						break;
					case '/':
						stringBytes.append('/');
						break;
					case 'u': {
						this->_ensureLength(string, idx, 5, *line, *character);
						String code = string.substring(*idx + 1, 4);
						try {
							stringBytes.append(Endian::toSystemVariant(code.hexData()->as<uint16_t>()->itemAtIndex(0), Endian::Variant::big));
						} catch (DecoderException&) {
							throw JSONMalformedException(*line, *character);
						} catch (OutOfBoundException&) {
							throw JSONMalformedException(*line, *character);
						}
						(*idx) += 4;
						(*character) += 4;
						break;
					}
					default:
						throw JSONMalformedException(*line, *character);
				}
				break;
			default:
				if (string[*idx] <= 0x0f) throw JSONMalformedException(*line, *character);
				stringBytes.append(string[*idx]);
		}

		(*idx)++;
		(*character)++;

	} while (true);

	(*idx)++;
	(*character)++;

	return Strong<String>(stringBytes, Endian::systemVariant()).as<Type>();

}

Strong<Type> JSON::_parseLiteral(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character
) {

	static const String trueLiteral = "true";
	static const String falseLiteral = "false";
	static const String nullLiteral = "null";

	switch (string[*idx]) {
		case 't':
			this->_ensureLength(string, idx, 4, *line, *character);
			if (string.substring(*idx, 4)->equals(trueLiteral)) {
				throw JSONMalformedException(*line, *character);
			}
			(*idx) += 4;
			(*character) += 4;
			return Strong<types::Boolean>(true).as<Type>();
		case 'f':
			this->_ensureLength(string, idx, 5, *line, *character);
			if (string.substring(*idx, 5)->equals(falseLiteral)) {
				throw JSONMalformedException(*line, *character);
			}
			(*idx) += 5;
			(*character) += 5;
			return Strong<types::Boolean>(false).as<Type>();
		case 'n':
			this->_ensureLength(string, idx, 4, *line, *character);
			if (string.substring(*idx, 4)->equals(nullLiteral)) {
				throw JSONMalformedException(*line, *character);
			}
			(*idx) += 4;
			(*character) += 4;
			return Strong<Null>().as<Type>();
		default:
			throw JSONMalformedException(*line, *character);
	}

}

Strong<Type> JSON::_parse(
	const String& string,
	size_t* idx,
	size_t* line,
	size_t* character,
	size_t level
) {

	// Maliciously deeply nested JSON could trigger stack overflow.
	if (level > 64) throw JSONMalformedException(*line, *character);

	this->_offsetWhiteSpaces(string, idx, line, character);

	uint32_t chr = string[*idx];

	switch (chr) {
		case '{':
			return this->_parseDictionary(string, idx, line, character, level);
		case '[':
			return this->_parseArray(string, idx, line, character, level);
		case '"':
			return this->_parseString(string, idx, line, character);
		case 'n':
		case 't':
		case 'f':
			return this->_parseLiteral(string, idx, line, character);
			break;
		default:
			if (chr == '-' || (chr >= '0' && chr <= '9')) {
				return this->_parseNumber(string, idx, line, character);
			}
			throw JSONMalformedException(*line, *character);
	}

}

JSON::JSON() { }

JSON::~JSON() { }

Strong<Type> JSON::parse(
	const String& string
) {

	size_t idx = 0;
	size_t line = 0;
	size_t character = 0;

	Strong<Type> result = nullptr;

	try {
		result = this->_parse(string, &idx, &line, &character, 0);
	} catch (const DecoderException& exception) {
		throw JSONMalformedException(line, character + exception.characterIndex());
	}

	this->_offsetWhiteSpaces(string, &idx, &line, &character, false);

	if (string.length() > idx) throw DecoderException(idx);

	return result;

}

bool JSON::isStringifyable(
	const Type& data
) {

	Data<void*> references;

	return _isStringifyable(
		data,
		references);

}

Strong<String> JSON::stringify(
	const Type& data
) {

	Data<void*> references;

	return this->_stringify(
		data,
		references);

}

bool JSON::_isStringifyable(
	const Type& data,
	Data<void*> references
) {

	if (references.contains((void*)&data)) {
		return false;
	}

	Data<void*> referencesNested = references
		.appending((void*)&data);

	switch (data.kind()) {
		case Type::Kind::dictionary: {

			const Dictionary<Type, Type>& dictionary = data.as<Dictionary<Type, Type>>();

			return dictionary.keys()->every([&](const Type& type) {
				return type.is(Type::Kind::string);
			}) && _isStringifyable(
				dictionary.values(),
				referencesNested);

		}
		case Type::Kind::array: {
			return data.as<Array<Type>>().every([&](const Type& data) {
				return _isStringifyable(
					data,
					referencesNested);
			});
		}
		case Type::Kind::string:
		case Type::Kind::number:
		case Type::Kind::null:
		case Type::Kind::date:
		case Type::Kind::uuid:
			return true;
		default:
			return false;
	}

}

Strong<String> JSON::_stringify(
	const Type& data,
	Data<void*>& references
) {

	if (references.contains((void*)&data)) {
		throw JSONEncodingCircularReferenceException();
	}

	Data<void*> referencesNested = references
		.appending((void*)&data);

	Strong<String> result;

	switch (data.kind()) {
		case Type::Kind::dictionary: {

			result->append("{");

			auto dictionary = data.as<Dictionary<Type, Type>>();

			result->append(String::join(dictionary.keys()->map<String>([&](const Type& key) {
				if (key.kind() != Type::Kind::string) throw EncoderTypeException();
				Strong<String> result;
				result->append(this->_stringify(key, referencesNested));
				result->append(":");
				result->append(this->_stringify(dictionary.get(key), referencesNested));
				return result;
			}), ","));

			result->append("}");

			break;
		}
		case Type::Kind::array: {

			Array<Type>& array = data.as<Array<Type>>();

			result->append("[");

			result->append(String::join(array.map<String>([&](const Type& item) {
				return this->_stringify(item, referencesNested);
			}), ","));

			result->append("]");

			break;
		}
		case Type::Kind::string: {
			auto bytes = data.as<String>().UTF16Data(Endian::systemVariant());
			result->append("\"");
			for (size_t idx = 0 ; idx < bytes->length() ; idx++) {
				auto byte = bytes->itemAtIndex(idx);
				switch (byte) {
					case '\b':
						result->append("\\b");
						break;
					case '\f':
						result->append("\\f");
						break;
					case '\n':
						result->append("\\n");
						break;
					case '\r':
						result->append("\\r");
						break;
					case '\t':
						result->append("\\t");
						break;
					case '\"':
						result->append("\\\"");
						break;
					case '\\':
						result->append("\\\\");
						break;
					default:
						if ((byte >= 0x20 && byte <= 0x7E)) {
							result->append(byte);
						} else {
							uint16_t beByte = Endian::fromSystemVariant(byte, Endian::Variant::big);
							auto beByteData = Data<uint16_t>(&beByte, 1).as<uint8_t>();
							result->append("\\u");
							result->append(String::fromHex(beByteData));
						}
						break;
				}
			}
			result->append("\"");
			break;
		}
		case Type::Kind::number: {
			switch (data.as<Numeric>().subType()) {
				case Numeric::Subtype::boolean:
					result->append(data.as<types::Boolean>().value() ? "true" : "false");
					break;
				case Numeric::Subtype::integer:
					result->append(String::format("%lld", data.as<Integer>().value()));
					break;
				case Numeric::Subtype::floatingPoint: {
					double value = data.as<Float>().value();
					uint64_t length = snprintf(nullptr, 0, "%f", value);
					char str[length + 1];
					snprintf(str, length + 1, "%f", value);
					result->append(str);
					break;
				}
			}
			break;
		}
		case Type::Kind::null:
			result->append("null");
			break;
		case Type::Kind::date:
			return this->_stringify(
				data.as<Date>()
					.to(Date::TimeZone::utc)
					.toISO8601(),
				referencesNested);
			break;
		case Type::Kind::uuid:
			return this->_stringify(
				data.as<UUID>()
					.string(),
					referencesNested);
		default:
			throw EncoderTypeException();
	}

	return result;

}