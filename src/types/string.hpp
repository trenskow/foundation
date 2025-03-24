//
// string.hpp
// foundation
//
// Created by Kristian Trenskow on 2018/08/17.
// See license in LICENSE.
//

#ifndef foundation_string_hpp
#define foundation_string_hpp

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "../system/endian.hpp"
#include "../memory/strong.hpp"
#include "../exceptions/exception.hpp"
#include "./data.hpp"
#include "./type.hpp"
#include "./array.hpp"
#include "./unicode.hpp"

using namespace foundation::system;
using namespace foundation::memory;
using namespace foundation::exceptions;

namespace foundation::types {

	static const uint8_t _bigEndianBOM[] = { 0xFE, 0xFF };
	static const uint8_t _littleEndianBOM[] = { 0xFF, 0xFE };

	static const Data<uint8_t> bigEndianBOM(_bigEndianBOM, 2);
	static const Data<uint8_t> littleEndianBOM(_littleEndianBOM, 2);

	class String : public Type, public Comparable<String> {

	public:

		static Type::Kind typeKind();

		static String fromCString(
			function<size_t(char*,size_t)> todo,
			const size_t size);

		static
		__attribute__ ((format (printf, 1, 0)))
		String format(const char* format, ...);

		static Strong<String> fromHex(
			const Data<uint8_t>& data);

		static Strong<String> join(
			const Array<String>& strings);

		static Strong<String> join(
			const Array<String>& strings,
			const String& separator);

		String();

		String(
			const char* string
		) noexcept(false);

		String(
			const Data<uint8_t>& data,
			bool nullTerminated = true
		) noexcept(false);

		String(
			const Data<uint16_t>& data,
			const Endian::Variant& endian,
			bool nullTerminated = true
		) noexcept(false);

		String(
			const Data<uint16_t>& data
		) noexcept(false);

		String(
			const Data<uint32_t>& store);

		String(
			const uint32_t character);

		String(
			const String& other);

		String(
			String&& other);

		virtual ~String();

		size_t length() const;

		template<typename T>
		T mapCString(
			const function<T(const char*)>& todo
		) const {
			return todo((const char*)this->UTF8Data(true)->items());
		}

		void withCString(
			const function<void(const char*)>& todo
		) const;

		template<typename T>
		T withCString(
			function<T(const char*)> todo
		) const {
			return todo((const char*)this->UTF8Data(true)->items());
		}

		void print(
			bool newLine = true
		) const;

		Strong<Data<uint8_t>> UTF8Data(
			bool nullTerminate = false
		) const;

		Strong<Data<uint16_t>> UTF16Data(
			Endian::Variant endian = Endian::systemVariant(),
			bool includeBOM = false
		) const;

		Strong<Data<uint32_t>> UTF32Data() const;

		Strong<Data<uint8_t>> hexData() const;

		void append(
			const String& other);

		void append(
			uint32_t character);

		Strong<String> appending(
			const String& other
		) const;

		Strong<Array<String>> split() const;

		Strong<Array<String>> split(
			const String& separator,
			IncludeSeparator includeSeparator = IncludeSeparator::none,
			size_t max = 0
		) const;

		Strong<Array<String>> split(
			const Array<String>& separators,
			IncludeSeparator includeSeparator = IncludeSeparator::none,
			size_t max = 0
		) const;

		Strong<String> replacing(
			const String& needle,
			const String& replacement
		) const;

		double doubleValue(
			size_t startIndex = 0,
			size_t* consumed = nullptr,
			bool allowLeadingZero = true
		) const noexcept(false);

		size_t indexOf(
			const String& other,
			size_t offset = 0
		) const;

		size_t indexOf(
			uint32_t chr
		) const;

		size_t lastIndexOf(
			const String& other
		) const;

		bool contains(
			const String& other
		) const;

		bool hasPrefix(
			const String& other
		) const;

		bool hasSuffix(
			const String& other
		) const;

		Strong<String> substring(
			size_t offset,
			size_t length = NotFound
		) const;

		Strong<String> slicing(
			ssize_t start = 0,
			ssize_t end = math::limit<ssize_t>()
		) const;

		Strong<String> uppercased() const;

		Strong<String> lowercased() const;

		Strong<String> capitalized() const;

		Strong<String> trimmedStart() const;

		Strong<String> trimmedEnd() const;

		Strong<String> trimmed() const;

		virtual uint64_t hash() const override;

		virtual Kind kind() const override;

		virtual Strong<Type> clone() const override;

		bool operator==(
			const Type& other
		) const = delete;

		bool operator==(
			const char* other
		) const = delete;

		bool operator>(
			const String& other
		) const = delete;

		virtual bool equals(
			const Type& other
		) const override;

		bool equals(
			const char* other
		) const;

		virtual bool greaterThan(
			const String& other
		) const override;

		uint32_t operator[](
			size_t idx
		) const;

		String& operator=(
			const String& other);

		String& operator=(String&& other) {
			Type::operator=(std::move(other));
			_storage = std::move(other._storage);
			return *this;
		}

	private:

		enum class DoublePart {
			integer = 0,
			fraction,
			exponent
		};

		DataValue<uint32_t> _storage;

		template<typename T>
		static size_t _lengthWithoutNullTerminator(
			const Data<T>& data
		) {
			size_t length = data.length();
			if (length > 0 && data.itemAtIndex(length - 1) == 0) {
				return length - 1;
			}
			return length;
		}

		static Data<uint32_t> _decodeUTF8(
			const uint8_t* buffer,
			size_t length
		) noexcept(false);

		static Data<uint8_t> _encodeUTF8(
			const Data<uint32_t>& buffer,
			bool nullTerminate = false
		) noexcept(false);

		static Data<uint32_t> _decodeUTF16(
			const uint16_t* buffer,
			size_t length,
			Endian::Variant endian
		) noexcept(false);

		static Data<uint16_t> _encodeUTF16(
			const Data<uint32_t>& buffer,
			Endian::Variant endian,
			bool includeBOM
		) noexcept(false);

		static Data<uint32_t> _decodeUTF32(
			const Data<uint32_t>& buffer,
			Endian::Variant endian);

		static Data<uint32_t> _encodeUTF32(
			const Data<uint32_t>& buffer,
			Endian::Variant endian);

		static uint8_t _valueFromHex(
			uint8_t chr,
			size_t idx
		) noexcept(false);

		static uint8_t _valueToHex(
			uint8_t value,
			size_t idx
		) noexcept(false);

		static Data<uint32_t> _decodeHex(
			const Data<uint8_t>& buffer
		) noexcept(false);

		static Data<uint8_t> _encodeHex(
			const Data<uint32_t> &buffer
		) noexcept(false);

	};

}

#endif /* foundation_string_hpp */
