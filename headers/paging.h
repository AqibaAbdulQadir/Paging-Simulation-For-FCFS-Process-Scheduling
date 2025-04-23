#ifndef PAGING_H
#define PAGING_H

#include <stdbool.h>

#define RAMSIZE 256
#define MAXCAPACITY 15
#define FRAMESIZE 4

extern int totalFrames;  // Will be defined in main.c

struct rem_proc {
    int pid;
    int entime;
};

struct process {
    int pid;
    int size;
    int intime;
    int pages;
    int entime;
    bool done;
    int btime;
    int* pages_st;
    int* pages_en;
    int breaks;
};

#endif // PAGING_H