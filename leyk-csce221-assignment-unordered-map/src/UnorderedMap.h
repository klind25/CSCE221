#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept: _map(map), _ptr(ptr) { /* TODO */ }

    public:
        basic_iterator(): _map(nullptr), _ptr(nullptr) { };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { return _ptr->val; }
        pointer operator->() const { return &(_ptr->val); }
        basic_iterator &operator++() { 
            if (_ptr == nullptr) {
                return *this;
            }
            
            if (_ptr->next == nullptr) {
                size_t index = _map->_bucket(_ptr->val.first);
                for (size_t i = index+1; i < _map->_bucket_count; ++i) {
                    if (_map->_buckets[i] != nullptr) {
                        _ptr = _map->_buckets[i];
                        return *this;
                    }
                }
                _ptr = nullptr;
                return *this;
            }
            else {
                _ptr= _ptr->next;
            }
            return *this; 
        }

        basic_iterator operator++(int) { 

            basic_iterator copy = *this;
            ++(*this);
            return copy; 
            
            }

        bool operator==(const basic_iterator &other) const noexcept { return _ptr==other._ptr; }
        bool operator!=(const basic_iterator &other) const noexcept { return _ptr!=other._ptr; }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept: _node(node) { }

        public:
            local_iterator() : _node(nullptr) { }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { return _node->val; }
            pointer operator->() const { return &(_node->val); }
            local_iterator & operator++() { 
                if (_node == nullptr) {
                    return *this;
                }
                _node = _node->next;
                return *this; }
            local_iterator operator++(int) { 
                if (_node == nullptr) {
                    return *this;
                }
                local_iterator copy = *this;
                _node = _node->next;
                return copy; }

            bool operator==(const local_iterator &other) const noexcept { return _node==other._node; }
            bool operator!=(const local_iterator &other) const noexcept { return _node!=other._node; }
    };

private:

    size_type _bucket(size_t code) const { return _range_hash(code, _bucket_count); }
    size_type _bucket(const Key & key) const { return _range_hash(_hash(key),_bucket_count); }
    size_type _bucket(const value_type & val) const { return _bucket(val.first); }

    HashNode* _find(size_type bucket, const Key & key) { 
        if(_buckets == nullptr) {
            return nullptr;
        }

        HashNode* node = _buckets[bucket];
        while (node) {
            if (_equal(node->val.first, key)) {
                return node;
            }
            node = node->next;
        }
        
        return node;
    }

    HashNode* _find(const Key & key) { return _find(_bucket(key),key); }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) { 
        HashNode *node = new HashNode(std::move(value), _buckets[bucket][0]);
        _buckets[bucket][0] = node;

        if (_head == nullptr || _bucket(_head->val) >= bucket) {
            _head = node;
        }
        return node;

    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
        const key_equal & equal = key_equal { }) : _size(0)  {  
            _bucket_count = next_greater_prime(bucket_count);
            
            _buckets = new HashNode*[_bucket_count]();

            _head = nullptr;
        }

    ~UnorderedMap() { 
        for (size_t i = 0; i < _bucket_count; ++i) { // Go through buckets
            while(_buckets[i] != nullptr) { // Go through linked list
                HashNode* next = _buckets[i]->next; 
                delete _buckets[i];
                _buckets[i] = next;
            }
            delete _buckets[i];
        }
        delete[] _buckets;
    }

    UnorderedMap(const UnorderedMap & other) : _bucket_count(other.bucket_count()), _head(other._head), _size(0), _hash(other._hash), _equal(other._equal) {  
        _buckets = new HashNode*[_bucket_count]();

        for (const_iterator i = other.cbegin(); i != other.cend(); ++i) {
            insert(*i);
        }
    }

    UnorderedMap(UnorderedMap && other) : _bucket_count(other.bucket_count()), _head(other._head), _size(other.size()), _hash(other._hash), _equal(other._equal) {  
        _buckets = new HashNode*[_bucket_count]();

        for (size_t i = 0; i < _bucket_count; ++i) {
            _buckets[i] = other._buckets[i];
            other._buckets[i] = nullptr;
        }
        other._head = nullptr;
        other._size = 0;
    }

    UnorderedMap & operator=(const UnorderedMap & other) { 
        if (this != &other) {
            clear();
            delete[] _buckets;

            _bucket_count = other.bucket_count(); 
            _head = other._head;
            _size = 0;
            _hash = other._hash;
            _equal = other._equal;
    
            _buckets = new HashNode*[_bucket_count]();
    
            for (const_iterator i = other.cbegin(); i != other.cend(); ++i) {
                insert(*i);
            }

            return *this;
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) { 
        if (this != &other) {
            clear();
            delete[] _buckets;

            _bucket_count = other.bucket_count(); 
            _head = other._head;
            _size = other.size();
            _hash = other._hash;
            _equal = other._equal;

            _buckets = new HashNode*[_bucket_count]();

            for (size_t i = 0; i < _bucket_count; ++i) {
                _buckets[i] = other._buckets[i];
                other._buckets[i] = nullptr;
            }
            other._head = nullptr;
            other._size = 0;

            return *this;
        }

        return *this;
    }

    void clear() noexcept { 
        if (_buckets == nullptr) {
            return;
        }
        iterator traverse = begin();

        while(traverse._ptr != nullptr) {
            traverse = erase(traverse);
            if (traverse._ptr == nullptr) {
                ++traverse;
            }
        }

        _size = 0;
    }

    size_type size() const noexcept { return _size; }

    bool empty() const noexcept { return _size == 0; }

    size_type bucket_count() const noexcept { return _bucket_count; }

    iterator begin() { return iterator(this, _head); }
    iterator end() { return iterator(this, nullptr);}

    const_iterator cbegin() const { return const_iterator(this,_head); }
    const_iterator cend() const { return const_iterator(this,nullptr);}

    local_iterator begin(size_type n) { return local_iterator(_buckets[n]); }
    local_iterator end(size_type n) { return local_iterator(nullptr); }

    size_type bucket_size(size_type n) {  
        size_t length = 0;
        auto t = begin(n);

        while(t != end(n)) {
            ++t;
            ++length;
        }
        
        return length;
    }

    float load_factor() const { return (_size*1.0f) / _bucket_count; }

    size_type bucket(const Key & key) const { return _bucket(key); }

    std::pair<iterator, bool> insert(value_type && value) { 
        bool inserted = false;

        auto inside = find(value.first);

        if (inside._ptr == nullptr) {
            inserted = true;

            size_type index = _bucket(value);

            HashNode* node = new HashNode(std::move(value), _buckets[index]);
            _buckets[index] = node;

            if (_head == nullptr || _bucket(_head->val) >= index) {
                _head = node;
            }
            ++_size;

            return std::pair(iterator(this, node), inserted);
        }

        return std::pair(inside, inserted);
    }

    std::pair<iterator, bool> insert(const value_type & value) { 
        bool inserted = false;

        auto inside = find(value.first);

        if (inside._ptr == nullptr) {
            inserted = true;

            size_type index = _bucket(value);

            HashNode* node = new HashNode(value, _buckets[index]);
            _buckets[index] = node;

            if (_head == nullptr || _bucket(_head->val) >= index) {
                _head = node;
            }
            ++_size;

            return std::pair(iterator(this, node), inserted);
        }

        return std::pair(inside, inserted);
    }

    iterator find(const Key & key) { return iterator(this, _find(key)); }

    T& operator[](const Key & key) { 
        HashNode* node = _find(key);
        if(node) { // Exists
            return node->val.second;
        }
        value_type val = value_type(key, T{}); // T{} puts default
        insert(val);

        node = _find(key);

        return node->val.second;
    }

    iterator erase(iterator pos) { 
        if (pos._ptr == nullptr) {
            return end();
        }
        
        HashNode* node = pos._ptr;
        size_t index = _bucket(node->val.first);

        HashNode* traversal_node = _buckets[index];
        HashNode* prev = traversal_node;

        while (traversal_node != node) {
            prev = traversal_node;
            traversal_node = traversal_node->next;
        }
        
        if (prev != traversal_node) {
            prev->next = node->next;
        }
        else {
            _buckets[index] = node->next;
        }

        if (_head == node) {
            for(size_t i = 0; i < _bucket_count; ++i) {
                if (_buckets[i] != nullptr) {
                    _head = _buckets[i];
                    break;
                }
                _head = nullptr;
            }
        }

        iterator next;

        if (node->next == nullptr) {
            ++pos;
            HashNode* available = pos._ptr;
            next = iterator(this,available);
        }
        else {
            next = iterator(this, node->next);
        }

        delete node;

        --_size;
        return next;
    }

    size_type erase(const Key & key) { 
        iterator pos = find(key);
        if (pos == end()) {
            return 0;
        }
        iterator returned = erase(pos);

        return 1;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
