#ifndef HEAP_H
#define HEAP_H

#include "paging.h"

typedef struct Heap {
    int size;
    int capacity;
    struct rem_proc* rem_proc;
} Heap;

Heap* createHeap(int capacity);
void insertHeap(Heap* heap, struct rem_proc p);
struct rem_proc extractMin(Heap* heap);

#endif // HEAP_H