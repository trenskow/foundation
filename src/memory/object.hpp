//
// object.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2018/08/17.
// See license in LICENSE.
//

#ifndef shared_foundation_object_hpp
#define shared_foundation_object_hpp

#include <unistd.h>
#include <atomic>

#include "./allocator.hpp"

namespace games::zerobit::shared::foundation::memory {

	class Object : public Allocator {

		template<typename T>
		friend class Strong;

		template<typename T>
		friend class Weak;

	private:

		mutable std::atomic<size_t> _retainCount;
		mutable void** _weakReferences;
		mutable size_t _weakReferencesSize;
		mutable size_t _weakReferencesCount;

		void addWeakReference(void* weakReference) const;

		void removeWeakReference(void* weakReference) const;

	public:

		Object();

		Object(
			const Object&);

		Object(
			Object&&);

		virtual ~Object();

		Object& operator=(
			const Object&);

		Object& operator=(
			Object&&);

	public:

		void retain() const;
		void release() const;

		size_t retainCount() const;

	};

}

#endif /* shared_foundation_object_hpp */
