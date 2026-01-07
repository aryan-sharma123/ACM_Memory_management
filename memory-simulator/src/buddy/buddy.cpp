#include "../../include/memsim.h"

//  Buddy Allocator State
static size_t BUDDY_MEMORY_SIZE = 0;

static unordered_map<size_t, set<size_t>> free_lists; // free lists: block size -> set of starting addresses

// allocation table: id -> (address, block size, requested size)
static unordered_map<int, tuple<size_t, size_t, size_t>> allocated;

static size_t buddy_internal_frag = 0;  // internal fragmentation tracking
static int buddy_next_id = 1;

static bool is_power_of_two(size_t x){
    return x && !(x & (x - 1));
}

static size_t next_power_of_two(size_t x){
    size_t p = 1;
    while (p < x)
        p <<= 1;
    return p;
}

void init_buddy(size_t size){
    if (!is_power_of_two(size)){
        cout << "Buddy memory size must be power of two\n";
        return;
    }
    BUDDY_MEMORY_SIZE = size;
    free_lists.clear();
    allocated.clear();
    buddy_internal_frag = 0;
    buddy_next_id = 1;

    // entire memory starts as one free block
    free_lists[size].insert(0);
}

// Allocation

int buddy_malloc(size_t req_size){
    size_t block_size = next_power_of_two(req_size);

    size_t cur = block_size;  // find smallest available block >= block_size
    while (cur <= BUDDY_MEMORY_SIZE && free_lists[cur].empty()) cur <<= 1;

    if (cur > BUDDY_MEMORY_SIZE) return -1;

    while (cur > block_size){   // split blocks until desired size is reached
        size_t addr = *free_lists[cur].begin();
        free_lists[cur].erase(addr);
        cur >>= 1;
        free_lists[cur].insert(addr);
        free_lists[cur].insert(addr + cur);
    }

    // allocate block
    size_t addr = *free_lists[block_size].begin();
    free_lists[block_size].erase(addr);
    int id = buddy_next_id++;
    allocated[id] = make_tuple(addr, block_size, req_size);
    buddy_internal_frag += (block_size - req_size);
    return id;
}
// Deallocation

bool buddy_free(int id){
    auto it = allocated.find(id);
    if (it == allocated.end()) return false;

    size_t addr = get<0>(it->second);
    size_t block_size = get<1>(it->second);
    size_t req_size = get<2>(it->second);
    allocated.erase(it);
    buddy_internal_frag -= (block_size - req_size);

    
    while (true) // coalesce with buddy blocks if possible
    {
        size_t buddy = addr ^ block_size;
        if (free_lists[block_size].count(buddy)){
            free_lists[block_size].erase(buddy);
            addr = min(addr, buddy);
            block_size <<= 1;
        }
        else{
            free_lists[block_size].insert(addr);
            break;
        }
    }
    return true;
}

// Stats

void print_buddy_stats(){
    cout << "Buddy internal fragmentation: "<< buddy_internal_frag << "\n";
}
void dump_buddy(){
    cout << "Buddy Free Lists:\n";
    for (auto &p : free_lists){
        cout << "Block size " << p.first << ": ";
        for (auto addr : p.second)
            cout << "[" << addr << "] ";
        cout << "\n";
    }
}
