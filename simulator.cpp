#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>

// Constants
const int NUM_REGISTERS = 32;
const int MEM_SIZE = 2048;
const std::string NOP = "NOP";

// Instruction structure
struct Instruction {
    std::string type;
    int src1, src2;
    int dest;
    int imm;
    std::string hex;
};

// Pipeline stage structure
struct PipelineStage {
    Instruction instruction;
    bool stalled = false;
};

// CPU Simulator
class RiscVSimulator {
private:
    std::vector<Instruction> instructions;
    PipelineStage pipeline[5];
    uint32_t PC = 0;
    uint32_t registers[NUM_REGISTERS] = {0};
    uint8_t memory[MEM_SIZE] = {0};
    int cycleCount = 0;
    int executedInstructions = 0;
    int stageActiveCycles[5] = {0};

    // Decode hex to instruction
    Instruction decode(const std::st
