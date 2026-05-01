#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

const int NUM_REGISTERS = 32;
const int MEM_SIZE = 2048;
const std::string NOP = "NOP";

struct Instruction {
    std::string type;
    int src1, src2, dest;
    std::string hex;
};

struct PipelineRegister {
    Instruction IR;
};

// Simplified RISC-V Pipeline Simulator
// Shows instruction flow through all 5 stages cycle by cycle.
// Does not perform register execution — use simulator.cpp for full simulation.
class RiscVSimulator {
private:
    std::vector<Instruction> instructions;
    PipelineRegister pipeline[5];
    uint32_t PC = 0;
    int cycleCount = 0;

    void fetch() {
        if (PC / 4 < instructions.size()) {
            pipeline[0].IR = instructions[PC / 4];
            PC += 4;
        } else {
            pipeline[0].IR = {NOP, 0, 0, 0, NOP};
        }
    }

    void decode()       { pipeline[1].IR = pipeline[0].IR; }
    void execute()      { pipeline[2].IR = pipeline[1].IR; }
    void memoryAccess() { pipeline[3].IR = pipeline[2].IR; }
    void writeBack()    { pipeline[4].IR = pipeline[3].IR; }

    void printPipelineState() {
        std::cout << "Cycle " << cycleCount << ":\n";
        const char* stageNames[] = {"Fetch", "Decode", "Execute", "Memory", "WriteBack"};
        for (int i = 4; i >= 0; --i) {
            std::cout << "  " << std::setw(10) << stageNames[i] << ": "
                      << pipeline[i].IR.type;
            if (pipeline[i].IR.type != NOP)
                std::cout << "  " << pipeline[i].IR.hex;
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    bool pipelineIsEmpty() {
        for (auto& stage : pipeline)
            if (stage.IR.type != NOP) return false;
        return true;
    }

public:
    RiscVSimulator(std::vector<Instruction> instrs) : instructions(instrs) {
        for (auto& stage : pipeline)
            stage.IR = {NOP, 0, 0, 0, NOP};
    }

    void simulate() {
        while (PC / 4 < instructions.size() || !pipelineIsEmpty()) {
            printPipelineState();
            cycleCount++;
            writeBack();
            memoryAccess();
            execute();
            decode();
            fetch();
        }
    }
};

int main() {
    std::vector<Instruction> instructions = {
        {"ADD",  1, 2, 3, "00010133"},
        {"SUB",  3, 4, 5, "40010133"},
        {"ADDI", 1, 0, 2, "00210113"},
        {"MUL",  5, 6, 4, "00B20233"},
        {"LW",   4, 0, 6, "00020303"},
        {"SW",   4, 6, 0, "00420323"},
        {"BEQ",  5, 6, 0, "00828263"}
    };

    RiscVSimulator simulator(instructions);
    simulator.simulate();
    return 0;
}
