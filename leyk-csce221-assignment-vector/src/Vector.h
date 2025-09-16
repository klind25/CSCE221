#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        if (_capacity != 0) {
            _capacity *= 2;
        }
        else {
            _capacity = 1;
        }
        
        T* temp = new T [_capacity]();

        std::move(array, array+_size, temp);
        delete[] array;
        array = temp;
        
    }

public:
    Vector() noexcept : array(nullptr), _capacity(0), _size(0) {}

    Vector(size_t count, const T& value) { 
        array = new T[count];
        for (size_t i = 0; i < count; ++i) {
            array[i] = value;
        }
        _capacity = count;
        _size = count;

    }

    explicit Vector(size_t count) { 
        array = new T[count]();
        _capacity = count;
        _size = count;
    }

    Vector(const Vector& other) { 
        array = new T[other.capacity()];
        std::copy(other.array,other.array+other.size(),array); // Takes in iterators
        _capacity = other.capacity();
        _size = other.size();
    }

    Vector(Vector&& other) noexcept : array(other.array) { 
        _capacity = other.capacity();
        _size = other.size();

        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;

    }

    ~Vector() { 
        delete[] array;
    }

    Vector& operator=(const Vector& other) { 
        if (other.array != array) {
            delete[] array;
            array = new T[other.capacity()];
            std::copy(other.array,other.array+other.size(),array);
            _capacity = other.capacity();
            _size = other.size();
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (other.array != array) {
            delete[] array;
            array = other.array;
            _size = other.size();
            _capacity = other.capacity();
                
            other.array = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
            
        return *this;
    }

    iterator begin() noexcept {
        return iterator(array);
    }

    iterator end() noexcept {
        return iterator(array+_size); // PAST the last element
    }

    [[nodiscard]] bool empty() const noexcept { 
        if (array == nullptr) {
            return true;
        } 
        return false;
    }
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }

    T& at(size_t pos) { 
        if (pos >= _capacity) {
            throw std::out_of_range("Out of range");
        }
        return array[pos]; 
    }

    const T& at(size_t pos) const  { 
        if (pos >= _capacity) {
            throw std::out_of_range("Out of range");
        }
        return array[pos]; 
    }

    T& operator[](size_t pos) { return array[pos]; }
    const T& operator[](size_t pos) const { return array[pos]; }
    T& front() { return array[0]; }
    const T& front() const { return array[0]; }
    T& back() { return array[_size-1]; }
    const T& back() const { return array[_size-1]; }

    void push_back(const T& value) { 
        if (_size == _capacity) {
            grow();
        } 
        array[_size] = value;
        ++_size;
    }
    void push_back(T&& value) {  
        if (_size == _capacity) {
            grow();
        }
        
        array[_size] = std::move(value);
        ++_size;
    }
    void pop_back() { 
        array[_size-1] = 0;
        --_size;
    }

    iterator insert(iterator pos, const T& value) { 
        size_t index = pos - begin(); 

        if (_size == _capacity) {
            grow();
            pos = begin() + index; // Pointer lost during growth
        } 
        
        for (size_t i = _size; i > index; --i) {
            array[i] = std::move(array[i-1]);
        }           
        
        array[index] = value;
        
        ++_size;
        return pos;
        
    }

    iterator insert(iterator pos, T&& value) { 
        size_t index = pos - begin(); 

        if (_size == _capacity) {
            grow();
            pos = begin() + index; // Pointer lost during growth
        }

        for (size_t i = _size; i > index; --i) {
            array[i] = std::move(array[i-1]);
        }           
        
        array[index] = std::move(value);
        
        ++_size;
        return pos;

    }

    iterator insert(iterator pos, size_t count, const T& value) { 
        size_t index = pos - begin(); 

        while (_size + count > _capacity) {
            grow();
            pos = begin() + index; // Pointer lost during growth
        }
        
        for (size_t i = _size; i > index; --i) {
            array[i+count-1] = std::move(array[i-1]);
        }           
        
        for (size_t i = 0; i < count; ++i) {
            array[index+i] = value;
        }
        
        _size += count;
        
        return pos;
    }

    iterator erase(iterator pos) { 
        if (pos == end()) { // Edge case
            return pos;
        }
        std::move(pos+1, end(), pos);
        
        --_size;
        return pos;
    }

    iterator erase(iterator first, iterator last) { 
        if (first == last) { // Edge case, nothing changed
            return first;
        }
        size_t width = last - first;
        
        std::move(last, end(), first);

        _size -= width;

        
        return first;
    }

    void clear() noexcept { 
        for (size_t i = 0; i < _size; ++i) {
            array[i] = 0;
        }
        _size = 0;
    }

    class iterator {
    public:
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

        [[nodiscard]] reference operator*() const noexcept { return *_ptr; }
        [[nodiscard]] pointer operator->() const noexcept { return _ptr; }
        iterator& operator++() noexcept { 
            _ptr += 1;
            return *this; 
        }

        iterator operator++(int) noexcept { 
            iterator copy = *this;
            _ptr += 1;
            return copy;
        }

        iterator& operator--() noexcept { 
            _ptr -= 1;
            return *this; 
        }
        iterator operator--(int) noexcept {
            iterator copy = *this;
            _ptr -= 1;
            return copy;
        }

        iterator& operator+=(difference_type offset) noexcept {
            _ptr += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator copy = *this;
            copy._ptr += offset;
            return copy; 
        }
        iterator& operator-=(difference_type offset) noexcept { 
            _ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            iterator copy = *this;
            copy._ptr -= offset;
            return copy; 
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { return _ptr - rhs._ptr; }
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { return *(_ptr+offset); }
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

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { 
    _Iterator copy = iterator;
    copy += offset;
    return copy; 
}

#endif
