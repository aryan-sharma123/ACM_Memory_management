#include "../include/memsim.h"

// ACCESS Time Stats

size_t total_memory_accesses = 0;
size_t total_access_time = 0;
size_t l1_miss_to_l2 = 0;
size_t l2_miss_to_l3 = 0;
size_t l3_miss_to_memory = 0;

static const size_t L1_LATENCY = 1;
static const size_t L2_LATENCY = 5;
static const size_t L3_LATENCY = 15;
static const size_t MEMORY_LATENCY = 100;

// CAchees
Cache L1, L2, L3;

// ALLOCATION
enum AllocMode{
    NORMAL,
    BUDDY
};

static AllocMode alloc_mode = NORMAL;
int main(){
    cout << "Memory Simulator\n";
    init_cache(L1, "L1", 64, 16, 1, LRU);
    init_cache(L2, "L2", 128, 16, 2, FIFO);
    init_cache(L3, "L3", 256, 32, 4, FIFO);

    string cmd;
    // int andi_bandi_s = 1;
    while (true)
    {
        cout << "> ";
        if (!(cin >> cmd))
            break;

        if (cmd == "init"){
            string what;
            size_t size;
            cin >> what >> size;
            if (what == "memory")
            {
                init_memory(size);
                alloc_mode = NORMAL;
            }
        }
        else if (cmd == "set")
        {
            string what, value;
            cin >> what >> value;
            if (what == "allocator")
            {
                if (value == "first_fit") alloc_type = FIRST_FIT;
                else if (value == "best_fit") alloc_type = BEST_FIT;
                else if (value == "worst_fit") alloc_type = WORST_FIT;
            }
            else if (what == "vm_policy")
            {
                if (value == "fifo") set_vm_policy(FIFO_VM);
                else if (value == "lru") set_vm_policy(LRU_VM);
            }
        }
        else if (cmd == "malloc")
        {
            size_t size;
            cin >> size;
            int id;
            if (alloc_mode == BUDDY) id = buddy_malloc(size);
            else id = allocate_block(size);
            if (id == -1) cout << "Allocation failed\n";               
            else cout << "Allocated id=" << id << "\n";                
        }
        else if (cmd == "free")
        {
            int id;
            cin >> id;
            bool ok;
            if (alloc_mode == BUDDY) ok = buddy_free(id);
            else ok = free_block(id);
            if (ok) cout << "Freed\n";
            else cout << "Invalid id\n";
        }
        else if (cmd == "dump")
        {
            if (alloc_mode == BUDDY) dump_buddy();
            else dump_memory();
        }
        else if (cmd == "stats")
        {
            if (alloc_mode == BUDDY) print_buddy_stats();
            else print_stats();
        }

//    Cachee
        else if (cmd == "access")
        {
            size_t addr;
            cin >> addr;
            size_t access_time = 0;
            total_memory_accesses++;

            bool l1_hit = access_cache(L1, addr);

            if (l1_hit)
            {
                cout << "L1 HIT\n";
                access_time += L1_LATENCY;
            }
            else
            {
                l1_miss_to_l2++;
                access_time += L1_LATENCY;

                bool l2_hit = access_cache(L2, addr);
                if (l2_hit)
                {
                    cout << "L1 MISS -> L2 HIT\n";
                    access_time += L2_LATENCY;
                }
                else
                {
                    l2_miss_to_l3++;
                    access_time += L2_LATENCY;

                    bool l3_hit = access_cache(L3, addr);
                    if (l3_hit)
                    {
                        cout << "L1 MISS -> L2 MISS -> L3 HIT\n";
                        access_time += L3_LATENCY;
                    }
                    else
                    {
                        l3_miss_to_memory++;
                        cout << "L1 MISS -> L2 MISS -> L3 MISS -> MEMORY\n";
                        access_time += L3_LATENCY;
                        access_time += MEMORY_LATENCY;
                    }
                }
            }

            total_access_time += access_time;
        }

//  Virtual Memory 
        else if (cmd == "init_vm"){
            size_t phys, page;
            cin >> phys >> page;
            init_vm(phys, page);
        }
        else if (cmd == "vm_access")
        {
            size_t vaddr;
            cin >> vaddr;
            size_t paddr = translate_address(vaddr);
            size_t access_time = 0;
            total_memory_accesses++;
            bool l1_hit = access_cache(L1, paddr);
            if (l1_hit)
            {
                cout << "L1 HIT\n";
                access_time += L1_LATENCY;
            }
            else
            {
                l1_miss_to_l2++;
                access_time += L1_LATENCY;
                bool l2_hit = access_cache(L2, paddr);
                if (l2_hit)
                {
                    cout << "L1 MISS -> L2 HIT\n";
                    access_time += L2_LATENCY;
                }
                else
                {
                    l2_miss_to_l3++;
                    access_time += L2_LATENCY;
                    bool l3_hit = access_cache(L3, paddr);
                    if (l3_hit)
                    {
                        cout << "L1 MISS -> L2 MISS -> L3 HIT\n";
                        access_time += L3_LATENCY;
                    }
                    else
                    {
                        l3_miss_to_memory++;
                        cout << "L1 MISS -> L2 MISS -> L3 MISS -> MEMORY\n";
                        access_time += L3_LATENCY;
                        access_time += MEMORY_LATENCY;
                    }
                }
            }
            total_access_time += access_time;
        }
        else if (cmd == "vm_stats"){ print_vm_stats();}
        else if (cmd == "dump_vm") { dump_page_table();}
        else if (cmd == "exit"){  break;}
        andi_bandi_s--;
    }

    return 0;
}
