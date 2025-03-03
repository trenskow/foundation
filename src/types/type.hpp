//
// type.hpp
// foundation
//
// Created by Kristian Trenskow on 2018/09/10.
// See license in LICENSE.
//

#ifndef foundation_type_hpp
#define foundation_type_hpp

#include <stdint.h>

#include "../memory/object.hpp"
#include "./hashable.hpp"
#include "./cloneable.hpp"
#include "../exceptions/exception.hpp"
#include "../tools/math.hpp"

using namespace foundation::memory;
using namespace foundation::exceptions::types;
using namespace foundation::tools;

namespace foundation::types {

	static const size_t NotFound = math::limit<size_t>();

	class Type : public Object, public Hashable, public Cloneable<Type> {

	public:

		enum class Kind : uint8_t {
			data,
			string,
			number,
			array,
			dictionary,
			date,
			pair,
			uuid,
			url,
			null
		};

		Type();

		Type(
			const Type& other
		) = delete;

		Type(
			Type&& other
		) = delete;

		virtual Kind kind() const;

		virtual Strong<Type> clone() const override;

		Type& operator=(
			const Type& other
		);

		Type& operator=(
			Type&& other);

		bool operator==(
			const Type& other
		) const = delete;

		bool operator!=(
			const Type& other
		) const = delete;

		virtual bool equals(
			const Type& other
		) const;

		bool is(
			Kind kind
		) const;

		template<typename T>
		inline T& as() const {
			return (T&)*this;
		}

		template<typename T>
		T& as(Kind kind) const {
			if (!this->is(kind)) throw TypeConversionException();
			return this->as<T>();
		}

	};

}

#endif /* foundation_type_hpp */
