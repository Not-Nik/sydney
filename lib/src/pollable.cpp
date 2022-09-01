// sydney (c) Nikolas Wipper 2022

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sydney/pollable.h>

namespace sydney {

namespace pollable_specialisations {
class always_ready_pollable : public ::sydney::pollable {
public:
    /// Always returns true
    [[nodiscard]] bool is_ready() const final { return true; }
};

class never_ready_pollable : public ::sydney::pollable {
public:
    /// Always returns false
    [[nodiscard]] bool is_ready() const final { return false; }
};
}

static pollable_specialisations::always_ready_pollable ar_pollable;
static pollable_specialisations::never_ready_pollable nr_pollable;

pollable *pollable::always_ready = &ar_pollable;
pollable *pollable::never_ready = &nr_pollable;
}
