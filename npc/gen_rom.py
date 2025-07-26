with open("rom.txt", "w") as f:
    f.write("00100013\n")   # addi x0, x0, 1
    f.write("7FF00513\n")   # addi x10, x0, 2047
    f.write("7FF50513\n")   # addi x10, x10, 2047
    f.write("7FF50513\n")   # addi x10, x10, 2047
    f.write("7FF50513\n")   # addi x10, x10, 2047
    f.write("00100013\n")   # addi x0, x0, 1
    f.write("002500E7\n")   # jalr x1, x10, 2
    f.write("00100513\n")   # addi x10, x0, 1
    f.write("00100513\n")   # addi x10, x0, 1
    f.write("00100013\n")   # addi x0, x0, 1
    f.write("00100013\n")   # addi x0, x0, 1
    f.write("00100073\n")   # ebreak
    f.write("FE100513\n")   # addi x10, x0, -1
    f.write("FE100513\n")   # addi x10, x0, -1
    f.write("00000000\n")   # nop
    