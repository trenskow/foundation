//
// run-loop.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#include "./run-loop.hpp"

using namespace foundation::parallelism;

RunLoop::RunLoop(
	uint64_t id
) : Object(),
	_id(id),
    _mutex(),
    _thread(nullptr),
    _tasks(),
    _shouldStop(false) { }

RunLoop::~RunLoop() {
	this->stop();
}

size_t RunLoop::taskCount() const {
	return this->_mutex.locked<size_t>([&]() {
		return this->_tasks.length();
	});
}

RunLoop& RunLoop::start() {

	if (!this->_thread.equals(nullptr)) {
		return *this;
	}

	this->_shouldStop = false;

	this->_thread = Strong<Thread>(
		String::format("RunLoop-%llu", this->_id),
		[&]() {

			this->_mutex.lock();

			while (!this->_tasks.isEmpty() || !this->_shouldStop) {

				if (!this->_tasks.isEmpty()) {

					auto task = this->_tasks.head();

					this->_tasks.pop();

					this->_mutex.unlock();
					task();
					this->_mutex.lock();

				}

				if (!this->_shouldStop) {
					this->_mutex.wait();
				}

			}

			this->_mutex.unlock();

		});

	return *this;

}

void RunLoop::stop() {

	this->_mutex.locked([&]() {

		if (this->_thread.equals(nullptr)) {
			return;
		}

		this->_shouldStop = true;

		this->_mutex.notify();

	});

	this->_thread = nullptr;

}

void RunLoop::post(std::function<void()> function) {

	this->_mutex.locked([&]() {

		this->_tasks.push(function);
		this->_mutex.notify();

	});

}
