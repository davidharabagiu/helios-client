#ifndef IDLEPROCESSOR_H
#define IDLEPROCESSOR_H

#include <cstdint>
#include <functional>

class IdleProcessor
{
public:
    static void processEventsUntilTimeout(int64_t timeout, bool yieldThread = true);

    static void processEventsUntilPredicate(const std::function<bool()>& predicate, int64_t timeout = -1,
                                            bool yieldThread = true);
};

#endif  // IDLEPROCESSOR_H
