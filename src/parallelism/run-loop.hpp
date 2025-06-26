//
// run-loop.hpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#ifndef foundation_parallelism_run_loop_hpp
#define foundation_parallelism_run_loop_hpp

#include <functional>
#include <queue>

#include "../memory/strong.hpp"

#include "./mutex.hpp"
#include "./thread.hpp"

using namespace foundation::memory;

namespace foundation::parallelism {

	class RunLoop {

		public:

			RunLoop();
			~RunLoop();

			void run();
			void stop();

			void post(std::function<void()> function);

		private:

			Mutex _mutex;

			Strong<Thread> _thread;
			std::queue<std::function<void()>> _tasks;

			bool _shouldStop = false;

	};

}

#endif // foundation_parallelism_run_loop_hpp
