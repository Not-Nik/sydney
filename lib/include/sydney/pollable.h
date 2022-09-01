// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SYDNEY_LIB_INCLUDE_SYDNEY_POLLABLE_H_
#define SYDNEY_LIB_INCLUDE_SYDNEY_POLLABLE_H_

namespace sydney {

/**
 * @brief sydney's base pollable class
 *
 * Is used to signal whether a coroutine should continue executing after suspending
 */
class pollable {
public:
    [[nodiscard]] virtual bool is_ready() const = 0;

    static pollable *always_ready;
    static pollable *never_ready;
};
} // sydney

#endif //SYDNEY_LIB_INCLUDE_SYDNEY_POLLABLE_H_
