//
// mutex.hpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#ifndef foundation_parallelism_mutex_hpp
#define foundation_parallelism_mutex_hpp

#include <pthread.h>

#include <functional>

namespace foundation::parallelism {

	class Mutex {

		public:

			Mutex();
			Mutex(const Mutex&) = delete;
			Mutex(Mutex&&) = delete;

			~Mutex();

			void lock() const;
			void unlock() const;

			void locked(
				std::function<void()> function
			) const;

			template<typename T>
			T locked(
				std::function<T()> function
			) const {

				this->lock();

				try {
					T result = function();
					this->unlock();
					return result;
				} catch (...) {
					this->unlock();
					throw;
				}

			}

			void wait();
			void notify();
			void broadcast();

		private:

			pthread_mutexattr_t _mutexAttributes;
			mutable pthread_mutex_t _mutex;
			mutable pthread_cond_t _condition;

	};

}

#endif // foundation_parallelism_mutex_hpp
