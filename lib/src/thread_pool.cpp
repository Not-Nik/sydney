// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/thread_pool.h>

namespace sydney {

thread_pool::thread_pool() {
    const uint32_t num_threads = std::thread::hardware_concurrency();
    threads_.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        threads_.at(i) = std::thread(&thread_pool::loop, this);
    }
}

thread_pool::~thread_pool() {
    terminate();
}

void thread_pool::loop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            mutex_condition_.wait(lock, [this] {
                return !jobs_.empty() || should_terminate_ || should_shutdown_;
            });
            if (should_terminate_ || (should_shutdown_ && jobs_.empty())) {
                return;
            }
            job = jobs_.front();
            jobs_.pop();
        }
        job();
    }
}

void thread_pool::clear_threads() {
    for (std::thread &active_thread: threads_) {
        active_thread.join();
    }
    threads_.clear();
}

void thread_pool::queue_job(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        jobs_.push(std::move(job));
    }
    mutex_condition_.notify_one();
}

void thread_pool::terminate() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        should_terminate_ = true;
    }
    mutex_condition_.notify_all();
    for (std::thread &active_thread: threads_) {
        active_thread.join();
    }
    threads_.clear();
}

void thread_pool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        should_shutdown_ = true;
    }
    mutex_condition_.notify_all();
    clear_threads();
}
}
