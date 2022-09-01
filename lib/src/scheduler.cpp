// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/scheduler.h>

namespace sydney {
thread_pool scheduler::blocking_thread_pool_;
static thread_local local_scheduler ls;

local_scheduler *scheduler::get_local_scheduler() {
    return &ls;
}
}
