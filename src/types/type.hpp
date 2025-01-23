//
// type.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2018/09/10.
// See license in LICENSE.
//

#ifndef shared_foundation_type_hpp
#define shared_foundation_type_hpp

#include <stdint.h>

#include "../memory/object.hpp"
#include "./hashable.hpp"
#include "../exceptions/exception.hpp"
#include "../tools/math.hpp"

using namespace games::zerobit::shared::foundation::memory;
using namespace games::zerobit::shared::foundation::exceptions::types;
using namespace games::zerobit::shared::foundation::tools;

namespace games::zerobit::shared::foundation::types {

	static const size_t NotFound = math::limit<size_t>();

	class Type : public Object, public Hashable {

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

		Type& operator=(
			const Type& other
		);

		Type& operator=(
			Type&& other);

		virtual bool operator==(
			const Type& other
		) const;

		virtual bool operator!=(
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

#endif /* shared_foundation_type_hpp */
