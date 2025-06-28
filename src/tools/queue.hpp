//
// queue.hpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#ifndef foundation_tools_queue_hpp
#define foundation_tools_queue_hpp

#include <stdlib.h>

#include "../memory/object.hpp"
#include "../exceptions/exception.hpp"

using namespace foundation::memory;

namespace foundation::tools {

	template<typename T>
	class Queue : public Object {

		public:

			Queue() {
				_capacity = FOUNDATION_BLOCK_SIZE;
				_data = (T*)calloc(FOUNDATION_BLOCK_SIZE, sizeof(T));
				_length = 0;
				_head = 0;
				_tail = 0;
			}

			Queue(
				const Queue<T>& other
			) = delete;

			Queue(
				Queue<T>&& other
			) = delete;

			virtual ~Queue() {
				for (size_t idx = 0 ; idx < this->_length ; idx++) {
					this->_data[(this->_head + idx) % this->_capacity].~T();
				}
				free(this->_data);
			}

			size_t length() const {
				return this->_length;
			}

			bool isEmpty() const {
				return this->_length == 0;
			}

			void push(const T& item) {
				
				if (this->_length + 1 > this->_capacity) {

					size_t newCapacity = Allocator::calculateBufferLength(this->_capacity + 1);
					T* newData = (T*)calloc(this->_capacity + 1, sizeof(T));

					for (size_t idx = 0 ; idx < this->_length ; idx++) {
						new (&newData[idx]) T(
							std::move(this->_data[(this->_head + idx) % this->_capacity]));
					}

					free(this->_data);

					this->_data = newData;
					this->_capacity = newCapacity;
					this->_head = 0;
					this->_tail = this->_length;

				}

				new (&this->_data[this->_tail]) T(item);

				this->_tail = (this->_tail + 1) % this->_capacity;
				this->_length++;

			}

			T& head() const noexcept(false) {
				if (this->_length == 0) {
					throw exceptions::tools::EmptyException();
				}
				return this->_data[this->_head];
			}

			void pop() noexcept(false) {

				if (this->_length == 0) {
					throw exceptions::tools::EmptyException();
				}

				this->_data[this->_head].~T();
				this->_head = (this->_head + 1) % this->_capacity;
				this->_length--;

				if (this->_length == 0) {
					this->_head = 0;
					this->_tail = 0;
				}

			}
		
		private:

			T* _data;

			size_t _capacity;
			size_t _length;
			size_t _head;
			size_t _tail;
		
	};

}

#endif /* foundation_tools_queue_hpp */
