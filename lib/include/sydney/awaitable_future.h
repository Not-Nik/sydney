// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_AWAITABLE_FUTURE_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_AWAITABLE_FUTURE_H_

#include "pollable.h"
#include "scheduler.h"
#include "local_scheduler.h"

#include <future>
#include <iostream>

namespace sydney {
template <class T>
class awaitable_future : public pollable {
    std::future<T> inner_;
public:
    explicit awaitable_future(std::future<T> future)
        : inner_(std::move(future)) {}

    awaitable_future(const awaitable_future &) = delete;
    awaitable_future(awaitable_future &&other) noexcept
        : inner_(std::move(other.inner_)) {}

    [[nodiscard]] bool is_ready() const final {
        return inner_.wait_for(std::chrono::steady_clock::duration::zero()) == std::future_status::ready;
    }

    /// Called once, when using `co_await awaitable_future()`
    bool await_ready() {
        return inner_.wait_for(std::chrono::steady_clock::duration::zero()) == std::future_status::ready;
    }

    /// Called once if @ref await_ready returned false
    void await_suspend(const std::coroutine_handle<> &handle) {
        // Tell the scheduler to wait for this before continuing the current coroutine
        scheduler::get_local_scheduler()->set_task_pollable(this);
    }

    /// Called once to receive this futures value
    T await_resume() {
        inner_.get();
    }
};
} // sydney

namespace co = sydney;

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_AWAITABLE_FUTURE_H_
