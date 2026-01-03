#include "../../include/memsim.h"

// Initialize cache structure and validate configuration

void init_cache(Cache &c, string name,size_t cache_size,size_t block_size,size_t associativity,ReplacementPolicy policy){
    c.name = name;
    c.cache_size = cache_size;
    c.block_size = block_size;
    c.associativity = associativity;
    c.policy = policy;

    // reset stats
    c.time = c.hits = c.misses = c.accesses = 0;

    if (block_size == 0 || associativity == 0 || cache_size % (block_size * associativity) != 0){
        cerr << "Invalid cache configuration\n";
        exit(1);
    }
    size_t lines = cache_size / block_size;
    c.num_sets = lines / associativity;

    // allocate cache sets
    c.sets.assign(c.num_sets, vector<CacheLine>(associativity));
}
// When we will access this cachee with given address then it will Returns true on HIT, false on MISS

bool access_cache(Cache &c, size_t address){
    c.time++;
    c.accesses++;

// block address, set index, and tag
    size_t block = address / c.block_size;
    size_t set_id = block % c.num_sets;
    size_t tag = block / c.num_sets;

    auto &set = c.sets[set_id];
    for (auto &line : set)
    {
        if (line.valid && line.tag == tag)
        {
            c.hits++;
            if (c.policy == LRU)
                line.timestamp = c.time;
            return true;
        }
    }
    c.misses++;

    // place in empty line if available
    for (auto &line : set){
        if (!line.valid){
            line = {true, tag, c.time};
            return false;
        }
    }

    // replacement (FIFO / LRU based on timestamp)
    int victim = 0;
    for (int i = 1; i < (int)set.size(); i++)
        if (set[i].timestamp < set[victim].timestamp) victim = i;
    set[victim] = {true, tag, c.time};
    return false;
}

// Cachee Stats
void print_cache_stats(const Cache &c){
    size_t total = c.hits + c.misses;
    double hit_ratio = total ? (double)c.hits / total : 0.0;
    cout << c.name << " Cache\n";
    cout << "Hits: " << c.hits << "\n";
    cout << "Misses: " << c.misses << "\n";
    cout << "Hit Ratio: "<< fixed << setprecision(2)<< hit_ratio * 100 << "%\n";
}
