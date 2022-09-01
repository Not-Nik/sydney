// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/thread_pool.h>

int main() {
    int status = 0;
    {
        sydney::thread_pool tp;

        tp.queue_job([&status]() {
            status = 42;
        });

        tp.shutdown();
    }

    return (status == 42) ? 0 : 1;
}
