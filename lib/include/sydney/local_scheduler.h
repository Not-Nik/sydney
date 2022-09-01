// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_LOCAL_SCHEDULER_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_LOCAL_SCHEDULER_H_

#include "pollable.h"

#include <map>
#include <list>
#include <memory>
#include <coroutine>

namespace sydney {
namespace detail {
struct resumable_coroutine {
    std::coroutine_handle<> handle;
    pollable *status;

    resumable_coroutine(std::coroutine_handle<> h, pollable *s)
        : handle(h), status(s) {}
};
}

class local_scheduler {
    template <class T>
    friend class awaitable_future;

    std::list<detail::resumable_coroutine> tasks_;
    std::list<detail::resumable_coroutine>::iterator current_task_;

    void set_task_pollable(pollable *status);
public:
    local_scheduler() = default;
    local_scheduler(const local_scheduler &) = delete;
    local_scheduler(local_scheduler &&) = delete;

    void start();
    void add_task(std::coroutine_handle<> coro);
};
} // sydney

namespace co = sydney;

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_LOCAL_SCHEDULER_H_
