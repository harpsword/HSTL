//
// Created by harpsword on 2020/5/4.
//

#ifndef STL_MAP_H
#define STL_MAP_H

#include "allocator.h"
#include "skiplist.h"
#include "functional.h"
#include "utility.h"

namespace HSTL{

    template<typename Key, typename T, typename Compare>
    struct keyCompare{
    private:
        Compare compare;
    public:
        using value_type = pair<Key, T>;
        bool operator() (value_type &lhs, value_type &rhs){
            return compare(lhs.first, rhs.first);
        }
    };

    template<
        typename Key,
        typename T,
        typename Compare=less<Key>>
    class map{
    public:
        using key_value = Key;
        using mapped_type = T;
        using value_type = pair<Key, T>;
        using size_type = std::size_t;
        using dfference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = skiplistIterator<value_type>;
        using const_iterator = const iterator;

        using return_type = pair<iterator, bool>;

        using trueCompare = keyCompare<Key, T, Compare>;

    private:
        skiplist<value_type , trueCompare> data_;
    public:
        // constructor(normal, copy, copy rvalue)
        map() = default;
        // destructor
        ~map() = default;
        // copy assignment, rvalue copy assignment

        // Element Access
        T& at(const Key& key);
        T& operator[] (const Key& key);

        // Iterators
        iterator begin() { return data_.begin(); }
        iterator end() { return data_.end(); }

        // Capacity
        bool empty() { return data_.empty(); }
        size_type size() { return data_.size(); }

        // Modifiers
        void clear();
        void insert(value_type value);
        void erase(const_iterator pos);
        void erase(value_type value);

        // Lookup
        iterator find(const Key& key);
        size_type count( const Key& key ) const;
    };

    template<typename Key, typename T, typename Compare>
    T& map<Key, T, Compare>::at(const Key &key) {
        value_type tmp{key, T()};
        iterator iter = data_.find(tmp);
        if (iter == end()) {
            throw out_of_range("not exist key!" );
        }else return (*iter).second;
    }

    template<typename Key, typename T, typename Compare>
    T& map<Key, T, Compare>::operator[] (const Key& key){
        value_type tmp{key, T()};
        iterator iter = data_.find(tmp);
        if (iter == end()){
            iter = data_.insert_aux(tmp);
            return (*iter).second;
        }else return (*iter).second;
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::clear() {
        data_.clear();
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::erase(const_iterator pos) {
        erase((*pos));
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::erase(value_type value) {
        data_.erase(value);
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::insert(value_type value) {
        data_.insert_nomulti(value);
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::iterator map<Key, T, Compare>::find(const Key &key) {
        return data_.find(value_type{key, T()});
    }

    template<typename Key, typename T, typename Compare>
    size_t map<Key, T, Compare>::count(const Key &key) const {
        if (data_.search(value_type{key, T()})){
            return 1;
        } else return 0;
    }

}

#endif //STL_MAP_H
