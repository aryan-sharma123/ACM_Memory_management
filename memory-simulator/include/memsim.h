#pragma once
#include <bits/stdc++.h>
using namespace std;

/*  Physical Memory (Dynamic Allocator) */

struct Block
{
    size_t start;    // starting address
    size_t size;    // allocated (aligned) size
    size_t requested;    // user requested size
    bool free;
    int id;
};
enum AllocatorType{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};
// Cachee
enum ReplacementPolicy{
    FIFO,
    LRU
};
struct CacheLine{
    bool valid = false;
    size_t tag = 0;
    // FIFO insertion and LRU last access
    size_t timestamp = 0;   
};
struct Cache{
    string name;
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    size_t num_sets;
    size_t time = 0;
    size_t accesses = 0;
    ReplacementPolicy policy = FIFO;
    vector<vector<CacheLine>> sets;
    size_t hits = 0;
    size_t misses = 0;
};

// Cachee interface

void init_cache(Cache &c,string name,size_t cache_size,size_t block_size,size_t associativity,ReplacementPolicy policy);
bool access_cache(Cache &c, size_t address);
void print_cache_stats(const Cache &c);

// VIRTUAL MEMORY
enum VMReplacement
{
    FIFO_VM,
    LRU_VM
};

void init_vm(size_t physical_memory_size, size_t page_size);
size_t translate_address(size_t virtual_addr);
void set_vm_policy(VMReplacement p);
void print_vm_stats();
void dump_page_table();

// ALLOCATION STATs
extern vector<Block> memory_blocks;
extern size_t TOTAL_MEMORY;
extern AllocatorType alloc_type;
extern int next_id;

// allocation metrics
extern size_t total_alloc_requests;
extern size_t successful_allocs;
extern size_t failed_allocs;

// fragmentation metrics
extern size_t total_requested_memory;
extern size_t total_allocated_memory;
extern size_t total_internal_fragmentation;

// Allocator interface
void init_memory(size_t size);
int allocate_block(size_t size);
bool free_block(int id);
void dump_memory();
void print_stats();

// BUDDY 
void init_buddy(size_t size);
int buddy_malloc(size_t req_size);
bool buddy_free(int id);
void dump_buddy();
void print_buddy_stats();
