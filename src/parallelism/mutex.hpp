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
			~Mutex();

			void lock();
			void unlock();

			void locked(std::function<void()> function);

			template<typename T>
			T locked(std::function<T()> function) {

				T result;

				this->lock();

				try {
					result = function();
				} catch (...) {
					this->unlock();
					throw;
				}

				this->unlock();

				return result;

			}

			void wait();
			void notify();
			void broadcast();

		private:

			pthread_mutexattr_t _mutexAttributes;
			pthread_mutex_t _mutex;
			pthread_cond_t _condition;

	};

}

#endif // foundation_parallelism_mutex_hpp
