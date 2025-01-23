//
// allocator.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2021/07/19.
// See license in LICENSE.
//

#ifndef shared_foundation_allocator_hpp
#define shared_foundation_allocator_hpp

namespace games::zerobit::shared::foundation::memory {

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

#endif /* shared_foundation_allocator_hpp */
