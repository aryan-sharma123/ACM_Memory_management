Markdown# Memory Management Simulator

A comprehensive C++ simulation of operating system memory management, covering physical memory allocation, multi-level cache hierarchies, and virtual memory paging.

---

## 📌 Overview
This project models how an operating system manages memory at multiple levels. It simulates dynamic memory allocation, multilevel CPU caches, and virtual memory using paging, providing a command-line interface (CLI) for interaction.

The goal is to accurately simulate OS memory-management behavior using well-defined data structures and algorithms, rather than building a functional OS kernel.

---

## 🚀 Features

### 1. Physical Memory Allocation
* Simulates a contiguous block of physical memory.
* Supports dynamic allocation and deallocation.
* Explicit tracking of allocated and free blocks.
* Automatic **coalescing** of adjacent free blocks.

### 2. Allocation Strategies
* **First Fit**: Allocates the first block that is large enough.
* **Best Fit**: Allocates the smallest block that fits to minimize wasted space.
* **Worst Fit**: Allocates the largest available block.
* Supports **block splitting** and **fragmentation tracking**.

### 3. Multilevel Cache Simulation (Core Feature)
* **L1, L2, and L3 caches** with configurable sizes, block sizes, and associativity.
* **Replacement Policies**: FIFO and LRU.
* **Addressing**: Tag, index, and offset-based addressing.
* **Propagation**: Miss penalty propagation across cache levels.
* **Statistics**: Detailed hit/miss reporting for every memory access.

### 4. Virtual Memory (Extension)
* Paging-based model: `Virtual Address → Page Table → Physical Address → Cache → Memory`.
* Page replacement policies: FIFO and LRU.
* Tracks page hits and page faults.

### 5. Buddy Memory Allocation (Extension)
* Memory size must be a power of two.
* Recursive splitting and buddy coalescing using XOR.
* Internal fragmentation tracking.

### 6. Metrics & Statistics
* Total, Used, and Free memory tracking.
* Internal and External fragmentation analysis.
* Allocation success rates and memory utilization.

---

## 📂 Project Structure

```text
memory-simulator/
├─ src/
│  ├─ allocator/        # Physical memory allocation logic
│  ├─ buddy/            # Buddy system implementation
│  ├─ cache/            # L1/L2/L3 cache logic
│  ├─ virtual_memory/   # Paging and translation
│  └─ main.cpp          # CLI and Entry point
├─ include/             # Header files (memsim.h)
├─ tests/               # Sample input scripts
├─ docs/                # Design documentation
├─ Makefile             # Build script
└─ README.md

```
### Build Instructions
Requirements
C++17 compatible compiler (g++)

Linux / macOS / WSL recommended

#### Build
```bash
make
```
This will generate the executable:

```bash
./memsim
```
Running the Simulator
```bash
./memsim
```
The simulator runs in interactive CLI mode.

Supported Commands
Memory Allocation
init memory <size>
set allocator first_fit | best_fit | worst_fit
malloc <size>
free <block_id>
dump
stats
Cache Access
access <physical_address>

Example output:
text
L1 MISS -> L2 HIT
