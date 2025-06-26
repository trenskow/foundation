//
// allocator.hpp
// foundation
//
// Created by Kristian Trenskow on 2021/07/19.
// See license in LICENSE.
//

#ifndef foundation_allocator_hpp
#define foundation_allocator_hpp

#include <stdint.h>

namespace foundation::memory {

	class Allocator {

	public:

		void *operator new(size_t size) noexcept(false);
		void operator delete(void *ptr) throw();

	protected:

		static uint64_t calculateBufferLength(
			const uint64_t& minimumLength);

		static void* allocate(
			size_t size
		) noexcept(false);

		static void deallocate(
			void* ptr);

	};

	class NoAllocator {

	public:

		void *operator new(size_t size) = delete;
		void operator delete(void *ptr) throw() = delete;

	};

}

#endif /* foundation_allocator_hpp */
