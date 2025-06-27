//
// strong.hpp
// foundation
//
// Created by Kristian Trenskow on 2018/08/17.
// See license in LICENSE.
//

#ifndef foundation_strong_hpp
#define foundation_strong_hpp

#include <assert.h>

#include <type_traits>
#include <functional>

#include "./allocator.hpp"
#include "./object.hpp"

using namespace std;

namespace foundation::memory {

	template<typename T>
	class Weak;

	template<typename T = Object>
	class Strong : public Allocator {

		static_assert(is_base_of<Object, T>::value);

	private:

		T* _object;

		void _setObject(T* object, bool newObject = false) {

			T* oldObject = _object;

			if (object) {
				// If object is allocated on the stack, we make a copy on the heap.
				if (!newObject && object->retainCount() == 0) {
					if constexpr (is_copy_constructible<T>::value) {
						_object = new T(*object);
					} else {
						assert(false);
					}
				} else {
					_object = object;
				}
				_object->retain();
			}

			if (oldObject != nullptr) {
				oldObject->release();
				oldObject = nullptr;
			}

		}

	public:

		Strong(nullptr_t) : _object(nullptr) {};
		Strong(T& object) : Strong(&object) {};

		Strong(const T& object) : Strong(nullptr) {
			if constexpr (is_copy_constructible<T>::value) {
				_setObject(new T(object), true);
			} else {
				_setObject(&(T&)object);
			}
		}

		Strong(T* object) : _object(nullptr) {
			_setObject(object);
		}

		explicit Strong(const T* object) : _object(nullptr) {
			_setObject((T*)object);
		}

		Strong(const Strong<T>& other) : _object(nullptr) {
			_setObject(other._object);
		}

		Strong(Strong<T>&& other) {
			this->_object = other._object;
			other._object = nullptr;
		}

		Strong(const Weak<T>& other) : _object(other) {}

		template<typename... Args>
		explicit Strong(Args&&... args) : _object(nullptr) {
			_setObject(new T(std::forward<Args>(args)...), true);
		}

		~Strong() {
			_setObject(nullptr);
		}

		operator T&() const {
			return *_object;
		}

		operator T*() const {
			return _object;
		}

		Strong<T>& operator =(T& object) {
			_setObject(&object);
			return *this;
		}

		Strong<T>& operator =(T* object) {
			_setObject(object);
			return *this;
		}

		Strong<T>& operator =(Weak<T> object) {
			_setObject(object);
			return *this;
		}

		Strong<T>& operator =(const Strong<T>& object) {
			_setObject(object._object);
			return *this;
		}

		Strong<T>& operator =(Strong<T>&& other) {
			this->_setObject(nullptr);
			this->_object = other._object;
			other._object = nullptr;
			return *this;
		}

		T* operator ->() const {
			return _object;
		}

		bool operator==(std::nullptr_t) const = delete;
		bool operator!=(std::nullptr_t) const = delete;
		bool operator==(const T& other) const = delete;
		bool operator!=(const T& other) const = delete;

		bool equals(std::nullptr_t) const {
			return this->_object == nullptr;
		}

		template<typename O>
		Strong<O> as() const {
			return Strong<O>((O*)_object);
		}

		template<typename O>
		Strong<O> map(function<const Strong<O>(T& unwrapped)> todo) const {
			if (_object == nullptr) return nullptr;
			return todo(*this);
		}

		Strong<T> def(T& value) const {
			if (this->_object == nullptr) return value;
			return this->_object;
		}

		Strong<T> with(function<void(T&)> todo) {
			if (this->_object != nullptr) todo(*this->_object);
			return *this;
		}

		Strong<T> retained() const {
			return Strong<T>(this->_object);
		}

	};

}

#endif /* foundation_strong_hpp */
