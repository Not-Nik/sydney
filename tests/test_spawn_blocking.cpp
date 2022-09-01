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

static int status = 0;

sydney::coroutine<void> async_main() {
    auto now = std::chrono::steady_clock::now();

    co_await sydney::spawn_blocking(std::this_thread::sleep_for<std::chrono::milliseconds::rep, std::chrono::milliseconds::period>,
                                    500ms);

    auto duration = std::chrono::steady_clock::now() - now;

    if (duration >= 1ms) {
        status = 42;
    }
}

int main() {
    sydney::start_at(async_main());
    return (status == 42) ? 0 : 1;
}
