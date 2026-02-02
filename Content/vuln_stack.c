/*
 * vuln_stack.c - Demonstration-only stack buffer overflow
 * CYBR 3050
 *
 * PURPOSE (educational):
 *   Demonstrate how an unsafe copy (strcpy) can overwrite control data
 *   such as a saved return address. This lab is designed to observe a crash
 *   and (optionally) observe the instruction pointer becoming 0x41414141
 *   ("AAAA") under gdb with protections disabled AT COMPILE TIME.
 *
 * IMPORTANT:
 *   - This program is NOT setuid and must NOT be made setuid.
 *   - This lab focuses on understanding vulnerability mechanics and defenses,
 *     not gaining shells or privileges.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void banner(void) {
    puts("=== vuln_stack.c: overflow mechanics demo ===");
    puts("This program copies data into a fixed-size stack buffer using strcpy().");
    puts("Provide input via a file to trigger an overflow and observe behavior.");
}

int bof(const char *input) {
    char buffer[32];
    strcpy(buffer, input);              /* Vulnerability: no bounds check */
    if (buffer[0] == 'Z') puts("Unlikely branch");
    return 1;
}

int main(int argc, char **argv) {
    banner();

    const char *fname = "input.bin";
    if (argc > 1) fname = argv[1];

    FILE *f = fopen(fname, "rb");
    if (!f) {
        perror("fopen");
        puts("Create input.bin first (see make input / python script).");
        return 1;
    }

    char data[512];
    size_t n = fread(data, 1, sizeof(data)-1, f);
    fclose(f);
    data[n] = '\0';

    puts("\nCalling bof()...");
    bof(data);
    puts("Returned normally (if you see this, overflow did not smash control data).");
    return 0;
}
