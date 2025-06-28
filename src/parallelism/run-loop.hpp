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

#include "../memory/strong.hpp"
#include "../memory/object.hpp"
#include "../tools/queue.hpp"

#include "./mutex.hpp"
#include "./thread.hpp"

using namespace foundation::memory;
using namespace foundation::tools;

namespace foundation::parallelism {

	class RunLoop : public Object {

		public:

			RunLoop(
				uint64_t id = 0);

			RunLoop(const RunLoop&) = delete;
			RunLoop(RunLoop&&) = delete;

			virtual ~RunLoop();

			size_t taskCount() const;

			RunLoop& start();
			void stop();

			void post(std::function<void()> function);

		private:

			uint64_t _id;

			Mutex _mutex;

			Strong<Thread> _thread;
			Queue<std::function<void()>> _tasks;

			bool _shouldStop = false;

	};

}

#endif // foundation_parallelism_run_loop_hpp
