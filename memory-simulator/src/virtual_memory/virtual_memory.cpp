#include "../../include/memsim.h"

static size_t PAGE_SIZE = 0;
static size_t NUM_FRAMES = 0;
static size_t VIRTUAL_PAGES = 0;

// replacement policy (FIFO / LRU)
static VMReplacement vm_policy = FIFO_VM;

struct PageTableEntry
{
    bool valid = false;
    size_t frame = 0;
    size_t timestamp = 0;   // insertion time (FIFO) orr last access (LRU)
};

static vector<PageTableEntry> page_table;
static vector<int> frame_to_page;

static size_t vm_time = 0;
static size_t page_hits = 0;
static size_t page_faults = 0;

// Initialize

void init_vm(size_t physical_memory_size, size_t page_size)
{
    PAGE_SIZE = page_size;
    NUM_FRAMES = physical_memory_size / PAGE_SIZE;

    VIRTUAL_PAGES = NUM_FRAMES * 4;
    page_table.assign(VIRTUAL_PAGES, PageTableEntry());
    frame_to_page.assign(NUM_FRAMES, -1);
    vm_time = 0;
    page_hits = 0;
    page_faults = 0;
}

// Address Translation

size_t translate_address(size_t virtual_addr)
{
    vm_time++;
    size_t page = virtual_addr / PAGE_SIZE;
    size_t offset = virtual_addr % PAGE_SIZE;

    // bounds check
    if (page >= VIRTUAL_PAGES){
        cerr << "Invalid virtual address\n";
        return 0;
    }
    if (page_table[page].valid) // page hit if else block
    {
        page_hits++;
        if (vm_policy == LRU_VM)
            page_table[page].timestamp = vm_time;
        return page_table[page].frame * PAGE_SIZE + offset;
    }
    page_faults++; // page fault

    // find free frame
    size_t frame = NUM_FRAMES;
    for (size_t i = 0; i < NUM_FRAMES; i++){
        if (frame_to_page[i] == -1){
            frame = i;
            break;
        }
    }

    // no free frame → replace victim
    if (frame == NUM_FRAMES){
        size_t victim = 0;
        size_t oldest = SIZE_MAX;
        for (size_t p = 0; p < VIRTUAL_PAGES; p++){
            if (page_table[p].valid && page_table[p].timestamp < oldest){
                oldest = page_table[p].timestamp;
                victim = p;
            }
        }
        frame = page_table[victim].frame;
        page_table[victim].valid = false;
        frame_to_page[frame] = -1;
    }
    // load new page
    page_table[page].valid = true;
    page_table[page].frame = frame;
    page_table[page].timestamp = vm_time;
    frame_to_page[frame] = static_cast<int>(page);

    return frame * PAGE_SIZE + offset;
}


void set_vm_policy(VMReplacement p)
{
    vm_policy = p;
}

// Stats
void print_vm_stats()
{
    cout << "Page hits: " << page_hits << "\n";
    cout << "Page faults: " << page_faults << "\n";
}

void dump_page_table()
{
    for (size_t i = 0; i < VIRTUAL_PAGES; i++){
        if (page_table[i].valid){
            cout << "Page " << i<< " -> Frame " << page_table[i].frame << "\n";
        }
    }
}
