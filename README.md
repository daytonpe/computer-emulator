# Overview

Project emulates a simple computer system consisting of a CPU and Memory.
CPU and Memory are separated by separate processes that communicate.
Memory contains one program that the CPU executes and the simulation will end.

### CPU

  * It will have these registers: PC, SP, IR, AC, X, Y. (These will be ints)
  * It will support the instructions shown on the next page of this document.
  * It will run the user program at address 0.
  * Instructions are fetched into the IR from memory. The operand can be fetched into a local variable.
  * Each instruction should be executed before the next instruction is fetched.
  * The user stack resides at the end of user memory and grows down toward address 0.
  * The system stack resides at the end of system memory and grows down toward address 0.
  * There is no hardware enforcement of stack size.
  * The program ends when the End instruction is executed. The 2 processes should end at that time.
  * The user program cannot access system memory (exits with error message).

## Memory

  * It will consist of 2000 integer entries, 0-999 for the user program, 1000-1999 for system code.
  * It will support two operations:
    * read(address) - returns the value at the address
    * write(address, data) - writes the data to the address
  * Memory will initialize itself by reading a program file.

## Timer

A timer will interrupt the processor after every X instructions, where X is a command-line parameter.

## Interrupt Process
  * There are two forms of interrupts: the timer and a system call using the int instruction.
  * In both cases the CPU should enter kernel mode.
  * The stack pointer should be switched to the system stack.
  * SP and PC registers should be saved on the system stack. (The handler may save additional registers).
  * A timer interrupt should cause execution at address 1000.
  * The int instruction should cause execution at address 1500.
  * Interrupts should be disabled during interrupt processing to avoid nested execution.
  * The iret instruction returns from an interrupt.

---

## Running the Code

```
g++ -std=c++11 index.cpp
.a/.out [program.txt] [timer frequency]
```

* program is in a standard .txt format
* timer frequency controls how often the timer interrupt is run
* Example: `.a/.out sample1.txt 30`

---

## Instruction Set

* 1 = Load value - Load the value at the address into the AC
* 2 = Load addr - Load the value at the address into the AC
* 3 = LoadInd addr - Load the value from the address found in the given address into the AC (for example, if LoadInd 500, and 500 contains 100, then load from 100).
* 4 = LoadIdxX addr - Load the value at (address+X) into the AC (for example, if LoadIdxX 500, and X contains 10, then load from 510).
* 5 = LoadIdxY addr - Load the value at (address+Y) into the AC
* 6 = LoadSpX - Load from (Sp+X) into the AC (if SP is 990, and X is 1, load from 991).
* 7 = Store addr - Store the value in the AC into the address
* 8 = Get - Gets a random int from 1 to 100 into the AC
* 9 = Put port - If port=1, writes AC as an int to the screen. If port=2, writes AC as a char to the screen
* 10 = AddX - Add the value in X to the AC
* 11 = AddY - Add the value in Y to the AC
* 12 = SubX - Subtract the value in X from the AC
* 13 = SubY - Subtract the value in Y from the AC
* 14 = CopyToX - Copy the value in the AC to X
* 15 = CopyFromX - Copy the value in X to the AC
* 16 = CopyToY - Copy the value in the AC to Y
* 17 = CopyFromY - Copy the value in Y to the AC
* 18 = CopyToSp - Copy the value in AC to the SP
* 19 = CopyFromSp - Copy the value in SP to the AC
* 20 = Jump addr - Jump to the address
* 21 = JumpIfEqual addr - Jump to the address only if the value in the AC is zero
* 22 = JumpIfNotEqual addr - Jump to the address only if the value in the AC is not zero
* 23 = Call addr - Push return address onto stack, jump to the address
* 24 = Ret - Pop return address from the stack, jump to the address
* 25 = IncX - Increment the value in X
* 26 = DecX - Decrement the value in X
* 27 = Push - Push AC onto stack
* 28 = Pop - Pop from stack into AC
* 29 = Int - Perform system call
* 30 = IRet - Return from system call
* 50 = End Load the value into the AC - End execution

Load the value at (address+X) into the AC
(for example, if LoadIdxX 500, and X contains 10, then load from 510).

---

## Input File Format

* Each instruction is on a separate line, with its operand (if any) on the following line.
* The instruction or operand may be followed by a comment which the loader will ignore.
* Anything following an integer is a comment, whether or not it begins with //.
* A line may be blank in which case the loader will skip it without advancing the load address.
* A line may begin by a period followed by a number which causes the loader to change the load address.
* Your program should run correctly with the any valid input files.

---

## Output

See screenshots...
