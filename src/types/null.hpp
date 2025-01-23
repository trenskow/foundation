//
// null.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2020/04/01.
// See license in LICENSE.
//

#ifndef shared_foundation_null_hpp
#define shared_foundation_null_hpp

#include "type.hpp"

namespace games::zerobit::shared::foundation::types {

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

#endif /* shared_foundation_null_hpp */
