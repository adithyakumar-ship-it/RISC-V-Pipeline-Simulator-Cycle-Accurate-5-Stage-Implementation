# RISC-V Pipeline Simulator — Cycle-Accurate 5-Stage Implementation

> A cycle-accurate simulation of a 5-stage RISC-V pipelined processor implemented in C++, featuring hazard detection and resolution, pipeline register modeling, stage utilization tracking, and both instruction-mode and cycle-mode execution. Built for ECE 610: Foundations of Computer Engineering at UMass Amherst.

**Team:** Adithya Kumar, Durga Nirmaleswaran, Deepikaveni Venkatanarayanan  
**Course:** ECE 610 — Foundations of Computer Engineering, University of Massachusetts Amherst  
**Instructor:** Prof. Nikhil Saxena  
**Live Demo:** [Run on OnlineGDB](https://www.onlinegdb.com/ERG5R9aJ_)

---

## Overview

This simulator models the classic 5-stage RISC-V pipeline at the cycle level, allowing users to observe exactly how instructions move through each pipeline stage, how hazards are detected and resolved, and how different configurations affect processor throughput and efficiency.

The simulator supports two execution modes:
- **Instruction Mode (I):** Steps through one instruction at a time, showing pipeline state after each instruction completes
- **Cycle Mode (C):** Steps through one clock cycle at a time, showing all pipeline registers and register file contents at each cycle

---

## Pipeline Architecture

```
         ┌──────┐    ┌──────┐    ┌──────┐    ┌──────┐    ┌──────┐
 Instr ──►  IF  ├────►  ID  ├────►  EX  ├────► MEM  ├────►  WB  │
         └──────┘    └──────┘    └──────┘    └──────┘    └──────┘
              IF/ID       ID/EX      EX/MEM      MEM/WB
         [IR, NPC]  [IR,NPC,  [IR, B,     [IR,
                     A, B,     ALUOut,     ALUOut,
                     Imm]      cond]       LMD]
```

### Stage Descriptions

| Stage | Function |
|-------|----------|
| **IF — Instruction Fetch** | Fetches instruction from instruction memory using PC; increments PC by 4 |
| **ID — Instruction Decode** | Decodes opcode, reads register operands, extracts immediates, detects hazards |
| **EX — Execute** | ALU performs arithmetic/logic operations; computes branch targets and memory addresses |
| **MEM — Memory Access** | Reads from or writes to data memory for load/store instructions |
| **WB — Writeback** | Writes ALU result or loaded data back to the destination register |

### Pipeline Registers

| Register | Contents |
|----------|----------|
| IF/ID | IR, NPC |
| ID/EX | IR, NPC, A, B, Imm |
| EX/MEM | IR, B, ALUOutput, cond |
| MEM/WB | IR, ALUOutput, LMD |

---

## Supported Instructions

| Type | Instructions |
|------|-------------|
| **R-type** | add, sub, mul, div, rem, and, or, sll, srl |
| **I-type** | addi, subi, andi, ori, slti, sltiu, lw, jalr |
| **S-type** | sw |
| **B-type** | beq |
| **U-type** | lui |
| **J-type** | jal |

> **Note on mul:** Syntax is `mul a, b, c` — lower 32 bits of b×c stored in register `a`, upper 32 bits stored in register `a+1`.

---

## Hazard Detection & Resolution

### Data Hazards (RAW)
- Detected in the ID stage by comparing source register fields against destination registers of instructions currently in EX and MEM stages
- Resolved via **pipeline forwarding** — results are forwarded from EX/MEM and MEM/WB pipeline registers directly to the EX stage inputs
- Where forwarding is insufficient (e.g., load-use hazard), a **stall** is inserted

### Control Hazards
- Detected when a branch instruction (BEQ) enters the ID stage
- IF stage is stalled and IF/ID latch is flushed (NOP inserted) while the branch is resolved
- Branch outcome is resolved at the EX stage; IF resumes fetching from the correct address

### Structural Hazards
- Eliminated by using **separate instruction and data memories**, ensuring no resource conflicts between stages

---

## Memory & Register Specifications

| Component | Specification |
|-----------|--------------|
| Instruction Memory | Separate, word-addressable, 2KB |
| Data Memory | Separate, word-addressable, 2KB, initialized to 0 |
| Register File | 32 registers (x0–x31); x0 hardwired to 0 |
| PC | Initialized to start of instruction memory |
| Instruction Format | 32-bit hexadecimal, loaded from external `.txt` file |

---

## Sample Output

The simulator outputs cycle-by-cycle pipeline state, register file contents, and final simulation statistics:

```
=== Cycle 1 ===
Pipeline State:
  IF : addi x2, x1, 15
  ID :
  EX :
  MEM:

Registers:
x0: 0  x1: 0  x2: 0  x3: 0 ...

--- Simulation Statistics ---
Total Cycles: 19
Stage Utilization:
  EX:  89.47%
  ID:  94.74%
  IF: 100.00%
  MEM: 84.21%
  WB:  78.95%
Data Hazards: 1 stall
Control Hazards: No
```

---

## How to Run

### Option 1 — Run Online (No Setup Required)
Click here: **[Run on OnlineGDB](https://www.onlinegdb.com/ERG5R9aJ_)**

### Option 2 — Compile Locally

**Requirements:** C++11 or later

```bash
# Clone the repo
git clone https://github.com/yourusername/riscv-pipeline-simulator.git
cd riscv-pipeline-simulator

# Compile
g++ -std=c++11 -o simulator simulator.cpp

# Run
./simulator
```

**At runtime:**
1. Select `1` to load a program
2. Enter your machine code filename (e.g., `program.txt`)
3. Select mode: `I` for Instruction Mode or `C` for Cycle Mode
4. Enter number of instructions to execute

### Input File Format
Instructions should be provided as 32-bit hexadecimal values, one per line:
```
00F00093
00208133
...
```

---

## Repository Structure

```
├── README.md
├── simulator.cpp          # Main C++ simulator source
├── sample_programs/
│   └── y.txt              # Sample machine code input used in demo
└── docs/
    ├── pipeline_output_1.png    # Cycle 1–6 output screenshot
    ├── pipeline_output_2.png    # Cycle 9–19 output screenshot
    └── simulation_stats.png     # Final statistics screenshot
```

---

## Results

Tested on a 19-instruction program containing arithmetic, shift, branch, and memory operations:

| Metric | Value |
|--------|-------|
| Total Cycles | 19 |
| IF Utilization | 100.00% |
| ID Utilization | 94.74% |
| EX Utilization | 89.47% |
| MEM Utilization | 84.21% |
| WB Utilization | 78.95% |
| Data Hazard Stalls | 1 |
| Control Hazards | None detected |

---

## Course Context

Built as the final project for **ECE 610: Foundations of Computer Engineering** at the University of Massachusetts Amherst. The project required implementing a cycle-accurate RISC-V pipeline simulator in C++ from scratch, modeling all five pipeline stages, pipeline registers, hazard detection, forwarding, stalling, and branch handling — matching real hardware behavior at the register level.

---

## References

1. RISC-V Foundation. *RISC-V Instruction Set Manual.*
2. Patterson, D. A. & Hennessy, J. L. *Computer Organization and Design: RISC-V Edition.*
3. Saxena, N. et al. *Educational Tools for RISC-V Architecture.*
4. Tanenbaum, A. S. *Structured Computer Organization.*
