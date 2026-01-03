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
🛠 Build InstructionsRequirementsC++17 compatible compiler (g++)Linux / macOS / WSL recommendedBuildRun the following command in the root directory:Bashmake
This will generate the executable: ./memsim🎮 Running the SimulatorLaunch the interactive CLI:Bash./memsim
Supported CommandsCategoryCommandDescriptionAllocationinit memory <size>Initialize physical memoryset allocator <type>first_fit, best_fit, or worst_fitmalloc <size>Allocate memory blockfree <block_id>Deallocate memory blockCacheaccess <address>Simulate memory access through L1/L2/L3VMinit_vm <size> <page>Initialize Virtual Memory pagingvm_access <address>Translate and access virtual addressSystemstatsDisplay memory and cache statisticsdumpVisualize memory layout📝 Example SessionPlaintext> init memory 1024
> set allocator first_fit
> malloc 100
Allocated id=1
> malloc 200
Allocated id=2
> free 1
Freed
> dump
[0 - 99] FREE
[100 - 299] USED (id=2)
[300 - 1023] FREE
> stats
Total memory: 1024
Used memory: 200
External fragmentation: 35.00%
⚠️ Assumptions & SimplificationsSingle-process simulation.Virtual memory is representational (no backing disk swap file).Cache write policies (write-back/write-through) are not implemented.No process isolation or protection bits (Read/Write/Execute).👤 AuthorAryan Sharma ACM Memory Management Project
