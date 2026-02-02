#!/usr/bin/env python3
"""
make_input.py
CYBR 3050 - Demo input generator

Creates input.bin containing a repeated pattern. Vary length to:
- stay within bounds (no crash)
- overflow buffer (may crash, or under gdb show control-data overwrite)

Usage:
  ./make_input.py 16
  ./make_input.py 80
  ./make_input.py 200
"""
import sys

def main():
    n = 80
    if len(sys.argv) > 1:
        n = int(sys.argv[1])

    payload = b"A" * n

    with open("input.bin", "wb") as f:
        f.write(payload)

    print(f"Wrote input.bin with {n} bytes of 'A'.")

if __name__ == "__main__":
    main()
