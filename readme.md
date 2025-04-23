PAGING SIMULATION SYSTEM
========================

A C implementation of a simple paging system with First-Come First-Served (FCFS) process scheduling.

DESCRIPTION
-----------
This program simulates a paging memory management system with:
- 256KB RAM divided into 4KB frames (64 total frames)
- First-Fit frame allocation strategy
- FCFS process scheduling using a min-heap
- Visual RAM state tracking with frame tables
- Process memory allocation/deallocation simulation

KEY FEATURES
------------
- Processes wait for memory if insufficient space
- Optimised Data Structure(Heap) usage for managing process completion times
- Contiguous frame allocation with wrap-around
- Visual representation of RAM state changes
- Process execution timeline tracking
- Frame allocation/deallocation logging

REQUIREMENTS
------------
- C compiler (GCC recommended)
- Linux/Unix-like system (for system("clear"))

Folder Structure
--------------------

OS_OEL/
├── headers/    
│   ├── paging.h
│   ├── heap.h
│   └── table.h
├── main.c
├── heap.c
├── table.c
├── input.txt
└── readmeS.md

CONFIGURATION
-------------
Modify in headers/paging.h:
- #define RAMSIZE      // Total RAM size (KB)
- #define FRAMESIZE    // Frame size (KB)
- #define MAXCAPACITY  // Max processes

*Compile and execute by:*

`gcc -Iheaders main.c heap.c table.c -o main`
`./main`

PROJECT STRUCTURE
-----------------
- main.c: Core simulation logic
- heap.c: Min-heap implementation
- table.c: RAM visualization system
- paging.h: Shared constants and structs
