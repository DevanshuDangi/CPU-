# RISC-V 5-Stage Pipeline CPU Simulator

This project simulates a basic **RISC-V CPU pipeline** in C++ with 5 stages:
- **IF**: Instruction Fetch  
- **ID**: Instruction Decode  
- **EX**: Execute  
- **MEM**: Memory Access  
- **WB**: Write Back

It includes hazard detection, basic branching, jump handling, and register write-back support.

---

## 📁 Project Structure

```
.
├── main.cpp              # Entry point and pipeline execution
├── CPU.hpp / CPU.cpp     # CPU pipeline logic and stage management
├── ControlUnit.hpp       # Control signal generation
├── ALU.hpp               # ALU logic and control
├── InstructionMemory.hpp # Reads instructions into memory
├── utils.hpp             # Utility functions like ImmGen, SignedExtend, etc.
├── machine.txt           # Machine code (auto-generated from assembly)
├── assembly.txt          # Input RISC-V assembly file
├── README.md             # Project documentation
```

---

## 🚀 Features

- Supports basic RISC-V instructions:
  - `addi`, `beq`, `add`, `mul`, `sw`
- Implements all five pipeline stages
- Hazard detection and basic forwarding
- Instruction memory and simple data memory
- Output of register file and memory state after simulation

---

## 🔧 How to Compile and Run

### 🔨 Compile

Use `g++` or any C++17-compatible compiler:

```bash
g++ -std=c++17 main.cpp -o riscv_sim
```

### ▶️ Run

```bash
./riscv_sim
```

---

## 🧪 Example: Factorial of 7

Place this RISC-V assembly code inside `assembly.txt`:

```asm
addi x0, x0, 1
addi x1, x1, 7
addi x1, x1, 1
addi x2, x2, 1
beq x0, x1, 16
mul x2, x2, x0
addi x0, x0, 1
beq x0, x0, -12
```

The simulator will:
- Convert it to binary (machine.txt)
- Run it through the 5-stage pipeline
- Output register values and memory state

---

## 📌 Notes

- You can modify the instruction set and memory size as needed.
- Only a subset of the RISC-V spec is implemented.
- Extend `ALUControl` and `ControlUnit` for more instructions (`slti`, `xor`, `sll`, etc).

---

## 🧠 Credits

Developed by [Your Name]  
Educational pipeline simulator for learning purposes.
