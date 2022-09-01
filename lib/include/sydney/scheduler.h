// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_SCHEDULER_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_SCHEDULER_H_

#include "thread_pool.h"
#include "local_scheduler.h"

namespace sydney {

template <class T>
class coroutine;

template <class T>
class awaitable_future;

class scheduler {
    template <class T>
    friend class awaitable_future;

    template <class Ret>
    friend void spawn(coroutine<Ret> coro);

    template <class Function, class... Args>
    friend awaitable_future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>> spawn_blocking(Function &&f,
                                                                                                                Args &&... args);

    template <class Ret>
    friend void start_at(coroutine<Ret> coro);

    static thread_pool blocking_thread_pool_;

    [[nodiscard]] static local_scheduler *get_local_scheduler();
};
} // sydney

namespace co = sydney;

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_SCHEDULER_H_
