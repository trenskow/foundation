//
// null.cpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "null.hpp"

using namespace games::zerobit::shared::foundation::types;

bool Null::is(const Type* value) {
	return value == nullptr || value->kind() == Type::Kind::null;
}

Null::Null() : Type() { }

Null::Null(
	nullptr_t
) : Type() { }

Type::Kind Null::typeKind() {
	return Type::Kind::null;
}

Type::Kind Null::kind() const {
	return Kind::null;
}

bool Null::operator==(
	const Type& other
) const {
	return other.kind() == Kind::null;
}
