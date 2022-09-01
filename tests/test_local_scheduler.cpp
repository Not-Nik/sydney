// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/sydney.h>

static int check1 = 0, check2 = 0;

sydney::coroutine<void> other_async_task1() {
    check1 = 42;
    co_return; // explicit co_return so the compiler knows this is a coroutine
}

sydney::coroutine<void> other_async_task2() {
    check2 = 42;
    co_return; // explicit co_return so the compiler knows this is a coroutine
}

sydney::coroutine<void> async_main() {
    sydney::spawn(other_async_task1());
    sydney::spawn(other_async_task2());
    co_return; // explicit co_return so the compiler knows this is a coroutine
}

int main() {
    sydney::start_at(async_main());

    return (check1 == 42 and check2 == 42) ? 0 : 1;
}
