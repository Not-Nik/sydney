// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/sydney.h>

static int status = 0;

sydney::coroutine<int> recursive_coroutine2() {
    co_return 42;
}

sydney::coroutine<int> recursive_coroutine1() {
    co_return co_await recursive_coroutine2();
}

sydney::coroutine<int> async_main() {
    status = co_await recursive_coroutine1();
}

int main() {
    sydney::start_at(async_main());
    return (status == 42) ? 0 : 1;
}
