// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_COROUTINE_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_COROUTINE_H_

#include <utility>
#include <coroutine>
#include <type_traits>

namespace sydney {

template <class T>
struct promise;

template <class Ret>
Ret block_on(coroutine<Ret> coro);

/**
 * @brief sydney's main coroutine class
 */
template <class T>
struct coroutine : std::coroutine_handle<promise<T>> {
    using promise_type [[maybe_unused]] = struct promise<T>;

    [[maybe_unused]] coroutine(std::coroutine_handle<promise<T>> handle)
        : std::coroutine_handle<promise<T>>(std::move(handle)) {}

    /**
     * @brief Called once, when using `co_await my_coroutine()`
     *
     * Uses @ref sydney::block_on to execute the coroutine
     *
     * @returns Always `true`
     */
    bool await_ready() {
        if (!this->done()) block_on(*this);
        return true;
    }

    /**
     * @brief Called once if @ref sydney::coroutine::await_ready returned false
     *
     * Not called in reality because `co_await`ing a coroutine never suspends the current one directly
     */
    void await_suspend(std::coroutine_handle<>) {}

    /**
     * @brief Called once to receive this coroutines return value
     */
    T await_resume() {
        if (!std::is_void_v<T>)
            return this->promise().ret;
    }
};

template <class T>
struct promise {
    coroutine<T> get_return_object() { return coroutine<T>::from_promise(*this); }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}

    void return_value(T expr) {
        ret = expr;
    }

    T ret;
};

template <>
struct promise<void> {
    coroutine<void> get_return_object() { return coroutine<void>::from_promise(*this); }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}

    void return_void() {}
};
}

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_COROUTINE_H_
