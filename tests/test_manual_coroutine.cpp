// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/sydney.h>

sydney::coroutine<int> my_int_coroutine() {
    co_return 42;
}

sydney::coroutine<void> my_void_coroutine() {
    co_return; // explicit co_return so the compiler knows this is a coroutine
}

int main() {
    sydney::block_on(my_void_coroutine());
    return (sydney::block_on(my_int_coroutine()) == 42) ? 0 : 1;
}
