//
// thread.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#include "./thread.hpp"

using namespace foundation::parallelism;
using namespace foundation::types;

Thread::Thread(
#if defined(__APPLE__) || defined(__linux__)
	const String name,
#else
	const String,
#endif
	std::function<void()> function
) : foundation::memory::Object(),
    _function([=]() {
#if defined(__APPLE__) || defined(__linux__)
    	name.withCString([&](const char* cString) {
#if defined(__APPLE__)
    		pthread_setname_np(cString);
#elif defined(__linux__)
    		pthread_setname_np(pthread_self(), cString);
#endif
    	});
#endif
    	function();
    }) {
	
	pthread_create(&this->_thread, nullptr, [](void* arg) -> void* {
		auto func = static_cast<std::function<void()>*>(arg);
		(*func)();
		return nullptr;
	}, &this->_function);

}

Thread::~Thread() {
	pthread_join(this->_thread, nullptr);
	this->_function = nullptr;
}
