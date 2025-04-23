#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(struct rem_proc* a, struct rem_proc* b) {
    struct rem_proc temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify(Heap* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && 
        heap->rem_proc[left].entime < heap->rem_proc[largest].entime)
        largest = left;

    if (right < heap->size && 
        heap->rem_proc[right].entime < heap->rem_proc[largest].entime)
        largest = right;

    if (largest != i) {
        swap(&heap->rem_proc[i], &heap->rem_proc[largest]);
        heapify(heap, largest);
    }
}

Heap* createHeap(int capacity) {
    Heap* heap = malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->rem_proc = malloc(capacity * sizeof(struct rem_proc));
    return heap;
}

void insertHeap(Heap* heap, struct rem_proc p) {
    if (heap->size == heap->capacity) {
        printf("Heap overflow\n");
        return;
    }

    heap->rem_proc[heap->size] = p;
    int i = heap->size++;
    
    while (i > 0 && heap->rem_proc[(i-1)/2].entime > heap->rem_proc[i].entime) {
        swap(&heap->rem_proc[i], &heap->rem_proc[(i-1)/2]);
        i = (i-1)/2;
    }
}

struct rem_proc extractMin(Heap* heap) {
    if (heap->size <= 0) return (struct rem_proc){0};
    
    struct rem_proc root = heap->rem_proc[0];
    heap->rem_proc[0] = heap->rem_proc[--heap->size];
    heapify(heap, 0);
    return root;
}
