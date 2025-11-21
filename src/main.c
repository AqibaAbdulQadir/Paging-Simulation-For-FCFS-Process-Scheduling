#include "paging.h"
#include "heap.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int totalFrames = RAMSIZE/FRAMESIZE;

static void remove_process(Heap* minHeap, struct process processes[], 
                         int* ram, int* freeFrames, int* now) {
    struct rem_proc rp = extractMin(minHeap);
    if (rp.pid == 0 && rp.entime == 0) return;
    int st, en, temp, wrap = 0;
    struct process* p = &processes[rp.pid];

    for (int j = 0; j < p->breaks; j++) {
        st = p->pages_st[j];
        en = p->pages_en[j];
        if (st > en){
            wrap = 1;
            en = totalFrames - 1;
        }
        // printf("%d %d %d", rp.pid, st, en);
        for (int k = st; k <= en; k++)
            ram[k] = -1;
        
        if(wrap == 1){
            for (int k = 0; k <= en; k++)
                ram[k] = -1;
        }
    }

    *freeFrames += p->pages;
    p->done = true;
    if (rp.entime > *now) *now = rp.entime;
    
    printf("\nProcess %d freed %d frames at %dms\n", 
          rp.pid, p->pages, rp.entime);
}



int main() {
    	
    int ram[totalFrames];
    int i, j, k, p, b, n=10, insrt_ptr = 0, now = 0;
    int freeFrames = totalFrames;
    char input[2];
    
    for(i = 0; i < totalFrames; i++) {
    ram[i] = -1; // initially empty RAM
    }
    
    // Uncomment the code below to manually input process number
    /* printf("Enter no. of processes: ");
    scanf("%d", &n);
    printf("\n");

    if (n > MAXCAPACITY) {
        printf("Too many processes. Maximum is %d\n", MAXCAPACITY); 
        // limit was necessary to determine array size -> constraint due to language
        return 1;
    } */

    printf("\nReal-time simulation of Simple Paging wih FCFS Scheduling Algorithm using 10 processes\n\n");

    struct process* processes;
    processes = (struct process*)malloc(n * sizeof(struct process));
    //int rmv_processes[n];
    Heap* minHeap = createHeap(n);
    struct rem_proc* rem_proc;
    rem_proc = (struct rem_proc*)malloc(n * sizeof(struct rem_proc));
    struct rem_proc temp_rem_proc;

    FILE* file = fopen("input.txt", "r");

    char arrvs[20];
    char sizes[20];
    char execs[20];
    i = 0;

    if (file != NULL) {
        while (fgets(arrvs, sizeof(arrvs), file)) {
            fgets(sizes, sizeof(arrvs), file);
            fgets(execs, sizeof(arrvs), file);
            processes[i].pid = i;
            processes[i].intime = atoi(arrvs);
            processes[i].size = atoi(sizes);
            processes[i].btime = atoi(execs);

            if (processes[i].size > RAMSIZE) {
                printf("Process size is too large. It cannot be allocated in RAM of size %d KB\n", RAMSIZE); 
                return 1;
            }
            processes[i].pages = processes[i].size / FRAMESIZE + (processes[i].size % FRAMESIZE != 0);
            processes[i].pages_st = (int*)malloc(processes[i].pages * sizeof(int));
            processes[i].pages_en = (int*)malloc(processes[i].pages * sizeof(int));
            if (processes[i].intime > now) now = processes[i].intime;

            // Removes executed processes from memory
            if (minHeap->size > 0){
                do{
                    // printf("%d after\n", now);
                    temp_rem_proc = extractMin(minHeap);	
                    insertHeap(minHeap, temp_rem_proc);
                    if (temp_rem_proc.entime <= now){
                        // printf("%d %d %d  therefore\n", now, temp_rem_proc.pid, temp_rem_proc.entime);
                        remove_process(minHeap, processes, ram, &freeFrames, &now);
                        table(ram, processes, freeFrames);
                        while(getchar() != '\n');
                        printf("Press Enter to continue...");
                        fgets(input, sizeof(input), stdin);
                        system("clear");
                    }  
                }while(temp_rem_proc.entime <= now && minHeap->size > 0);
            }

            printf("Process %d shows up at time %d ms.\n", i, processes[i].intime);
            printf("Process %d has size %d KB with %d pages.\n", i, processes[i].size, processes[i].pages);
            printf("Process %d has execution time %d ms.\n\n",i , processes[i].btime);

            // Waits for memory to free up before assigning new process
            while (freeFrames < processes[i].pages){
                printf("Process %d is waiting while memory frees up.\n\n", i);
                remove_process(minHeap, processes, ram, &freeFrames, &now);
                table(ram, processes, freeFrames);
                while(getchar() != '\n');
                printf("Press Enter to continue...");
                fgets(input, sizeof(input), stdin);
                system("clear");
            }

            p = 0; // page number for the current process to allocate
            b = 0; // breaks while storing pages
            printf("Process %d is being assigned to RAM.\n\n", i);
            while(p != processes[i].pages){
                processes[i].pages_st[b] = -1;
                processes[i].pages_en[b] = -1;
            
                while (ram[insrt_ptr] == -1 && p != processes[i].pages){
                    if(processes[i].pages_st[b] == -1) processes[i].pages_st[b] = insrt_ptr;
                    ram[insrt_ptr] = i*1000 + p;
                    p++;
                    insrt_ptr = (insrt_ptr + 1) % totalFrames;
                }
                if(processes[i].pages_st[b] != -1){
                    processes[i].pages_en[b] = (insrt_ptr - 1 + totalFrames) % totalFrames;
                    b++;
                }
                if(p != processes[i].pages) insrt_ptr = (insrt_ptr + 1) % totalFrames;
            }
        
            rem_proc[i].pid = i;
            rem_proc[i].entime = now + processes[i].btime;
            processes[i].entime = rem_proc[i].entime;
            processes[i].done = false;
            insertHeap(minHeap, rem_proc[i]);
            processes[i].breaks = b;
            freeFrames -= processes[i].pages;
            
            printf("\nCURRENT STATE OF THE RAM AFTER INSERTION OF PROCESS %d at time %d ms\n\n", i, now);
            printf("Process %d occupied %d frames\n", i, processes[i].pages);
            table(ram, processes, freeFrames);

            while(getchar() != '\n');
            printf("Press Enter to continue...");
            fgets(input, sizeof(input), stdin);
            system("clear");

            // printf("%d %d %d", processes[i].btime, processes[i].intime, processes[i].size);
            i++;
        }
        fclose(file);
    }
    else {
        fprintf(stderr, "Unable to open file!\n");
    }

    // To remove remaining processes from RAM after all processes have been catered
    while (minHeap->size != 0){
        remove_process(minHeap, processes, ram, &freeFrames, &now);
        table(ram, processes, freeFrames);
        while(getchar() != '\n');
        printf("Press Enter to continue...");
        fgets(input, sizeof(input), stdin);
        system("clear");
    }
    printf("\nAll processes allocated and executed!\n\n");
    return 0;
}
