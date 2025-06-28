//
// thread.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#include "./thread.hpp"

using namespace foundation::parallelism;
using namespace foundation::types;

Thread::Thread(
	const String name,
	std::function<void()> function
) : foundation::memory::Object(),
    _function([=]() {
    	name.withCString([&](const char* cString) {
    		pthread_setname_np(cString);
    	});
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
