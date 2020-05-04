//
// Created by harpsword on 2020/5/3
//

#ifndef STL_SKIPLIST_H
#define STL_SKIPLIST_H

#include <stdlib.h>
#include <time.h>

#include "allocator.h"
#include "vector.h"

namespace HSTL{
    template<typename T1, typename T2>
    struct skiplistNode {
        T1 key_;
        T2 value_;
        vector<skiplistNode*> level_;
        skiplistNode(): key_(T1()), value_(T2()) {}
        skiplistNode(int initLevel): key_(T1()), value_(T2()), level_(initLevel, nullptr) {}
    };

    template<typename T1, typename T2=int>
    class skiplist{
    public:
    private:
        using NodeType = skiplistNode<T1, T2>;
        using dataAllocator = allocator<NodeType>;
        static constexpr double kProb = 0.25;
        static constexpr int kMaxLevel = 32;

        NodeType *head_, *tail_;
        int level_;
        unsigned long length_;
        vector<NodeType *> search_index_;
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
        void insertKey(T1 key_value);
        void insertKeyValue(T1 key_value, T2 value_value);
        void search(T1 key_value);

        void output();
    };

    template <typename T1, typename T2>
    skiplist<T1, T2>::skiplist():level_(0),
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

    template<typename T1, typename T2>
    skiplist<T1, T2>::~skiplist() {
        // delete all node
        NodeType *tmpNode;
        while (head_ != tail_){
            tmpNode = head_->level_[0];
            releaseNode(head_);
            head_ = tmpNode;
        }
        releaseNode(tail_);
    }

    template<typename T1, typename T2>
    int skiplist<T1, T2>::getRandomLevel() {
        int level = 1;
        while (rand()/double(RAND_MAX) < kProb && level < kMaxLevel){
            level ++;
        }
//        int value[10]= {1,2,2,1,1,2,2,2,2,2};
//        return value[length_];
        return level;
    }

    template<typename T1, typename T2>
    void skiplist<T1, T2>::insertKey(T1 key_value) {
        insertKeyValue(key_value, T2());
    }

    template<typename T1, typename T2>
    void skiplist<T1, T2>::insertKeyValue(T1 key_value, T2 value_value) {
        int level = getRandomLevel();
        std::cout << "get random level:" << level << std::endl;
        // create new node
        NodeType *new_node = initNode(level);
        new_node->key_ = key_value;
        new_node->value_ = value_value;

        search(key_value);
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
    }

    template<typename T1, typename T2>
    void skiplist<T1, T2>::search(T1 key_value) {
        int level_id = level_ - 1;
        NodeType *current_node = head_;
        while (level_id !=0){
            // search forward
            while (current_node->level_[level_id] != tail_ && current_node->level_[level_id]->key_ < key_value)
            {
                current_node = current_node->level_[level_id];
            }
            // move down
            while (level_id > 0 && (current_node->level_[level_id] == tail_ || current_node->level_[level_id]->key_ >= key_value)){
                search_index_[level_id] = current_node;
                level_id--;
            }
        }
        while (current_node->level_[level_id] != tail_ && current_node->level_[level_id]->key_ < key_value)
        {
            current_node = current_node->level_[level_id];
        }
        // record level 1's previous node
        search_index_[level_id] = current_node;
    }

    template<typename T1, typename T2>
    void skiplist<T1, T2>::output() {
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


}



#endif
