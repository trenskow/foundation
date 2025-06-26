//
// run-loop.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/06/26.
// See license in LICENSE.
//

#include "./run-loop.hpp"

using namespace foundation::parallelism;

RunLoop::RunLoop()
	: _mutex(),
	  _thread(nullptr),
	  _tasks(),
	  _shouldStop(false) { }

RunLoop::~RunLoop() {
	this->stop();
}

void RunLoop::run() {

	this->_mutex.locked([&]() {

		if (!this->_thread.equals(nullptr)) {
			return;
		}

		this->_shouldStop = false;

		this->_thread = Strong<Thread>([&]() {

			this->_mutex.lock();

			while (!this->_shouldStop) {

				while (!this->_tasks.empty()) {

					auto task = this->_tasks.front();

					this->_tasks.pop();

					this->_mutex.unlock();
					task();
					this->_mutex.lock();

				}

				if (this->_shouldStop) {
					break;
				}

				this->_mutex.wait();

			}

			this->_mutex.unlock();

		});

	});

}

void RunLoop::stop() {

	this->_mutex.locked([&]() {

		if (this->_thread.equals(nullptr)) {
			return;
		}

		this->_shouldStop = true;
		this->_mutex.notify();

		this->_thread = nullptr;

	});

}

void RunLoop::post(std::function<void()> function) {

	this->_mutex.locked([&]() {

		this->_tasks.push(function);
		this->_mutex.notify();

	});

}
