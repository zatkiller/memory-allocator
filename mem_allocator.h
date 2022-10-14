#pragma once
#include <iostream>
#include <forward_list>
#include <vector>

using namespace std;

struct AllocatedHeader {
    size_t size;
};

struct FreeListNode {
    size_t size;
    char* start;

    FreeListNode() {}
    FreeListNode(size_t size, char *start) : size(size), start(start) {}
};

/**
 * Memory Allocator that does malloc and free
 *
 * Given a large pre-allocated memory block, the memory allocator must
 * - Allocate objects with various size inside given memory block
 * - Free said objects as user request
 * 
 * Uses First-fit as the memory allocation algorithm
 */
class Allocator {
    char *buf;
    size_t size;
    size_t cap;
    forward_list<FreeListNode> head;

public:
    Allocator(char *mem, size_t size) : buf(mem), cap(size) { head.emplace_front(cap, buf); }

    // find a continuous memory segment inside the given block that is free
    // [mem, mem + size)
    void* malloc(size_t size);
    
    // mark the continuous memory segment as free, so we can reuse it later
    void free(void* ptr);

    // For debugging purposes
    void print();

    // For testing purposes
    vector<pair<char*, size_t>> getFreeList();
};
