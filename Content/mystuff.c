/*
 * mystuff.c - Intended-bounds exploration (read-only overrun)
 * CYBR 3050
 *
 * This program intentionally allows OUT-OF-BOUNDS READS for educational purposes
 * so you can observe how adjacent fields can appear in memory.
 *
 * Build:
 *   gcc -g -o mystuff mystuff.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct myData {
    char public_info[20];   /* intended: 0..19 */
    int  pin;               /* adjacent field */
    char fav_color[9];
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
        if (!fgets(line, sizeof(line), stdin)) return;
        if (line[0] == 'q' || line[0] == 'Q') return;

        char *end = NULL;
        long off = strtol(line, &end, 10);
        if (end == line || off < 0 || off > 80) {
            printf("Please enter a number 0-80 or 'q'.\n");
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

    printf("=== mystuff.c: bounds exploration (read-only) ===\n\n");
    printf("public_info addr: %p\n", (void *)data.public_info);
    printf("pin addr        : %p\n", (void *)&data.pin);
    printf("fav_color addr  : %p\n\n", (void *)data.fav_color);

    printf("public_info: \"%s\"\n", data.public_info);
    printf("pin (dec): %d\n", data.pin);
    printf("fav_color: \"%s\"\n", data.fav_color);

    printf("\nTip: public_info has 20 bytes (0..19). Try reading offset 20+.\n");
    handleMyStuff(&data);
    printf("\nDone.\n");
    return 0;
}
