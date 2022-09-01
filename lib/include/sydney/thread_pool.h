// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_THREAD_POOL_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_THREAD_POOL_H_

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

namespace sydney {
class thread_pool {
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable mutex_condition_;
    bool should_terminate_ = false, should_shutdown_ = false;

    void loop();
    void clear_threads();

public:
    thread_pool();
    ~thread_pool();

    /**
     * @brief Add a new job to the pool's queue
     *
     * @param job Function to execute
     */
    void queue_job(std::function<void()> job);

    /**
     * @brief Stop all threads, leaving existing jobs unexecuted
     */
    void terminate();

    /**
     * @brief Execute all jobs, then stop all threads
     */
    void shutdown();
};
} // sydney

namespace co = sydney;

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_THREAD_POOL_H_
