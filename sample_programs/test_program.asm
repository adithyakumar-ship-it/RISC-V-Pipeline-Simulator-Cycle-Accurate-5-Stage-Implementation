# RISC-V Test Program — used as input for the pipeline simulator
# Each instruction below corresponds to a hex value in simulator.cpp

lui   x1,  0x10000        # Load upper immediate
addi  x2,  x1,  42        # x2 = x1 + 42
addi  x3,  x2,  -12       # x3 = x2 - 12  (subi equivalent)
add   x4,  x2,  x3        # x4 = x2 + x3
sub   x5,  x4,  x2        # x5 = x4 - x2
mul   x6,  x2,  x3        # x6 = lower 32 bits of x2 * x3
                           # x7 = upper 32 bits of x2 * x3
div   x8,  x6,  x2        # x8 = x6 / x2
rem   x9,  x6,  x2        # x9 = x6 % x2
andi  x10, x6,  0x0F      # x10 = x6 & 0x0F
ori   x11, x6,  0xF0      # x11 = x6 | 0xF0
and   x12, x10, x11       # x12 = x10 & x11
or    x13, x10, x11       # x13 = x10 | x11
sll   x14, x13, 2         # x14 = x13 << 2
srl   x15, x14, 1         # x15 = x14 >> 1
slti  x16, x15, 100       # x16 = (x15 < 100) ? 1 : 0
sltiu x17, x15, 100       # x17 = (x15 <u 100) ? 1 : 0
sw    x2,  0(x0)          # memory[0] = x2
lw    x18, 0(x0)          # x18 = memory[0]
beq   x18, x2,  label     # if x18 == x2, branch to label
