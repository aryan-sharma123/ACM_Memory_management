# Design and Implementation Document  
## Memory Management Simulator

---

## 1. Design Iterations and Efficiency Improvements (Summary)

During the development of this simulator, multiple design approaches were explored.  
Some initial approaches were **functionally correct**, but while implementing them, they were found to be **more complex, harder to maintain, or inefficient** for a simulator of this scope.

Instead of sticking rigidly to the first design, the implementation was **iteratively refined** based on practical observations during coding and testing. The objective was to improve **efficiency, clarity, and maintainability**, while still accurately modeling operating system behavior.

The following key design improvements were made:

- Memory blocks were initially planned to be managed using linked lists. This approach complicated traversal and block merging. The design was changed to use a `std::vector`, simplifying splitting, coalescing, and improving cache locality.
- Cache lines initially stored only valid bits and tags. A timestamp field was added to efficiently support FIFO and LRU replacement using a single mechanism.
- Cache replacement logic was unified to avoid separate implementations for each policy, reducing code complexity.
- Virtual memory was implemented using a single global page table instead of per-process tables to reduce overhead while preserving correct paging behavior.
- Disk access and page movement were modeled representationally to avoid unnecessary over-simulation.

These refinements resulted in a cleaner, more efficient, and easier-to-maintain design.

> **Note on Address Representation:**  
> During debugging and evaluation, memory layouts were initially printed only in decimal form.  
> To improve clarity and make address interpretation easier, the memory dump output was enhanced to display **decimal and hexadecimal addresses side-by-side**.  
> This change improves readability without affecting allocator logic or behavior.


---

## 2. Introduction

This document explains the **design, implementation approach, and reasoning** behind the Memory Management Simulator.

The project simulates core operating system concepts such as:
- Dynamic memory allocation
- Multilevel CPU cache hierarchy
- Optional virtual memory using paging

The focus is on **correct behavior and clean system design**, not kernel-level implementation.

---

## 3. High-Level Architecture

The simulator is organized into independent modules:

- **Allocator Module** – Dynamic memory allocation strategies
- **Buddy Allocator Module** – Optional power-of-two allocation
- **Cache Module** – Multilevel cache simulation (L1, L2, L3)
- **Virtual Memory Module** – Optional paging system
- **Main Module** – CLI interface and subsystem integration

Shared structures and interfaces are defined in `memsim.h`.

---

## 4. Physical Memory Model

### 4.1 Memory Representation

- Physical memory is modeled as a **single contiguous block**
- Memory addresses are offsets starting from `0`
- Memory is divided dynamically into blocks during execution

Each memory block stores:
- Start address
- Allocated (aligned) size
- Requested size
- Free/used status
- Allocation ID

For visualization purposes, memory dumps display block address ranges in **both decimal and hexadecimal formats**, allowing easier cross-verification with low-level address calculations.


---

### 4.2 Data Structure Choice

A linked list was initially considered for storing memory blocks.

During implementation, this approach made block splitting and merging more complex without providing clear benefits.

The final design uses a `std::vector<Block>`:
- Simpler sequential traversal
- Easier coalescing of free blocks
- Better cache locality
- Clearer memory visualization

This change improved both **efficiency and readability**.

---

## 5. Dynamic Memory Allocation Strategies

The simulator implements the following mandatory strategies:

### 5.1 First Fit
- Selects the first free block large enough to satisfy the request

### 5.2 Best Fit
- Selects the smallest free block that satisfies the request

### 5.3 Worst Fit
- Selects the largest available free block

---

### 5.4 Allocation Flow

1. Align requested size to 8 bytes  
2. Select a free block based on the chosen strategy  
3. Split the block if it is larger than required  
4. Update allocation metadata  
5. Update fragmentation statistics  

---

### 5.5 Deallocation and Coalescing

- Freed blocks are immediately merged with adjacent free blocks
- This reduces external fragmentation and keeps memory layout compact

---

## 6. Fragmentation Metrics

- **Internal fragmentation** is calculated as:   aligned_size − requested_size

- **External fragmentation** is calculated as:    (total_free_memory − largest_free_block) / total_free_memory
- Allocation success rate and memory utilization are also tracked

All metrics are reported using the `stats` command.

---

## 7. Multilevel Cache Simulation (Mandatory)

### 7.1 Cache Hierarchy

- Three-level cache: L1, L2, L3
- Configurable cache size, block size, and associativity
- Set-associative cache design

---

### 7.2 Cache Address Mapping

For a given physical address:
- Block address = address / block_size
- Set index = block_address % number_of_sets
- Tag = block_address / number_of_sets

---

### 7.3 Cache Line Metadata

Initially, cache lines stored only valid bits and tags.

To improve efficiency and simplify replacement logic, a timestamp field was added:
- FIFO uses insertion time
- LRU uses last access time

This unified approach reduced code duplication.

---

### 7.4 Replacement Policies

- **FIFO (mandatory)** – Evicts the oldest inserted cache line
- **LRU (optional)** – Evicts the least recently accessed cache line

---

### 7.5 Cache Access Flow and Miss Propagation

For each `access <address>` command:  CPU → L1 → L2 → L3 → Main Memory


- A hit at any level stops further lookup
- Misses propagate to lower levels
- Miss penalties are accumulated
- Hits and misses are tracked per cache level

---

## 8. Virtual Memory Simulation (Optional)

### 8.1 Virtual Memory Model

- Implemented using paging
- Single global page table for simplicity
- Virtual address divided into page number and offset

---

### 8.2 Page Table Entry

Each page table entry stores:
- Valid bit
- Frame number
- Timestamp (for FIFO / LRU)

---

### 8.3 Page Replacement

Supported policies:
- FIFO
- LRU

On a page fault:
1. A free frame is used if available
2. Otherwise, a victim page is selected
3. Victim page is evicted representationally
4. New page is loaded

---

### 8.4 Integration with Cache

Access order:
```text
Virtual Address
→ Page Table
→ Physical Address
→ Cache Hierarchy
→ Memory
```

Paging and caching logic are kept separate.

---

## 9. Buddy Allocation System (Optional)

- Memory size must be a power of two
- Allocation size is rounded to the nearest power of two
- Blocks are split and merged using the buddy system

Internal fragmentation specific to the buddy allocator is tracked.

---

## 10. CLI Integration

The `main.cpp` module:
- Parses user commands
- Routes requests to the correct subsystem
- Integrates allocator, cache, and virtual memory components
- Displays immediate feedback

---

## 11. Simplifications

Future Improvements :-
- Per-process page tables
- TLB simulation
- Multi-threaded access




Design choices were refined during development to improve **efficiency, simplicity, and maintainability**, resulting in a clean and robust memory management simulator.



