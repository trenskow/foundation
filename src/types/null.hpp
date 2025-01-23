//
// null.hpp
// foundation
//
// Created by Kristian Trenskow on 2020/04/01.
// See license in LICENSE.
//

#ifndef foundation_null_hpp
#define foundation_null_hpp

#include "type.hpp"

namespace foundation::types {

	class Null: public Type {

	public:

		static bool is(
			const Type* value);

		Null();

		Null(
			nullptr_t);

		static Type::Kind typeKind();

		virtual Kind kind() const override;

		virtual bool operator==(
			const Type& other
		) const override;

	};

}

#endif /* foundation_null_hpp */
