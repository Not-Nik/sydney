// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/sydney.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

static bool check = false;
static int status = 0;

sydney::coroutine<void> set_check() {
    co_await sydney::spawn_blocking(std::this_thread::sleep_for<std::chrono::milliseconds::rep, std::chrono::milliseconds::period>,
                                    250ms);
    check = true;
}

sydney::coroutine<void> async_main() {
    sydney::spawn(set_check());

    co_await sydney::spawn_blocking(std::this_thread::sleep_for<std::chrono::milliseconds::rep, std::chrono::milliseconds::period>,
                                    500ms);

    if (check) {
        status = 42;
    }
}

int main() {
    sydney::start_at(async_main());
    return (status == 42) ? 0 : 1;
}
