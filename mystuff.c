/*
 * mystuff.c
 * CYBR 3050 - Overrun Exploration (C + gdb)
 *
 * Purpose:
 *   Observe what happens when reading beyond an array's intended bounds
 *   inside a struct. This program intentionally allows out-of-bounds reads
 *   for educational purposes.
 *
 * Build (recommended):
 *   gcc -m32 -g -o mystuff mystuff.c
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct myData {
    char public_info[20];   /* Intended bounds: 0..19 */
    int  pin;               /* Adjacent field */
    char fav_color[9];      /* Odd-sized buffer to potentially show padding/alignment */
} myData;

static void showMemory(myData *data, int offset) {
    unsigned char *base = (unsigned char *)data->public_info;
    unsigned char value = base[offset];

    printf("Offset %3d  Address %p  Value 0x%02x  (%c)\n",
           offset, (void *)(base + offset), value,
           isprint(value) ? value : '.');
}

static void handleMyStuff(myData *data) {
    char line[64];

    while (1) {
        printf("\nEnter an offset to display (0-80), or 'q' to quit: ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\nEOF received. Exiting.\n");
            return;
        }
        if (line[0] == 'q' || line[0] == 'Q') {
            return;
        }

        char *end = NULL;
        long off = strtol(line, &end, 10);
        if (end == line) {
            printf("Please enter a number or 'q'.\n");
            continue;
        }
        if (off < 0 || off > 80) {
            printf("Offset out of allowed range. Try 0-80.\n");
            continue;
        }

        showMemory(data, (int)off);
    }
}

int main(void) {
    myData data;

    memset(&data, 0, sizeof(data));
    strncpy(data.public_info, "public: hello-student", sizeof(data.public_info) - 1);
    data.pin = 1234;
    strncpy(data.fav_color, "blue", sizeof(data.fav_color) - 1);

    printf("=== mystuff.c: bounds exploration ===\n\n");
    printf("public_info (start) address: %p\n", (void *)data.public_info);
    printf("pin        (field) address: %p\n", (void *)&data.pin);
    printf("fav_color  (start) address: %p\n\n", (void *)data.fav_color);

    printf("public_info contents: \"%s\"\n", data.public_info);
    printf("pin value (decimal): %d\n", data.pin);
    printf("fav_color contents: \"%s\"\n\n", data.fav_color);

    printf("Tip: public_info has 20 bytes (0..19). Try reading offset 20+.\n");
    handleMyStuff(&data);

    printf("\nDone.\n");
    return 0;
}
