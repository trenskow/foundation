//
// allocator.cpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include <math.h>

#include "../tools/math.hpp"

#include "../exceptions/exception.hpp"

#include "allocator.hpp"

#define FOUNDATION_BLOCK_SIZE 32

using namespace games::zerobit::shared::foundation::memory;
using namespace games::zerobit::shared::foundation::exceptions::memory;
using namespace games::zerobit::shared::foundation::tools;

void* Allocator::operator new(
	size_t size
) noexcept(false) {
	return allocate(size);
}

void Allocator::operator delete(
	void *ptr
) throw() {
	deallocate(ptr);
}

uint64_t Allocator::calculateBufferLength(
	const uint64_t& minimumLength
) {
	return (uint64_t)pow(2, ceil(log2((math::max<double>(minimumLength, FOUNDATION_BLOCK_SIZE) / FOUNDATION_BLOCK_SIZE)))) * FOUNDATION_BLOCK_SIZE;;
}

void* Allocator::allocate(
	size_t size
) noexcept(false) {
	void *mem = calloc(size, sizeof(uint8_t));
	if (!mem) throw AllocationException(size);
	return mem;
}

void Allocator::deallocate(
	void* ptr
) {
	free(ptr);
}
