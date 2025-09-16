#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node(nullptr) {};
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            // TODO
            return node->data;
        }
        pointer operator->() const {
            // TODO
            return &(node->data);

        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            // TODO
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            // TODO
            basic_iterator copy = *this;
            node = node->next;
            return copy;

        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            // TODO
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            // TODO
            basic_iterator copy = *this;
            node = node->prev;
            return copy;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            // TODO
            if (node == other.node) {
                return true;
            }
            return false;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            // TODO
            if (node == other.node) {
                return false;
            }
            return true;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head(), tail(), _size(0) {
        // TODO - Don't forget to initialize the list beforehand
        head.next = &tail;
        tail.prev = &head;

    }
    List( size_type count, const T& value ) : List() {
        // TODO - Don't forget initialize the list beforehand

        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
        
    }
    explicit List( size_type count ) : List() {
        // TODO - Don't forget initialize the list beforehand

        for (size_t i = 0; i < count; ++i) {
            push_back(T{}); // Pushes no value
        }

    }
    List( const List& other ) : List() {
        // TODO - Don't forget initialize the list beforehand
        Node* temp = other.head.next;

        for(size_t i = 0; i < other.size(); ++i) { // Until end of list
            push_back(temp->data); // Copy node over
            temp = temp->next; // Move to next node
        }

    }
    List( List&& other ) : List() {
        // TODO - Don't forget initialize the list beforehand
        if (!other.empty()) {
            head.next = other.head.next;
            tail.prev = other.tail.prev;
            head.next->prev = &head;
            tail.prev->next = &tail;
            other.head.next = &(other.tail);
            other.tail.prev = &(other.head);

            _size = other.size();
            other._size = 0;
        }

    }
    ~List() {
        // TODO
        Node* temp = head.next;
        while(temp != &tail) {
            Node* after = temp->next;
            delete temp;
            temp = after;
        }
    }
    List& operator=( const List& other ) {
        // TODO
        if (this != &other) {
            clear();

            Node* temp = other.head.next;

            for(size_t i = 0; i < other.size(); ++i) {
                push_back(temp->data); 
                temp = temp->next;
            }
            return *this;
        }

        return *this;

    }
    List& operator=( List&& other ) noexcept {
        // TODO
        if (this != &other) {
            clear();

            if (!other.empty()) {
                head.next = other.head.next;
                tail.prev = other.tail.prev;
                head.next->prev = &head;
                tail.prev->next = &tail;
                other.head.next = &(other.tail);
                other.tail.prev = &(other.head);

                _size = other.size();
                other._size = 0;
            }

            return *this;
        }
        return *this;
    }

    reference front() {
        // TODO
        
        return head.next->data;
    }
    const_reference front() const {
        // TODO
        return head.next->data;
    }
	
    reference back() {
        // TODO
        return tail.prev->data;
    }
    const_reference back() const {
        // TODO
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        // TODO
        return iterator((head.next));
    }
    const_iterator begin() const noexcept {
        // TODO
        return const_iterator((head.next));
    }
    const_iterator cbegin() const noexcept {
        // TODO
        return const_iterator((head.next));
    }

    iterator end() noexcept {
        // TODO
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        // TODO
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        // TODO
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        // TODO
        if (_size == 0) {
            return true;
        }
        return false;
    }

    size_type size() const noexcept {
        // TODO
        return _size;
    }

    void clear() noexcept {
        // TODO
        while (_size != 0) {
            pop_back();
        }
    }

    iterator insert( const_iterator pos, const T& value ) {
        // TODO
        Node* temp = new Node(value, pos.node->prev, pos.node);

        temp->prev->next = temp;
        temp->next->prev = temp;

        ++_size;

        return iterator(temp);

    }
    iterator insert( const_iterator pos, T&& value ) {
        // TODO
        Node* temp = new Node(std::move(value), pos.node->prev, pos.node);

        temp->prev->next = temp;
        temp->next->prev = temp;

        ++_size;

        return iterator(temp);

    }

    iterator erase( const_iterator pos ) {
        // TODO

        Node* temp = pos.node;


        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;

        --_size;

        iterator index = iterator(temp->next);

        delete temp;

        return index;

    }

    void push_back( const T& value ) {
        // TODO
        Node* node = new Node(value, tail.prev, &tail);

        tail.prev->next = node; // Before setting tail.prev
        tail.prev = node;

        _size++;
        
    }
    void push_back( T&& value ) {
        // TODO
        Node* node = new Node(std::move(value), tail.prev, &tail);

        tail.prev->next = node; // Before setting tail.prev
        tail.prev = node;

        _size++;
    }

    void pop_back() {
        // TODO
        if (_size == 0) {return;}
        Node* temp = tail.prev;
        temp->prev->next = &tail;
        tail.prev = temp->prev; 

        delete temp;

        --_size;
    }
	
    void push_front( const T& value ) {
        // TODO
        Node* node = new Node();
        node->prev = &head; // Initializing
        node->next = head.next;
        node->data = value;
        
        head.next->prev = node; // Before setting head.prev
        head.next = node;
        
        _size++;
    }
	void push_front( T&& value ) {
        // TODO
        Node* node = new Node();
        node->prev = &head; // Initializing
        node->next = head.next;
        node->data = std::move(value);
        
        head.next->prev = node; // Before setting head.prev
        head.next = node;
        
        _size++;
    }

    void pop_front() {
        // TODO
        if (_size == 0) {return;}
        Node* temp = head.next;
        temp->next->prev = &head;
        head.next = temp->next; 

        delete temp;

        --_size;

    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}