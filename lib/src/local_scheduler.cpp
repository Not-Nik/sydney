// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/local_scheduler.h>

namespace sydney {

void local_scheduler::set_task_pollable(pollable *status) {
    current_task_->status = status;
}

void local_scheduler::start() {
    while (!tasks_.empty()) {
        for (current_task_ = tasks_.begin(); current_task_ != tasks_.end();) {
            if (!current_task_->status->is_ready()) {
                current_task_++;
                continue;
            }

            // replace pollable, if ready. if this points to an `awaitable_future` that just turned ready, it'll be
            // destroyed during the next `resume` call
            current_task_->status = pollable::always_ready;
            current_task_->handle.resume();
            if (current_task_->handle.done()) {
                current_task_ = tasks_.erase(current_task_);
            } else {
                continue;
            }
        }
    }
}

void local_scheduler::add_task(std::coroutine_handle<> coro) {
    if (!coro.done()) tasks_.emplace_back(coro, pollable::always_ready);
}
}
