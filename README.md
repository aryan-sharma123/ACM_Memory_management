## Author - Aryan
## En.no - 23112022

### outputs :- Logs / SS have been stored in folder  - outputs (txt logs for buddy and alloc , rest in PNG format )


### [demo video link](https://drive.google.com/file/d/1MBbV6iVKyJeuompEs1fMbb5cJknr-hfX/view?usp=drive_link)


### design document is also uploaded


# Memory Management Simulator

## Overview
This project is a **user-space Memory Management Simulator** that models how an operating system manages memory.  
It simulates **dynamic memory allocation**, **multilevel CPU caches**, and an **optional virtual memory subsystem**, focusing on algorithmic correctness and clarity rather than kernel-level complexity.

The simulator is interactive and CLI-driven, allowing users to issue memory, cache, and virtual memory commands and observe system behavior.

---

## Project Structure
``` text
memory-simulator/
├─ src/
│ ├─ allocator/
│ │ └─ allocator.cpp
│ ├─ buddy/
│ │ └─ buddy.cpp
│ ├─ cache/
│ │ └─ cache.cpp
│ ├─ virtual_memory/
│ │ └─ virtual_memory.cpp
│ └─ main.cpp
├─ include/
│ └─ memsim.h
├─ tests/
│ └─ sample_inputs.txt
├─ docs/
│ └─ design_document.pdf
├─ Makefile
└─ README.md
```


## Implemented Features

###  Mandatory Features

#### 1. Physical Memory Simulation
- Simulates a **contiguous block of physical memory**
- Memory is dynamically split and merged based on allocation requests
- Memory blocks track:
  - Start address
  - Allocated size (aligned)
  - Requested size
  - Allocation status

#### 2. Dynamic Memory Allocation Strategies
Implemented and selectable at runtime:
- **First Fit**
- **Best Fit**
- **Worst Fit**

Each allocation:
- Finds a suitable free block
- Splits blocks if required
- Tracks allocated and free regions explicitly

Each deallocation:
- Frees the block
- **Coalesces adjacent free blocks** to reduce fragmentation

#### 3. Allocation Interface (CLI)
Supported commands:
- `init memory <size>`
- `set allocator first_fit | best_fit | worst_fit`
- `malloc <size>`
- `free <id>`
- `dump`
- `stats`

#### 4. Metrics and Statistics
Tracked and reported:
- Internal fragmentation
- External fragmentation
- Allocation success / failure count
- Memory utilization

---

###  Mandatory Feature: Multilevel Cache Simulation

#### Cache Hierarchy
- **L1, L2, L3 caches**
- Configurable:
  - Cache size
  - Block size
  - Associativity
- Set-associative cache structure

#### Cache Replacement Policies
- **FIFO** (mandatory)
- **LRU** (implemented using timestamps)

#### Cache Behavior
- Memory accesses flow as:
- L1 → L2 → L3 → Main Memory
- On each access:
- Hit or miss is detected per level
- Miss penalties propagate to lower levels
- Access time is accumulated

#### CLI Command
- `access <address>`
- Cache statistics printed using:
- `cache_stats`

---
- Memory dump output displays **decimal and hexadecimal addresses side-by-side** for improved readability and debugging clarity.


###  Optional Features Implemented

#### 1. Buddy Memory Allocation System
- Memory size restricted to powers of two
- Allocation rounds up to nearest power of two
- Recursive block splitting and buddy coalescing
- Tracks buddy-specific internal fragmentation

Commands:
- `buddy_malloc <size>`
- `buddy_free <id>`
- `dump` (in buddy mode)
- `stats` (in buddy mode)

#### 2. Virtual Memory Simulation (Paging)
- Virtual to physical address translation
- Single global page table (simplified model)
- Page replacement:
- FIFO
- LRU
- Page hit / fault tracking
- Cache accessed **after** address translation

Commands:
- `init_vm <physical_memory> <page_size>`
- `vm_access <virtual_address>`
- `vm_stats`
- `dump_vm`

> Note: Disk access and page movement are simulated representationally as per project guidelines.

---

## Design Choices & Assumptions

- **Vectors are used instead of linked lists** for memory block management for simplicity and cache friendliness
- Single-process memory model (no per-process page tables)
- Disk and write-back cache policies are representational and not explicitly simulated
- Memory alignment handled explicitly (8-byte alignment)
- Focus is on correctness and clarity rather than hardware-accurate timing

---

## Build & Run Instructions
if you are on windows make sure you should have access of UBUNTU ( WSL )
-open the terminal/bash 
-go to the directory of your memory-management folder (using cd /mnt/folder_path ) and start building 

```bash
make
./memsim
```
### Example Usage
```text
> init memory 1024
> set allocator best_fit
> malloc 100
> malloc 200
> free 1
> dump
> stats

> access 128
> access 128
> cache_stats

> init_vm 512 16
> vm_access 64
> vm_stats
```

### Further Improvements

- Per-process virtual address spaces
- TLB simulation
- Multi-threaded access simulation



### Notes

- All mandatory features are fully implemented
- Optional features are clearly separated and documented
- Sample test inputs and output logs are provided in the tests/ directory

