#include "table.h"
#include <stdio.h>
#include <string.h>

static void print_table(char headers[][20], char data[][4][20], int rows, int cols) {
    int column_widths[cols];
    for (int i = 0; i < cols; i++) {
        int max_width = strlen(headers[i]);
        for (int j = 0; j < rows; j++) {
            int data_width = strlen(data[j][i]);
            if (data_width > max_width) max_width = data_width;
        }
        column_widths[i] = max_width + 2;
    }

    // Print headers
    for (int i = 0; i < cols; i++) {
        printf("| %-*s ", column_widths[i] - 2, headers[i]);
    }
    printf("|\n");

    // Print separator
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < column_widths[i]; j++) printf("-");
        printf("-");
    }
    printf("|\n");

    // Print data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("| %-*s ", column_widths[j] - 2, data[i][j]);
        }
        printf("|\n");
    }
}

void table(int ram[], struct process processes[], int freeFrames) {
    char headers[4][20] = {"  Frame Number  ", "  Process Number  ", 
                          "  Page Number  ", " Exit time(ms) "};
    char data[totalFrames][4][20];
    
    for (int i = 0; i < totalFrames; i++) {
        sprintf(data[i][0], "%d", i);
        int proc = ram[i] != -1 ? ram[i]/1000 : -1;
        int page = ram[i] != -1 ? ram[i]%1000 : -1;
        int time = proc != -1 ? processes[proc].entime : -1;
        
        sprintf(data[i][1], "%d", proc);
        sprintf(data[i][2], "%d", page);
        sprintf(data[i][3], "%d", time);
    }
    
    printf("Free Frames in RAM: %d\n\n", freeFrames);
    print_table(headers, data, totalFrames, 4);
}