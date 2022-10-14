#include "mem_allocator.h"

using namespace std;

// find a continuous memory segment inside the given block that is free
// [mem, mem + size)
void* Allocator::malloc(size_t size) {
    forward_list<FreeListNode>::iterator prev = head.before_begin(), curr = head.begin();
    size_t totalSize = size + sizeof(AllocatedHeader);
    while (curr != head.end()) {
        if (curr->size >= totalSize) {

            AllocatedHeader* h = new (curr->start) AllocatedHeader();
            h->size = totalSize;
            char *mem = curr->start + sizeof(AllocatedHeader);
            size_t newSize = curr->size - totalSize;
            char* newStart = curr->start + totalSize;
            if (prev != head.before_begin()) {
                head.erase_after(prev);
                head.emplace_after(prev, newSize, newStart);
            } else {
                // If first free list node can be assigned
                head.pop_front();
                head.emplace_front(newSize, newStart);
            }

            return (void*) mem;
        } else{
            prev = curr;
            curr = next(curr);
        }
    }

    return (void*) nullptr;
}

// mark the continuous memory segment as free, so we can reuse it later
void Allocator::free(void* ptr) {
    char* s = (char *) ptr;
    char* h = s - sizeof(AllocatedHeader);
    AllocatedHeader* headerP = (AllocatedHeader*) h;
    // cout << "Free: " << headerP->size << " " << h-buf << " " << s-buf << endl;

    forward_list<FreeListNode>::iterator prev = head.before_begin(), curr = head.begin();
    while (curr != head.end()) {
        if (curr->start > h) {
            // cout << h - buf << endl;
            break;
        }

        prev = curr;
        curr = next(curr);
    }
    
    bool mergedToPrev = false, mergedToCurr = false;
    
    // If end address of previous free list node aligns with
    // Current start memory address to release, we merge the 2 nodes
    if (prev != head.before_begin()) {
        char* end = prev->start + prev->size;
        if (end == h) {
            prev->size += headerP->size;
            mergedToPrev = true;
        }
    }

    // If end address of memory block to release, aligns with the start address of
    // some free list node, we merge
    if (h + headerP->size == curr->start) {
        if (mergedToPrev) {
            prev->size += curr->size;
            head.erase_after(prev);
        } else {
            curr->start = h;
            curr->size += headerP->size;
        }
        mergedToCurr = true;
    }

    // If no merging occurs, we insert a new FreeList Node
    if (!mergedToCurr && !mergedToPrev) {           
        if (prev != head.before_begin()) {
            head.emplace_after(prev, headerP->size, h);
        } else {
            head.emplace_front(headerP->size, h);
        }
    }

    headerP->~AllocatedHeader();

    return;
}

// For debugging purposes
void Allocator::print() {
    auto it = head.begin();
    while (it != head.end()) {
        cout << "Start: " << it->start - buf << endl;
        cout << "Size: " << it->size << endl;
        it = next(it);
    }
}

// For testing purposes
vector<pair<char*, size_t>> Allocator::getFreeList() {
    vector<pair<char*, size_t>> res;
    auto it = head.begin();
    while (it != head.end()) {
        res.emplace_back(it->start, it->size);
        it = next(it);
    }
    return res;
}

