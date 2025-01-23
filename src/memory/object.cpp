//
// object.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include <assert.h>

#include "../exceptions/exceptions.hpp"
#include "./weak.hpp"

#include "./object.hpp"

using namespace foundation::memory;
using namespace foundation::exceptions::memory;

void Object::addWeakReference(
	void* weakReference
) const {
	if (_weakReferencesSize < _weakReferencesCount + 1) {
		_weakReferencesSize = Allocator::calculateBufferLength(_weakReferencesCount + 1);
		_weakReferences = (void**)realloc(_weakReferences, _weakReferencesSize);
		_weakReferences[_weakReferencesCount++] = weakReference;
	}
}

void Object::removeWeakReference(
	void* weakReference
) const {
	for (size_t idx = 0 ; idx < _weakReferencesCount ; idx++) {
		if (_weakReferences[idx] == weakReference) {
			for (size_t midx = idx ; midx <= _weakReferencesCount - 1 ; midx++) {
				_weakReferences[midx] = _weakReferences[midx + 1];
			}
			_weakReferencesCount--;
		}
	}
}

Object::Object(
) : _retainCount(0),
	_weakReferences(nullptr),
	_weakReferencesSize(0),
	_weakReferencesCount(0) { }

Object::Object(
	const Object&
) : Object() { }

Object::Object(
	Object&&
) : Object() { }

Object::~Object(
) {
	assert(_retainCount == 0);
	for (size_t idx = 0 ; idx < _weakReferencesCount ; idx++) {
		((Weak<Object>*)_weakReferences[idx])->_object = nullptr;
	}
	if (_weakReferences != nullptr) {
		free(_weakReferences);
	}
}

Object& Object::operator=(
	const Object&
) {
	return *this;
}

Object& Object::operator=(
	Object&&
) {
	return *this;
}

void Object::retain(
) const {
	this->_retainCount++;
}

void Object::release(
) const {
	if (--this->_retainCount == 0) delete this;
}

size_t Object::retainCount(
) const {
	return this->_retainCount;
}
