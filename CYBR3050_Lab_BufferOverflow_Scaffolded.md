# CYBR 3050 – Lab: Buffer Overflow Mechanics (Scaffolded Demonstration)

## Background
Buffer overflow is a memory-safety bug where a program writes past the end of a fixed-size buffer. In classic stack-based overflows, this can corrupt **control data** (such as a saved return address), which historically enabled attackers to alter program flow and execute unintended code.

This lab builds on your earlier “intended bounds” exploration (reading past a buffer) and adds a **controlled, safety-focused** demonstration of a stack buffer overflow. The goal is to *observe* how overwriting happens and how modern defenses respond—**not** to gain a shell or escalate privileges.

The reference document explains that buffer overflow can change control flow by mixing storage for data and storage for controls (e.g., return addresses). fileciteturn2file0L6-L15

---

## Step 1: Get the Lab Files (GitHub Clone)
1. Open a terminal.
2. Clone the lab repository:
   ```bash
   git clone https://github.com/CYBR-3050/Labs/Lab1
   ```
3. Change into the lab directory (adjust path if needed):
   ```bash
   cd Lab1
   ```
4. Confirm the lab files are present:
   ```bash
   ls -la
   ```
   You should see:
   - `mystuff.c`
   - `vuln_stack.c`
   - `make_input.py`
   - `Makefile`

### Reflection Questions
- Why is it important to verify the origin and integrity of security lab code before compiling/running it?
- In an operational environment, what risks come with compiling and executing unknown C code?

---

## Step 2: “Overrun of Intended Bounds” Refresher (Read Beyond a Buffer)
This step reinforces the *memory layout* idea using a program that performs **out-of-bounds reads** (not writes).

1. Build and run:
   ```bash
   make mystuff
   ./mystuff
   ```
2. When prompted for offsets, try values:
   - Inside bounds: `0`, `5`, `19`
   - Outside bounds: `20`, `24`, `28`, `32`

### Background (What’s happening and why it matters)
In C, arrays do not carry bounds metadata at runtime. When you read `public_info[20]`, the CPU will still read “whatever byte is at that memory address,” even if it belongs to the next field in the struct. This is why memory-safety matters: neighboring data can be exposed or corrupted.

### Reflection Questions
- What data appeared when you read beyond the intended bounds?
- Why are out-of-bounds reads a security concern even if nothing “crashes”?

---

## Step 3: Build the Overflow Demo Program (Protected Build)
Now you will compile a demonstration program that contains an unsafe `strcpy()` into a small stack buffer. The program reads input from `input.bin`.

1. Build the protected version:
   ```bash
   make vuln_protected
   ```
2. Create a small input and run:
   ```bash
   ./make_input.py 16
   ./vuln_protected input.bin
   ```
3. Create a larger input and run again:
   ```bash
   ./make_input.py 200
   ./vuln_protected input.bin
   ```

### Background (What’s happening and why it matters)
The vulnerable function copies bytes into a fixed-size buffer without bounds checking. In the classic model, if enough bytes are copied, the overflow can overwrite adjacent stack values, potentially including saved frame pointers and return addresses. The reference describes this as corruption of control flow (e.g., return address) caused by overflow. fileciteturn2file0L11-L15

Modern toolchains often include mitigations (stack canaries/StackGuard, ASLR, NX, PIE) that change what you observe. The reference document discusses StackGuard and non-executable stack as protection schemes. fileciteturn2file0L18-L33

### Reflection Questions
- What differences did you observe between running with 16 bytes vs. 200 bytes?
- Did the program crash? If so, what message did you see and what do you think it indicates?

---

## Step 4: Use AddressSanitizer (ASan) to See a Diagnostic Report
ASan is a compiler/runtime tool that detects memory errors and prints a detailed report.

1. Build the ASan version:
   ```bash
   make vuln_asan
   ```
2. Generate an input likely to overflow and run:
   ```bash
   ./make_input.py 200
   ./vuln_asan input.bin
   ```

### Background (What’s happening and why it matters)
ASan inserts checks and metadata so that overflows can be detected with clearer explanations than a generic crash. In defensive work, instrumentation is often used during development/testing to reduce exploitable vulnerabilities before deployment.

### Reflection Questions
- What did ASan report (summary in your own words)?
- How does instrumentation like ASan change a defender’s workflow compared to “debugging crashes”?

---

## Step 5: Observe Control-Data Overwrite in gdb (Controlled Demonstration)
In this step, you will compile an **observation-friendly** binary that disables some protections at compile time so you can see stack behavior more directly under gdb.

> **Safety note:** This is only for controlled lab observation. Do not use these flags for real software.

1. Build the observation binary:
   ```bash
   make vuln_unprotected
   ```
2. Start gdb:
   ```bash
   gdb ./vuln_unprotected
   ```
3. (Optional) In gdb, reduce randomness for this debug session:
   ```gdb
   set disable-randomization on
   ```
4. Set a breakpoint in `bof` and run with a large input:
   ```gdb
   break bof
   run input.bin
   ```
5. When it breaks, inspect the stack near the current frame:
   ```gdb
   info frame
   x/40x $esp
   ```
6. Continue execution:
   ```gdb
   continue
   ```

### Background (What’s happening and why it matters)
A stack frame contains local variables (like `buffer`) and also control information used to return to the caller (e.g., saved return address). The reference provides a conceptual stack layout and explains that knowing buffer location helps reason about where the return address is stored. fileciteturn2file0L131-L161

This lab is designed to help you recognize when unsafe copies can corrupt control data and how defenders/debuggers validate that risk.

### Reflection Questions
- In your stack dump, do you see a region filled with `0x41414141` (AAAA)? Where is it relative to the frame?
- What does that imply about how far your input reached in memory?
- Why do compile-time mitigations (stack protector, PIE, NX) matter for real systems? fileciteturn2file0L18-L33

---

## Step 6: Compare Protections (Short Analysis)
You have now seen three perspectives:
- Default (“protected”) behavior
- ASan diagnostics
- Direct stack observation in gdb (compile-time toggles for visibility)

### Reflection Questions
- Which approach (protected run, ASan, gdb) gave you the most actionable understanding, and why?
- If you were a defender triaging a crash report, what artifacts would you want (core dump, ASan report, logs, reproduction steps)?

---

## Step 7: Submission
Submit **one document** containing your answers to all reflection questions.
Include:
- The input sizes you tested
- Any key error messages (copy/paste is fine)
- Short explanations in your own words

---

## Shutdown Lab
- Exit gdb with `quit` (if used)
- Clean up build artifacts if desired:
  ```bash
  make clean
  ```
