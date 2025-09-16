#ifndef VECTOR_BASIC_H
#define VECTOR_BASIC_H

#include <vector>
#include <cstddef> // size_t

template <class T>
class Vector_Basic {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;
public:
    // Can create a vector from an std::vector
    // DO NOT MODIFY THIS FUNCTION
    Vector_Basic(const std::vector<T>& v): array(new T[v.capacity()]), _capacity(v.capacity()), _size(0) {
        for (const auto& elem: v) {
            array[_size] = elem;
            _size++;
        }
    }

    // Should return an iterator pointing to the first element
    iterator begin() noexcept {
        return iterator(array);
    }

    // Should return an iterator pointing past the last element of the container
    // This is because you want to stop when you reach end
    iterator end() noexcept {
        return iterator(array+_size); // PAST the last element
    }

    // Iterator class for the Vector_Basic
    // It is essentially a pointer defined for the type with memory safety
    class iterator {
    public:
        // These are characteristics that can be accessed from an iterator
        // For example, you could get the type from an iterator called iter
        // by using iter::value_type
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Points to some element in the vector (or nullptr)
        T* _ptr;
    public:
        iterator() : _ptr(nullptr) {}
        explicit iterator(T* ptr) : _ptr(ptr) {}

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        // This is used to access the element the iterator points to, like dereferencing a pointer
        [[nodiscard]] reference operator*() const noexcept { return *_ptr; }

        // This is used to access data members of the underlying elements
        // It should return the address of the element the iterator points to
        [[nodiscard]] pointer operator->() const noexcept { return _ptr; }

        // Prefix Increment: ++a
        // The value returned should be the iterator moved forward in the container
        iterator& operator++() noexcept { 
            _ptr += 1;
            return *this; 
        }

        // Postfix Increment: a++
        // The value returned should be the iterator before it was moved forward in the container
        iterator operator++(int) noexcept { 
            iterator copy = *this;
            _ptr += 1;
            return copy;
        }

        // Prefix Decrement: --a
        // The value returned should be the iterator moved backwards in the container
        iterator& operator--() noexcept { 
            _ptr -= 1;
            return *this; 
        }

        // Postfix Decrement: a--
        // The value returned should be the iterator before it was moved backwards in the container
        iterator operator--(int) noexcept {
            iterator copy = *this;
            _ptr -= 1;
            return copy;
        }

        // Should move the iterator forward by an offset and return the new iterator
        iterator& operator+=(difference_type offset) noexcept { _ptr += offset;
            return *this;
        }
        
        // Should return a copy of the iterator moved forward by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator copy = *this;
            copy._ptr += offset;
            return copy; 
        }
        
        // Should move the iterator backwards by an offset and return the new iterator
        iterator& operator-=(difference_type offset) noexcept { _ptr -= offset;
            return *this;
        }

        // Should return a copy of the iterator moved backwards by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            iterator copy = *this;
            copy._ptr -= offset;
            return copy; 
        }

        // Should return the difference between two iterators. This should be the offset between the
        // elements in memory.
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { return _ptr - rhs._ptr; }

        // Should return the element at the location offset from the iterator
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { return *(_ptr+offset); }

        // Used for comparing iterators
        // NOTE: This should be used to compare the iterators, not the elements they point to
        // HINT: Because iterators mimic pointers, many if not all of these operations can be done
        //       on the underlying pointers
        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            if (_ptr == rhs._ptr) {
                return true;
            }
            return false;
        }

        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {            
             if (_ptr == rhs._ptr) {
                return false;
            }
            return true; 
        }

        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { 
            if (_ptr < rhs._ptr) {
                return true;
            }
            else {return false;}
        }

        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {            
            if (_ptr > rhs._ptr) {
                return true;
            }
            else {return false;}
        }

        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept {
            if (_ptr <= rhs._ptr) {
                return true;
            }
            else {return false;}
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept {
            if (_ptr >= rhs._ptr) {
                return true;
            }
            else {return false;}
        }
    };
};

#endif
