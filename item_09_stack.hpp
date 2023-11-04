#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

template <typename T>
class Stack {
public:
    Stack()
        : v_ { new T[10] }
        , vsize_ { 10 }
        , vused_ { 0 }
    {
    }

    ~Stack() noexcept
    {
        delete[] v_;
    }

    T* NewCopy(const T* src, std::size_t src_size, std::size_t dest_size)
    {
        assert(dest_size >= src_size);

        // Same as the explanation for the default constructor:
        // we're OK even if this throws.
        T* dest = new T[dest_size];

        try {
            // Copy constructor of T may throw, so this copy may throw.
            std::copy(src, src + src_size, dest);
        } catch (...) {
            // Need to free the memory allocated, then re-throw
            delete[] dest;
            throw;
        }

        return dest;
    }

    Stack(const Stack& other)
        : v_ { NewCopy(other.v_, other.vsize_, other.vsize_) }
        , vsize_ { other.vsize_ }
        , vused_ { other.vused_ }
    {
    }

    Stack& operator=(const Stack& other)
    {
        // Handle self-assignment
        if (this == &other) {
            return *this;
        }

        // Create a temporary stack first, so if this fails, the old stack is intact.
        Stack* tmp = new Stack(other);

        // Delete the old stack, this cannot throw
        delete[] v_;

        // Take the content of the new stack
        v_ = tmp->v_;
        vsize_ = tmp->vsize_;
        vused_ = tmp->vused_;

        return *this;
    }

private:
    T* v_;
    std::size_t vsize_;
    std::size_t vused_;
};
