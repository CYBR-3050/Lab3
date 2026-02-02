# Lab 3: Overrun of Intended Bounds in a C Program

## Background
This lab explores what can happen when a program reads beyond the **intended bounds** of an array in C. You will use a small program (**mystuff.c**) and the GNU Debugger (**gdb**) to observe memory layout in and around a C `struct`.

This activity is adapted from a Labtainers exercise illustrating overrunning intended bounds in a C program.

---

## Step 1: Get the Lab Files (GitHub Clone)
The files needed for this lab are in the course GitHub repo.

1. Open a terminal.
2. Change to a directory where you keep lab work (example: your home directory).
3. Clone the repository:
   ```bash
   git clone https://github.com/CYBR-3050/Lab3
   ```
4. Change into the lab directory (adjust if your repo layout differs):
   ```bash
   cd Lab3
   ```
5. Confirm you can see `mystuff.c`:
   ```bash
   ls -la
   ```

### Reflection Questions
- What do you know about **buffer overflow** attacks?
- What are the differences between compiled binaries and source code?

---

## Step 2: Review the `mystuff.c` Program
Before compiling, read the program and identify the data structure layout.

1. View the code:
   ```bash
   less mystuff.c
   ```
2. Locate the `myData` struct and identify:
   - The `public_info` array size
   - The `pin` field type
   - Any additional fields after `pin`

The lab is designed around this question: if `public_info[19]` is the last element, what does `public_info[20]` refer to? 

### Reflection Questions
- In C, what does it mean to access an array index outside the declared bounds?
- Based on the struct definition, what do you suspect is adjacent to `public_info` in memory?

---

## Step 3: Compile and Run the Program
You will compile with debug symbols and (optionally) as a 32-bit binary for easier inspection.

We first need to update Kali.

Do this in your Kali terminal:

1) Update package lists
```bash
sudo apt update
```

2) Install the missing headers + compiler toolchain
```bash
sudo apt install -y build-essential libc6-dev
```

3) Install Multilib Support (so we can compile in 32-bits)
```bash
sudo apt install -y gcc-multilib libc6-dev-i386
```
Now we are good to compile and run the program.

1. Compile:
   ```bash
   gcc -m32 -g -o mystuff mystuff.c
   ```
   This matches the compile approach in the sample lab.

   If `-m32` fails, install multilib support:
   ```bash
   sudo apt update
   sudo apt install -y gcc-multilib
   ```

2. Run:
   ```bash
   ./mystuff
   ```
3. When prompted for offsets, try:
   - Inside the array: `0`, `5`, `19`
   - Beyond the array: `20`, `21`, `24`, `28`, `32`

The program prints the addresses of fields and lets you view byte values at selected offsets. 

### Reflection Questions
- What are the printed addresses of `public_info` and `pin`? How many bytes apart are they?
- At what offset do you first observe bytes that seem to correspond to `pin`?
- Why might there be padding between fields in a struct (alignment)? 

---

## Step 4: Explore Memory Using `gdb`
Now confirm your observations by examining memory in the debugger.

GDB, the GNU Project debugger, allows you to see what is going on `inside' another program while it executes -- or what another program was doing at the moment it crashed.

GDB can do four main kinds of things (plus other things in support of these) to help you catch bugs in the act:

- Start your program, specifying anything that might affect its behavior.
- Make your program stop on specified conditions.
- Examine what has happened, when your program has stopped.
- Change things in your program, so you can experiment with correcting the effects of one bug and go on to learn about another.

1. Start gdb:
   ```bash
   gdb mystuff
   ```
2. View source:
   ```gdb
   list
   ```
3. Set a breakpoint inside `showMemory` on the line where it prints the byte value, then run:
   ```gdb
   list showMemory
   break <line_number>
   run
   ```

4. When the breakpoint hits, display 10 words (40 bytes) starting at the struct:
   ```gdb
   x/10x &data
   ```

### Reflection Questions
- Does the gdb memory dump correspond to what you observed while running normally? Why or why not?
- Can you identify where `public_info` ends and where `pin` begins?
- What is the advantage of using gdb vs. only printing memory from inside the program?

---

## Step 5: Extra Exploration (Optional)
If you have time, explore the return path described in the sample lab: step through instructions near the end of `handleMyStuff`, observe `ret`, and inspect the stack. 

### Reflection Questions
- What does the `ret` instruction do at a high level?
- Why do return addresses matter in memory safety vulnerabilities?

---

## Step 6: Submission
Submit **one document** containing answers to all reflection questions.
