//
// hashable.cpp
// foundation
//
// Created by Kristian Trenskow on 2022/01/22.
// See license in LICENSE.
//

#include "hashable.hpp"

using namespace foundation::types;
using namespace foundation::tools;

Hashable::Builder::Builder() : _hash(5381) { }

Hashable::Builder& Hashable::Builder::add(
	uint64_t value
) {
	_hash = ((_hash << 5) + _hash) + (uint64_t)value;
	return *this;
}

Hashable::Builder& Hashable::Builder::add(
	double value
) {
	uint64_t intValue = 0;
	memcpy(&intValue, &value, math::min(sizeof(uint64_t), sizeof(double)));
	return this->add(intValue);
}

Hashable::Builder& Hashable::Builder::add(
	float value
) {
	return this->add((double)value);
}

Hashable::Builder& Hashable::Builder::add(
	const Hashable& hashable
) {
	return this->add(hashable.hash());
}

Hashable::Builder::operator uint64_t() {
	return _hash;
}

Hashable::Hashable() { }

Hashable::Hashable(
	const Hashable&
) { }

Hashable::Hashable(
	Hashable&&
) { }

uint64_t Hashable::hash() const {
	return this->hashBuilder();
}

Hashable& Hashable::operator=(
	const Hashable&
) {
	return *this;
}

Hashable& Hashable::operator=(
	Hashable&&
) {
	return *this;
}

Hashable::Builder Hashable::hashBuilder() const {
	throw NotImplementedException();
}
