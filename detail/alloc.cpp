//
// Created by harpsword on 2020/4/20.
//

#include "../include/alloc.h"

namespace HSTL{
    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;
    size_t alloc::record_value = 0;

    alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    void * alloc::allocate(size_t bytes) {
        if (bytes > EMaxBytes::MAXBYTES) return malloc(bytes);
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        // 为什么 判断 list是否有空间可以使用直接判断 list 就可以？
        // 可以尝试一下 list != nullptr, 结果表明可以使用
        // std::cout << (list == nullptr) << std::endl;
        // output is 1, which means list is equal to nullptr
        if (list){
            // 有足够的空间可以分配
            free_list[index] = list->next;
            return list;
        }else {
            // 没有剩余的空间了，重新分配
            return refill(ROUND_UP(bytes));
        }
    }

    void alloc::deallocate(void *ptr, size_t bytes){
        if (bytes > EMaxBytes::MAXBYTES){
            free(ptr);
        }
        else{
            size_t index = FREELIST_INDEX(bytes);
            obj *node = static_cast<obj *>(ptr);
            node->next = free_list[index];
            free_list[index] = node;
        }
    }

    void *alloc::refill(size_t bytes) {
        size_t nobjs = ENObjs::NOBJS;
        char *chunk = chunk_alloc(bytes, nobjs);

        if (nobjs == 1) return chunk;
        else {
            obj **list = free_list + FREELIST_INDEX(bytes);
            *list = (obj *)(chunk + bytes);
            obj *current_obj = *list;
            while (nobjs > 2){
                current_obj->next = (obj *)((char *)current_obj + bytes);
                current_obj = current_obj->next;
                nobjs--;
            }
            current_obj->next = nullptr;
            return (obj *)(chunk);
        }
    }

    //假设bytes已经上调为8的倍数
    char *alloc::chunk_alloc(size_t bytes, size_t& nobjs){
        char *result = nullptr;
        size_t total_bytes = bytes * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= total_bytes){// 内存池满足要求
            result = start_free;
            start_free = start_free + total_bytes;
            return result;
        }
        else if (bytes_left >= bytes){// 内存池至少能提供一个及以上的区块
            nobjs = bytes_left / bytes;
            total_bytes = bytes * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else {// 内存池的大小已经小于 bytes了
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
            if (bytes_left > 0){
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->next = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            start_free = (char *)malloc(bytes_to_get);
            if (start_free == nullptr){// 申请失败
                obj **my_free_list = nullptr, *p = nullptr;
                for (size_t i = bytes; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN){
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != nullptr){
                        *my_free_list = p->next;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return chunk_alloc(bytes, nobjs);
                    }
                }
                end_free = 0;
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            // 当一开始free_list中没有元素，并且start_free申请失败的时候
            // 会运行到这里，但是end_free被修改了，引起bug
            if (start_free == nullptr) end_free = nullptr;
            return chunk_alloc(bytes, nobjs);
        }
    }
}