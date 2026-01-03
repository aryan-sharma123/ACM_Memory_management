# Memory Management Simulator

## Overview

This project is a **Memory Management Simulator** implemented in C++ that models how an operating system manages memory at multiple levels. It simulates **dynamic memory allocation**, **multilevel CPU caches**, and **virtual memory using paging**, through a command-line interface.

The goal of this project is **not** to build a real OS kernel, but to **accurately simulate OS-level memory management behavior** using well-defined data structures and algorithms.

---

## Features Implemented

### 1. Physical Memory Allocation
- Simulates a contiguous block of physical memory
- Dynamic allocation and deallocation
- Explicit tracking of allocated and free blocks
- Automatic coalescing of adjacent free blocks

---

### 2. Allocation Strategies
- **First Fit**
- **Best Fit**
- **Worst Fit**

Each allocation:
- Splits blocks when required
- Tracks internal fragmentation

---

### 3. Memory Metrics & Statistics
The simulator reports:
- Total memory
- Used memory
- Free memory
- Internal fragmentation
- External fragmentation
- Allocation success rate
- Failed allocations
- Memory utilization

---

### 4. Buddy Memory Allocation (Optional Extension)
- Memory size must be a power of two
- Allocation rounds up to nearest power of two
- Recursive splitting and buddy coalescing (XOR-based)
- Internal fragmentation tracking

---

### 5. Multilevel Cache Simulation (MUST HAVE)
- **L1, L2, and L3 caches**
- Configurable:
  - Cache size
  - Block size
  - Associativity
- Replacement policies:
  - FIFO
  - LRU
- Tag, index, and offset based addressing
- Miss penalty propagation across cache levels
- Cache hit/miss statistics
- Reports the level (L1 / L2 / L3 / Memory) serving each access

---

### 6. Virtual Memory Simulation (Optional Extension)
- Paging-based virtual memory model
- Address translation flow:



memory-simulator/
├─ src/
│  ├─ allocator/
│  │  └─ allocator.cpp
│  ├─ buddy/
│  │  └─ buddy.cpp
│  ├─ cache/
│  │  └─ cache.cpp
│  ├─ virtual_memory/
│  │  └─ virtual_memory.cpp
│  └─ main.cpp
├─ include/
│  └─ memsim.h
├─ tests/
│  └─ sample_inputs.txt
├─ docs/
│  └─ design_document.pdf
├─ Makefile
└─ README.md
