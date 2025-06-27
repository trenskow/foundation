//
// mutex.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#include "./mutex.hpp"

using namespace foundation::parallelism;

Mutex::Mutex() {
	pthread_mutexattr_init(&_mutexAttributes);
	pthread_mutexattr_settype(&_mutexAttributes, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&_mutex, &_mutexAttributes);
	pthread_cond_init(&_condition, nullptr);
}

Mutex::~Mutex() {
	pthread_cond_destroy(&_condition);
	pthread_mutex_destroy(&_mutex);
	pthread_mutexattr_destroy(&_mutexAttributes);
}

void Mutex::lock() const {
	pthread_mutex_lock(&_mutex);
}

void Mutex::unlock() const {
	pthread_mutex_unlock(&_mutex);
}

void Mutex::locked(
	std::function<void()> function
) const {
	(void)this->locked<void*>([&]() {
		function();
		return nullptr;
	});
}

void Mutex::wait() {
	pthread_cond_wait(&_condition, &_mutex);
}

void Mutex::notify() {
	pthread_cond_signal(&_condition);
}

void Mutex::broadcast() {
	pthread_cond_broadcast(&_condition);
}
