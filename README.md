# sydney

A C++ 20 coroutine runtime

## Usage

```cpp
#include <sydney/sydney.h>

sydney::coroutine<void> async_main() {
    // Do your async programming here
    co_return;
}

int main() {
    sydney::start_at(async_main());
    return 0;
}
```

## I/O Operations

sydney currently doesn't abstract system specific async I/O functions, but you can use `sydney::spawn_blocking` to run
blocking functions on a thread from a thread pool.
