CYBR 3050 - Buffer Overflow (Scaffolded, Safety-Focused) Lab Files

Included:
- mystuff.c        : Read-only bounds exploration in a struct
- vuln_stack.c     : Demonstration-only stack overflow program (NOT setuid)
- make_input.py    : Generates input.bin with a chosen length
- Makefile         : Builds protected/unprotected/ASan variants

Quick start:
  make
  make input
  ./vuln_protected input.bin

For observation under gdb:
  make vuln_unprotected
  gdb ./vuln_unprotected
