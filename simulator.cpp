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
    Instruction decode(const std::string& hex) {
        static const std::unordered_map<std::string, Instruction> instructionSet = {
            {"00010133", {"ADD",   1, 0, 2, 0,          "00010133"}},
            {"40010133", {"SUB",   1, 0, 2, 0,          "40010133"}},
            {"00210113", {"ADDI",  1, 0, 2, 2,          "00210113"}},
            {"00218193", {"SUBI",  3, 0, 3, 2,          "00218193"}},
            {"00B20233", {"MUL",   5, 6, 4, 0,          "00B20233"}},
            {"00B203B3", {"MULU",  5, 6, 7, 0,          "00B203B3"}},
            {"40020233", {"DIV",   5, 6, 4, 0,          "40020233"}},
            {"00B282B3", {"REM",   5, 6, 5, 0,          "00B282B3"}},
            {"00020303", {"LW",    4, 0, 6, 0,          "00020303"}},
            {"00420323", {"SW",    4, 6, 0, 4,          "00420323"}},
            {"00828263", {"BEQ",   5, 6, 0, 8,          "00828263"}},
            {"000202B7", {"LUI",   0, 0, 5, 0x00020000, "000202B7"}},
            {"00028293", {"AND",   5, 6, 5, 0,          "00028293"}},
            {"000282E3", {"OR",    5, 6, 5, 0,          "000282E3"}},
            {"00028313", {"XOR",   5, 6, 5, 0,          "00028313"}},
            {"00120313", {"SLL",   4, 1, 6, 0,          "00120313"}},
            {"00120393", {"SRL",   4, 1, 7, 0,          "00120393"}},
            {"00229313", {"SLTI",  5, 0, 6, 2,          "00229313"}},
            {"002293B3", {"SLTIU", 5, 0, 7, 2,          "002293B3"}},
        };

        if (instructionSet.find(hex) != instructionSet.end()) {
            return instructionSet.at(hex);
        }
        return {NOP, 0, 0, 0, 0, hex};
    }

    // Execute an instruction
    void executeInstruction(const Instruction& instr) {
        if (instr.type == "ADD") {
            registers[instr.dest] = registers[instr.src1] + registers[instr.src2];
        } else if (instr.type == "SUB") {
            registers[instr.dest] = registers[instr.src1] - registers[instr.src2];
        } else if (instr.type == "ADDI") {
            registers[instr.dest] = registers[instr.src1] + instr.imm;
        } else if (instr.type == "MUL") {
            registers[instr.dest] = registers[instr.src1] * registers[instr.src2];
        } else if (instr.type == "DIV") {
            if (registers[instr.src2] != 0)
                registers[instr.dest] = registers[instr.src1] / registers[instr.src2];
        } else if (instr.type == "REM") {
            if (registers[instr.src2] != 0)
                registers[instr.dest] = registers[instr.src1] % registers[instr.src2];
        } else if (instr.type == "LW") {
            registers[instr.dest] = memory[registers[instr.src1] + instr.imm];
        } else if (instr.type == "SW") {
            memory[registers[instr.src1] + instr.imm] = registers[instr.src2];
        } else if (instr.type == "AND") {
            registers[instr.dest] = registers[instr.src1] & registers[instr.src2];
        } else if (instr.type == "OR") {
            registers[instr.dest] = registers[instr.src1] | registers[instr.src2];
        } else if (instr.type == "SLTI") {
            registers[instr.dest] = registers[instr.src1] < instr.imm ? 1 : 0;
        } else if (instr.type == "SLTIU") {
            registers[instr.dest] = static_cast<uint32_t>(registers[instr.src1]) 
                                    static_cast<uint32_t>(instr.imm) ? 1 : 0;
        }
    }

    // Print pipeline state
    void printPipelineState() {
        std::cout << "\n=== Cycle " << cycleCount << " ===\n";
        const char* stageNames[] = {"IF", "ID", "EX", "MEM", "WB"};
        for (int i = 4; i >= 0; --i) {
            std::cout << std::setw(8) << stageNames[i] << ": ";
            const auto& instr = pipeline[i].instruction;
            if (instr.type != NOP)
                std::cout << std::setw(5) << instr.type << "  (" << instr.hex << ")";
            else
                std::cout << "NOP";
            std::cout << "\n";
        }

        // Print non-zero registers only
        std::cout << "\nRegister Values:\n";
        for (int i = 0; i < NUM_REGISTERS; ++i) {
            if (registers[i] != 0)
                std::cout << "x" << i << ": " << registers[i] << "  ";
        }
        std::cout << "\n";
    }

    // Print final statistics
    void calculateAndPrintStatistics() {
        std::cout << "\n=== Simulation Statistics ===\n";
        const char* stageNames[] = {"IF", "ID", "EX", "MEM", "WB"};
        for (int i = 0; i < 5; ++i) {
            double utilization = (static_cast<double>(stageActiveCycles[i]) / cycleCount) * 100;
            std::cout << stageNames[i] << " Utilization: "
                      << std::fixed << std::setprecision(2) << utilization << "%\n";
        }
        std::cout << "Total Cycles: " << cycleCount << "\n";
    }

public:
    RiscVSimulator() {
        // Instructions loaded as 32-bit hex.
        // To use a custom program, replace this vector with your own
        // hex-encoded RISC-V instructions.
        std::vector<std::string> hexInstructions = {
            "00010133", "40010133", "00210113", "00218193",
            "00B20233", "00B203B3", "40020233", "00B282B3",
            "00020303", "00420323", "00828263", "000202B7",
            "00028293", "000282E3", "00028313", "00120313",
            "00120393", "00229313", "002293B3"
        };
        for (const auto& hex : hexInstructions)
            instructions.push_back(decode(hex));

        // Initialize pipeline with NOPs
        for (auto& stage : pipeline)
            stage.instruction = {NOP, 0, 0, 0, 0, ""};
    }

    void runCycle() {
        // Track active cycles per stage
        for (int i = 0; i < 5; ++i) {
            if (pipeline[i].instruction.type != NOP)
                stageActiveCycles[i]++;
        }

        // Advance pipeline: WB <- MEM <- EX <- ID <- IF
        pipeline[4] = pipeline[3];
        pipeline[3] = pipeline[2];

        if (pipeline[2].instruction.type != NOP)
            executeInstruction(pipeline[2].instruction);
        pipeline[2] = pipeline[1];

        if (!pipeline[1].stalled)
            pipeline[1] = pipeline[0];

        // Fetch next instruction
        if (PC / 4 < instructions.size()) {
            pipeline[0].instruction = instructions[PC / 4];
            PC += 4;
        } else {
            pipeline[0].instruction = {NOP, 0, 0, 0, 0, ""};
        }

        cycleCount++;
        printPipelineState();
    }

    void runSimulation() {
        while (true) {
            std::string mode;
            int steps;
            std::cout << "\nSelect mode (instruction/cycle): ";
            std::cin >> mode;
            std::cout << "Enter number of "
                      << (mode == "instruction" ? "instructions" : "cycles")
                      << ": ";
            std::cin >> steps;

            for (int i = 0; i < steps; ++i)
                runCycle();

            char choice;
            std::cout << "\nContinue simulation? (y/n): ";
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') break;
        }
        calculateAndPrintStatistics();
    }
};

int main() {
    RiscVSimulator simulator;
    simulator.runSimulation();
    return 0;
}
