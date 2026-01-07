#include "../../include/memsim.h"

/* Configuration */

static const size_t ALIGNMENT = 8;

//  Global State
vector<Block> memory_blocks;
size_t TOTAL_MEMORY = 0;
AllocatorType alloc_type = FIRST_FIT;
int next_id = 1;

// Allocation metrics
size_t total_alloc_requests = 0;
size_t successful_allocs = 0;
size_t failed_allocs = 0;

// Fragmentation tracking
size_t total_requested_memory = 0;
size_t total_allocated_memory = 0;
size_t total_internal_fragmentation = 0;

//   Helper: Coalescing

static void coalesce(){
    for (size_t i = 0; i + 1 < memory_blocks.size();){
        if (memory_blocks[i].free && memory_blocks[i + 1].free){
            memory_blocks[i].size += memory_blocks[i + 1].size;
            memory_blocks.erase(memory_blocks.begin() + i + 1);
        }
        else{
            i++;
        }
    }
}

// Inititialization

void init_memory(size_t size){
    TOTAL_MEMORY = size;
    memory_blocks.clear();
    memory_blocks.push_back({0, size, 0, true, -1});
    next_id = 1;
    total_alloc_requests = 0;
    successful_allocs = 0;
    failed_allocs = 0;
    total_requested_memory = 0;
    total_allocated_memory = 0;
    total_internal_fragmentation = 0;
}

// Allocation

int allocate_block(size_t req)
{
    total_alloc_requests++;
    total_requested_memory += req;
    size_t aligned_req = ((req + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    int idx = -1;
    if (alloc_type == FIRST_FIT){
        for (size_t i = 0; i < memory_blocks.size(); i++){
            if (memory_blocks[i].free && memory_blocks[i].size >= aligned_req){
                idx = i;
                break;
            }
        }
    }
    else if (alloc_type == BEST_FIT){
        size_t best = SIZE_MAX;
        for (size_t i = 0; i < memory_blocks.size(); i++){
            if (memory_blocks[i].free && memory_blocks[i].size >= aligned_req && memory_blocks[i].size < best){
                best = memory_blocks[i].size;
                idx = i;
            }
        }
    }
    else{
        size_t worst = 0;
        for (size_t i = 0; i < memory_blocks.size(); i++)
        {
            if (memory_blocks[i].free && memory_blocks[i].size >= aligned_req && memory_blocks[i].size > worst){
                worst = memory_blocks[i].size;
                idx = i;
            }
        }
    }
    if(idx == -1){
        failed_allocs++;
        return -1;
    }
    Block old = memory_blocks[idx];
    if(old.size > aligned_req){
        memory_blocks[idx] = {old.start,aligned_req,req,false,next_id++};
        memory_blocks.insert(memory_blocks.begin()+idx+1,{old.start +aligned_req,old.size-aligned_req,0,true,-1});
    }
    else{
        memory_blocks[idx].free = false;
        memory_blocks[idx].id = next_id++;
        memory_blocks[idx].requested = req;
    }
    successful_allocs++;
    total_allocated_memory += aligned_req;
    total_internal_fragmentation += (aligned_req - req);
    return memory_blocks[idx].id;
}

// Deallocation

bool free_block(int id)
{
    for (auto &b : memory_blocks){
        if (!b.free && b.id == id){
            total_allocated_memory -= b.size;
            total_internal_fragmentation -= (b.size - b.requested);
            b.free = true;
            b.id = -1;
            b.requested = 0;
            coalesce();
            return true;
        }
    }
    return false;
}

//   Debug / Visualization

void dump_memory() {
    for (auto &b : memory_blocks) {
        size_t start = b.start;
        size_t end = b.start + b.size - 1;
        cout << "[" << start << " - " << end << "] "<< "(0x" << hex << start << " - 0x" << end << dec << ") ";
        if (b.free) cout << "FREE\n";
        else cout << "USED (id=" << b.id << ")\n";
    }
}


// Statistics

void print_stats(){
    size_t used = 0;
    size_t free_mem = 0;
    size_t max_free = 0;
    for (auto &b : memory_blocks){
        if (b.free){
            free_mem += b.size;
            max_free = max(max_free, b.size);
        }
        else{
            used += b.size;
        }
    }
    double ext_frag = 0.0;
    if (free_mem > 0) ext_frag = 100.0 * (double)(free_mem - max_free) / free_mem;

    double success_rate = 0.0;
    if (total_alloc_requests > 0) success_rate = (double)successful_allocs / total_alloc_requests * 100;

    double utilization = 0.0;
    if (TOTAL_MEMORY > 0) utilization = (double)used / TOTAL_MEMORY * 100;
    
    cout<<"Total memory: "<<TOTAL_MEMORY<<"\n";
    cout<<"Used memory: "<<used<<"\n";
    cout<<"Free memory: "<<free_mem<<"\n";
    cout<<"External fragmentation: "<<fixed<<setprecision(2)<<ext_frag<<"%\n";
    cout<<"Internal fragmentation: "<<total_internal_fragmentation<<"\n";
    cout<<"Allocation success rate: "<<fixed<<setprecision(2)<<success_rate<<"%\n";
    cout<<"Failed allocations: "<<failed_allocs<<"\n";
    cout<<"Memory utilization: "<<fixed<<setprecision(2)<<utilization<<"%\n";
}
