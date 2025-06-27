//
// thread.hpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#ifndef foundation_parallelism_thread_hpp
#define foundation_parallelism_thread_hpp

#include <pthread.h>
#include <functional>

#include "../memory/object.hpp"

namespace foundation::parallelism {

	class Thread: public foundation::memory::Object {

		public:

			Thread(std::function<void()> function);
			Thread(const Thread&) = delete;
			Thread(Thread&&) = delete;

			virtual ~Thread();

		private:

			pthread_t _thread;
			std::function<void()> _function;

	};

}

#endif // foundation_parallelism_thread_hpp
