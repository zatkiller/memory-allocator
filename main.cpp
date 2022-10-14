#include <cassert>

#include "mem_allocator.h"

using namespace std;

int main() {

    size_t sz1 = 512;
    char buf1[sz1];
    Allocator alloc(buf1, sz1);

    char* m1 = (char*) alloc.malloc(28);
    vector<pair<char*, size_t>> test1 = {{buf1 + 36, sz1-36}};
    assert(alloc.getFreeList() == test1);

    char* m2 = (char*) alloc.malloc(100);
    vector<pair<char*, size_t>> test2 = {{buf1+36+108, sz1-36-108}};
    assert(alloc.getFreeList() == test2);

    char* m3 = (char*) alloc.malloc(100);
    vector<pair<char*, size_t>> test3 = {{buf1+36+108+108, sz1-36-108-108}};
    assert(alloc.getFreeList() == test3);
    
    alloc.free(m2);
    vector<pair<char*, size_t>> test4 = {{buf1+36, 108}, {buf1+36+108+108, sz1-36-108-108}};
    assert(alloc.getFreeList() == test4);;
    
    char* m4 = (char*) alloc.malloc(45);
    vector<pair<char*, size_t>> test5 = {{buf1+36+53, 108-53}, {buf1+36+108+108, sz1-36-108-108}};
    assert(alloc.getFreeList() == test5);

    
    char* m5 = (char*) alloc.malloc(180);
    vector<pair<char*, size_t>> test6 = {{buf1+36+53, 108-53}, {buf1+36+108+108+188, sz1-36-108-108-188}};
    assert(alloc.getFreeList() == test6);

    alloc.free(m1);
    vector<pair<char*, size_t>> test7 = {{buf1, 36}, {buf1+36+53, 108-53}, {buf1+36+108+108+188, sz1-36-108-108-188}};
    assert(alloc.getFreeList() == test7);

    alloc.free(m3);
    vector<pair<char*, size_t>> test8 = {{buf1, 36}, {buf1+36+53, 163}, {buf1+36+108+108+188, sz1-36-108-108-188}};
    assert(alloc.getFreeList() == test8);


    alloc.free(m4);
    vector<pair<char*, size_t>> test9 = {{buf1, 252}, {buf1+36+108+108+188, sz1-36-108-108-188}};
    assert(alloc.getFreeList() == test9);

    alloc.free(m5);
    vector<pair<char*, size_t>> test10 = {{buf1, sz1}};
    assert(alloc.getFreeList() == test10);


    return 0;
}