#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Stack {
public:
    // 1. This function is exception neutral: We don't catch anything.
    // 2. We don't leak:
    //    2.1 If `new` throws std::bad_alloc, nothing has been allocated yet.
    //    2.2 If `new` can allocate the memory, but one of T's default constructor throws,
    //        any constructed T's objects will be destructed. Assuming that T's destructor
    //        does not throw, then the objects will be cleaned up correctly.
    // 3. We're always in a consistent state. If an exception is thrown
    // here, no object is created yet, so there's nothing to be inconsistent.
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

    Stack(Stack&& other)
    {
        // Use swap and let other's destructor do the cleaning job
        using std::swap;
        swap(v_, other.v_);
        swap(vsize_, other.vsize_);
        swap(vused_, other.vused_);
    }

    Stack& operator=(Stack&& other)
    {
        // Use swap and let other's destructor do the cleaning job
        using std::swap;
        swap(v_, other.v_);
        swap(vsize_, other.vsize_);
        swap(vused_, other.vused_);
        return *this;
    }

    std::size_t Count() const
    {
        return vused_;
    }

    void Push(const T& obj)
    {
        // Re-allocate if needed
        if (vused_ == vsize_) {
            const auto new_size = (vsize_ > 0) ? 2 * vsize_ : 10;
            T* tmp = NewCopy(v_, vsize_, new_size);

            // Delete old stack, this will not throw
            delete[] v_;

            // Take the new contents
            v_ = tmp;
            vsize_ = new_size;
        }

        v_[vused_] = obj;
        ++vused_;
    }

    void Pop()
    {
        if (vused_ == 0) {
            throw std::out_of_range("Empty stack");
        }

        --vused_;
    }

    T& Top()
    {
        if (vused_ == 0) {
            throw std::out_of_range("Empty stack");
        }
        return v_[vused_ - 1];
    }

private:
    T* v_;
    std::size_t vsize_;
    std::size_t vused_;
};
