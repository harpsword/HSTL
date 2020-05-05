//
// Created by harpsword on 2020/5/3
//

#ifndef STL_SKIPLIST_H
#define STL_SKIPLIST_H

#include <stdlib.h>
#include <time.h>

#include "allocator.h"
#include "vector.h"
#include "functional.h"
#include "utility.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

namespace HSTL{
    template<typename T1>
    struct skiplistNode {
        T1 key_;
        vector<skiplistNode*> level_;
        skiplistNode()=default;
        skiplistNode(int initLevel): level_(initLevel, nullptr) {}
    };

    template<typename T>
    class skiplistIterator{
    private:
        using value_type = T;
        using iterator = skiplistNode<T>*;
        skiplistNode<T> *data;
    public:
        explicit skiplistIterator(iterator iter):data(iter) {}
        value_type & operator* (){
            return data->key_;
        }
        // ++operator
        // 修改了本身的数据
        skiplistIterator<T>& operator++(){
            data = data->level_[0];
            return *this;
        }
        // operator ++ (operator++++不支持)
        const skiplistIterator<T> operator++(int){
            return skiplistIterator(data->level_[0]);
//            return data->level_[0];
        }
        bool operator != (const skiplistIterator<T> & other){
            return data != other.data;
        }
        bool operator == (const skiplistIterator<T> &other){
            return data == other.data;
        }
    };

    template<typename T1, typename Compare=less<T1>>
    class skiplist{
    public:
        using size_type = std::size_t;
        using NodeType = skiplistNode<T1>;
        using dataAllocator = allocator<NodeType>;
        using iterator = skiplistIterator<T1>;
        using return_type = pair<iterator, bool>;

        static constexpr double kProb = 0.25;
        static constexpr int kMaxLevel = 32;

    private:
        NodeType *head_, *tail_;
        int level_;
        unsigned long length_;
        vector<NodeType *> search_index_;
        Compare compare;
    public:
        // function
        skiplist();
        ~skiplist();
        // tool function
        void releaseNode(NodeType *node){
            dataAllocator::destroy(node);
            dataAllocator::deallocate(node);
        }
        NodeType* initNode(){
            NodeType *tmp =dataAllocator::allocate(1);
            dataAllocator::construct(tmp);
            return tmp;
        }
        NodeType* initNode(int initLevel){
            NodeType *tmp =dataAllocator::allocate(1);
            new(tmp) NodeType(initLevel);
            return tmp;
        }
        int getRandomLevel();
        void output();
        // Iterator
        iterator begin() { return ++iterator(head_); }
        iterator end() { return iterator(tail_); }

        // Capacity
        bool empty() { return length_ == 0;}
        size_type size() { return length_; }

        // Modifiers
        iterator insert_aux(T1 key_value);
        // insert with repetition limit
        return_type insert_nomulti(T1 key_value);
        // insert allow repetition
        iterator insert_multi(T1 key_value);

//        void erase(iterator pos);
//        void erase(iterator first, iterator last);
        size_type erase(const T1 key_value);
        void clear();

        // Lookup
        bool search(T1 key_value);
        iterator find(T1 key_value);

    };

    template <typename T1, typename Compare>
    skiplist<T1, Compare>::skiplist():level_(0),
                                length_(0),
                                search_index_(kMaxLevel, nullptr){
        head_ = initNode(kMaxLevel);
        tail_ = initNode(kMaxLevel);
        head_->level_[0] = tail_;
        level_ = 1;
        length_ = 0;
        // init random
        srand((unsigned )time(nullptr));
    }

    template <typename T1, typename Compare>
    skiplist<T1, Compare>::~skiplist() {
        // delete all node
        NodeType *tmpNode;
        while (head_ != tail_){
            tmpNode = head_->level_[0];
            releaseNode(head_);
            head_ = tmpNode;
        }
        releaseNode(tail_);
    }

    template <typename T1, typename Compare>
    int skiplist<T1, Compare>::getRandomLevel() {
        int level = 1;
        while (rand()/double(RAND_MAX) < kProb && level < kMaxLevel){
            level ++;
        }
        return level;
    }

    template <typename T1, typename Compare>
    typename skiplist<T1, Compare>::iterator skiplist<T1, Compare>::insert_aux(T1 key_value) {
        // 当列表中有元素1, 2, 2, 2, 3
        // 插入 一个新的元素2时，会插入到1和第一个2之间
        int level = getRandomLevel();
        // create new node
        NodeType *new_node = initNode(level);
        new_node->key_ = key_value;

//        search(key_value);
        for (int i = 0; i < min(level, level_); i++){
            // insert node
            new_node->level_[i] = search_index_[i]->level_[i];
            search_index_[i]->level_[i] = new_node;
        }

        if (level > level_){
            for (int i = level_; i != level; i++){
                head_->level_[i] = new_node;
                new_node->level_[i] = tail_;
            }
            level_ = level;
        }
        length_++;
        return iterator(new_node);
    }

    template<typename T1, typename Compare>
    typename skiplist<T1, Compare>::return_type skiplist<T1, Compare>::insert_nomulti(T1 key_value) {
        bool already_have = search(key_value);
        if (! already_have) return return_type{insert_aux(key_value), true};
        else return return_type{end(), false};
    }

    template<typename T1, typename Compare>
    typename skiplist<T1, Compare>::iterator skiplist<T1, Compare>::insert_multi(T1 key_value) {
        bool _tmp = search(key_value);
        return insert_aux(key_value);
    }

    template <typename T1, typename Compare>
    bool skiplist<T1, Compare>::search(T1 key_value) {
        // 返回key_value是否存在
        // 若存在即 search_index_[0]->level_[0]
        int level_id = level_ - 1;
        NodeType *current_node = head_;
        while (level_id !=0){
            // search forward
            while (current_node->level_[level_id] != tail_ && compare(current_node->level_[level_id]->key_, key_value))
            {
                current_node = current_node->level_[level_id];
            }
            // move down
            while (level_id > 0 && \
                  (current_node->level_[level_id] == tail_ || \
                   !compare(current_node->level_[level_id]->key_, key_value))){
                search_index_[level_id] = current_node;
                level_id--;
            }
        }

        while (current_node->level_[level_id] != tail_ &&\
               compare(current_node->level_[level_id]->key_, key_value))
        {
            current_node = current_node->level_[level_id];
        }
        // record level 1's previous node
        search_index_[level_id] = current_node;
        if (current_node->level_[level_id] == tail_) return false;
        T1 next_value = current_node->level_[level_id]->key_;
        return !compare(key_value, next_value);
    }

    template<typename T1, typename Compare>
    typename skiplist<T1, Compare>::iterator skiplist<T1, Compare>::find(T1 key_value) {
        bool isExist = search(key_value);
        if (isExist) return iterator(search_index_[0])++;
        else return end();
    }

    template <typename T1, typename Compare>
    void skiplist<T1, Compare>::output() {
        NodeType *tmpNode;
        for (int i = 0; i != level_; i++){
            tmpNode = head_->level_[i];
            while (tmpNode != tail_){
                std::cout << tmpNode->key_ << " ";
                tmpNode = tmpNode->level_[i];
            }
            std::cout << std::endl;
        }
    }

    template<typename T1, typename Compare>
    void skiplist<T1, Compare>::clear() {
        NodeType *tmp = head_->level_[0];
        NodeType *next;
        while (tmp != tail_){
            next = tmp->level_[0];
            releaseNode(tmp);
            tmp = next;
        }
        head_->level_[0] = tail_;
        level_ = 1;
        length_ = 0;
    }

    template<typename T1, typename Compare>
    size_t skiplist<T1, Compare>::erase(const T1 key_value) {
        bool isExist = search(key_value);
        if (!isExist) return 0;
        else {
            size_t count{0};
            NodeType *current_node = search_index_[0]->level_[0];
            while (!compare(key_value, current_node->key_)){
                // 从current_node开始的节点是有序的，
                // 若 compare 来自 less
                // 节点顺序为从小到大（按照T1.first 对于map)
                // 当key_value小于current_node->key意味着已经删除完了该删除的节点
                for (int level_id = 0; level_id != level_; level_id++){
                    if (search_index_[level_id] != current_node) break;
                    search_index_[level_id] = current_node->level_[level_id];
                }
                NodeType *tmp = current_node->level_[0];
                releaseNode(current_node);
                current_node = tmp;
                count++;
            }
            return count;
        }
    }

}



#endif
