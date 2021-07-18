//
//  weak.hpp
//  fart
//
//  Created by Kristian Trenskow on 04/09/2018.
//  Copyright © 2018 Kristian Trenskow. All rights reserved.
//

#ifndef weak_hpp
#define weak_hpp

#include <type_traits>
#include <atomic>

namespace fart::memory {

	class Object;

	template<class T>
	class Strong;

	template<class T>
	class Weak {

		static_assert(std::is_base_of<Object, T>::value);

		friend class Object;

	private:
		std::atomic<T*> _object;

		void _setObject(T* object) {
			if (_object != nullptr) {
				_object->removeWeakReference(this);
			}
			_object = object;
			if (_object) {
				_object->addWeakReference(this);
			}
		}

	public:
		Weak() : Weak(nullptr) {}
		Weak(T& object) : Weak(&object) {}

		Weak(T* object) : _object(nullptr) {
			_setObject(object);
		}

		Weak(std::nullptr_t) : _object(nullptr) {
			_setObject(nullptr);
		}

		Weak(const Strong<T>& other) : Weak(other) {}

		Weak(Weak<T>&& other) : _object(other._object) {
			other._object = nullptr;
		}

		~Weak() {
			_setObject(nullptr);
		}

		bool isNull() const {
			return _object == nullptr;
		}

		operator T&() const {
			return *_object;
		}

		operator T*() const {
			return _object;
		}

		Weak<T>& operator =(T& object) {
			_setObject(&object);
			return *this;
		}

		Weak<T>& operator =(T&& other) {
			_setObject(other._object);
			other._setObject(nullptr);
			return *this;
		}

		Weak<T>& operator =(T* object) {
			_setObject(object);
			return *this;
		}

		Weak<T>& operator =(Strong<T> object) {
			_setObject(object);
			return *this;
		}

		T* operator ->() const {
			return _object;
		}

		bool operator==(std::nullptr_t n) {
			return this->_object == n;
		}

		bool operator!=(std::nullptr_t n) {
			return !(this == n);
		}

#ifdef FART_ALLOW_MANUAL_HEAP

		void *operator new(size_t size) {
			return Object::allocate(size);
		}

		void operator delete(void *ptr) throw() {
			Object::deallocate(ptr);
		}

#else
		void *operator new(size_t size) = delete;
		void operator delete(void *ptr) throw() = delete;
#endif

	};

}

#endif /* weak_hpp */
