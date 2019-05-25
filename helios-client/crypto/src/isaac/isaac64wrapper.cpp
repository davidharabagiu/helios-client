#include <chrono>

#include "isaac/isaac64wrapper.h"

extern "C" {
#include "isaac/isaac64.h"
}

Isaac64Wrappwer::Isaac64Wrappwer()
{
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch())
                    .count();
    randrsl[0] = static_cast<ub8>(time);
    randinit(TRUE);
}

uint64_t Isaac64Wrappwer::get()
{
    return rand();
}
