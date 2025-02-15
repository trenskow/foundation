//
// json.hpp
// foundation
//
// Created by Kristian Trenskow on 2020/04/01.
// See license in LICENSE.
//

#ifndef foundation_json_hpp
#define foundation_json_hpp

#include "../memory/object.hpp"
#include "../types/string.hpp"

using namespace foundation::memory;
using namespace foundation::types;

namespace foundation::serialization {

	class JSON: public Object {

	private:

		bool _hasLength(
			const String& string,
			size_t* idx,
			size_t length);

		void _ensureLength(
			const String& string,
			size_t* idx,
			size_t length,
			size_t line,
			size_t character);

		void _ensureData(
			const String& string,
			size_t* idx,
			size_t line,
			size_t character);

		void _offsetWhiteSpaces(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character,
			bool ensure = true);

		Strong<Type> _parseDictionary(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character,
			size_t level);

		Strong<Type> _parseArray(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character,
			size_t level);

		Strong<Type> _parseNumber(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character);

		Strong<Type> _parseString(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character);

		Strong<Type> _parseLiteral(
			const String&
			string,
			size_t* idx,
			size_t* line,
			size_t* character);

		Strong<Type> _parse(
			const String& string,
			size_t* idx,
			size_t* line,
			size_t* character,
			size_t level);

	public:

		JSON();
		virtual ~JSON();

		Strong<Type> parse(
			const String& string);

		static bool isStringifyable(
			Strong<Type> data);

		Strong<String> stringify(
			Strong<Type> data);

	private:

		static bool _isStringifyable(
			Strong<Type> data,
			Data<void*> references);

		Strong<String> _stringify(
			Strong<Type> data,
			Data<void*>& references);

	};

}

#endif /* foundation_json_hpp */
