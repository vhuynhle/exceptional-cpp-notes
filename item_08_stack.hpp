#pragma once

#include <cstddef>

template <typename T>
class Stack {
public:
    /// 1. This function is exception neutral: We don't catch anything.
    /// 2. We don't leak: If `new` fails and throws std::bad_alloc,
    /// nothing has been allocated yet.
    /// 3. We're always in a consistent state. If an exception is thrown
    /// here, no object is created yet, so there's nothing to be inconsistent.
    Stack()
        : v_ { new T[10] }
        , vsize_ { 10 }
        , vused_ { 0 }
    {
    }

    ~Stack() noexcept
    {
        // This delete[] operator is noexcept
        delete[] v_;
    }

private:
    T* v_;
    std::size_t vsize_;
    std::size_t vused_;
};