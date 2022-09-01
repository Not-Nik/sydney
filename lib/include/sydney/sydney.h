// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_SYDNEY_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_SYDNEY_H_

#include "scheduler.h"
#include "coroutine.h"
#include "awaitable_future.h"

#include <future>
#include <type_traits>

namespace sydney {

/**
 * @brief Spawns a new asynchronous task
 *
 * The function is called as if a new std::thread was created, i.e. by executing
 * `std::invoke(std::move(f_copy), std::move(args_copy)...)`, where
 *
 *  - f_copy is an object of type `std::decay<Function>::type` and constructed from `std::forward<Function>(f)`, and
 *  - args_copy... are objects of types `std::decay<Args>::type...` and constructed from `std::forward<Args>(args)...`.
 *
 * @param coro Coroutine to execute
 */
template <class Ret>
void spawn(coroutine<Ret> coro) {
    scheduler::get_local_scheduler()->add_task(coro);
}

/**
 * @brief Spawns a blocking task in a new thread
 *
 * Creates a new thread, or uses an existing one from the thread pool and executes f on it as if it was directly run
 * on it, i.e. by executing `std::invoke(std::move(f_copy), std::move(args_copy)...)`, where
 *
 *  - f_copy is an object of type `std::decay<Function>::type` and constructed from `std::forward<Function>(f)`, and
 *  - args_copy... are objects of types `std::decay<Args>::type...` and constructed from `std::forward<Args>(args)...`,
 *
 * storing its result in an awaitable future
 *
 * @param f Function to execute
 * @param args Parameter to pass
 * @returns An awaitable future to the return value of `f`
 */
template <class Function, class... Args>
awaitable_future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>> spawn_blocking(Function &&f,
                                                                                                     Args &&... args) {
    auto *promise = new std::promise<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>();

    auto runner = [promise, f, args...]() {
        if constexpr (std::is_same_v<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>, void>) {
            std::invoke(std::move(std::decay_t<Function>(f)), std::move(std::decay_t<Args>(args...))...);
            promise->set_value();
        } else {
            promise->set_value(std::invoke(std::move(std::decay_t<Function>(f)),
                                           std::move(std::decay_t<Args>(args...))...));
        }
        delete promise;
    };

    scheduler::blocking_thread_pool_.queue_job(runner);

    return awaitable_future(promise->get_future());
}

/**
 * @brief Blocks the current thread until @a coro is done executing
 *
 * If this call did not originate in a @ref sydney::start_at call, while @a coro executes, using any sydney functions
 * other than @ref sydney::block_on and @ref sydney::start_at causes <b>undefined behaviour</b>. Using @ref
 * sydney::start_at in coro lifts this requirement for the passed coroutine only. Using @ref sydney::block_on in coro
 * behaves exactly like described here.
 * If this call did originate in a @ref sydney::start_at, execution continues as if coro was a normal function that
 * was called normally, meaning `co_await my_coroutine()` is semantically identical to `sydney::block_on(my_coroutine)`.
 *
 * @param coro Coroutine to execute
 */
template <class Ret>
Ret block_on(coroutine<Ret> coro) {
    while (!coro.done()) coro.resume();
    if constexpr (!std::is_void_v<Ret>) return coro.promise().ret;
}

/**
 * @brief Starts asynchronous executing with @a coro
 *
 * Uses @ref sydney::spawn to spawn @a coro. Then initiates the scheduler. Calling this function within a coroutine
 * that originated in another @ref sydney::start_at call causes <b>undefined behaviour</b>.
 *
 * @param coro Coroutine to execute
 */
template <class Ret>
void start_at(coroutine<Ret> coro) {
    spawn(coro);
    scheduler::get_local_scheduler()->start();
}
} // sydney

namespace co = sydney;

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_SYDNEY_H_
