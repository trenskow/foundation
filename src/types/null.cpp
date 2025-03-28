//
// null.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include <stddef.h>

#include "null.hpp"

using namespace foundation::types;

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

Strong<Type> Null::clone() const {
	return Strong<Null>()
		.as<Type>();
}

bool Null::equals(
	const Type& other
) const {
	return other.kind() == Kind::null;
}
