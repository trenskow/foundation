//
// type.cpp
// foundation
//
// Created by Kristian Trenskow on 2018/09/10.
// See license in LICENSE.
//

#include <utility>

#include "type.hpp"

using namespace foundation::types;

Type::Type() : Object(), Hashable() { }

Type::Kind Type::kind() const {
	throw NotImplementedException();
}

Strong<Type> Type::clone() const {
	throw NotImplementedException();
}

Type& Type::operator=(
	const Type& other
) {
	Object::operator=(other);
	Hashable::operator=(other);
	return *this;
}

Type& Type::operator=(
	Type&& other
) {
	Object::operator=(std::move(other));
	Hashable::operator=(std::move(other));
	return *this;
}

bool Type::equals(
	const Type& other
) const {
	if (this->kind() != other.kind()) return false;
	return this->hash() == other.hash();
}

bool Type::is(
	Kind kind
) const {
	return kind == this->kind();
}
