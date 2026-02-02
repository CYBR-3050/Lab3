CC=gcc
CFLAGS_COMMON=-Wall -Wextra -O0 -g

.PHONY: all clean input

all: mystuff vuln_protected

mystuff: mystuff.c
	$(CC) $(CFLAGS_COMMON) -o $@ $<

vuln_protected: vuln_stack.c
	$(CC) $(CFLAGS_COMMON) -o $@ $<

# Observation-friendly build (compile-time toggles for a controlled lab)
vuln_unprotected: vuln_stack.c
	$(CC) $(CFLAGS_COMMON) -fno-stack-protector -z execstack -no-pie -o $@ $<

# ASan build for clear diagnostics
vuln_asan: vuln_stack.c
	$(CC) $(CFLAGS_COMMON) -fsanitize=address -fno-omit-frame-pointer -o $@ $<

input:
	./make_input.py 80

clean:
	rm -f mystuff vuln_protected vuln_unprotected vuln_asan input.bin
