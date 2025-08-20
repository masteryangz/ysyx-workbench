package npc

import chisel3._
import chisel3.util._

class ALU(pcInc: Int = 4, ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new ALUIO())

    val adder = Module(new adder(DATA_WIDTH))

    //concatenate together
    val opFunct3 = Cat(io.Op, io.funct3)

    // default
    io.wdata        := 0.U
    io.rwen         := false.B
    io.is_jump      := false.B
    io.target       := 0.U
    adder.io.add1   := 0.U
    adder.io.add2   := 0.U
    io.valid        := false.B
    io.mwen         := false.B

    // case switch
    switch(io.Op) {
        is("b0010011".U) {
            switch(io.funct3) {
                is("b000".U) { // ADDI
                    //io.wdata    := io.rdata1 + io.imm
                    io.wdata        := adder.io.result
                    adder.io.add1   := io.rdata1
                    adder.io.add2   := io.imm
                    io.rwen         := true.B
                }
                is("b010".U) { // SLTI
                    io.wdata    := (io.rdata1.asSInt < io.imm.asSInt).asUInt
                    io.rwen     := true.B
                }
                is("b011".U) { // SLTIU
                    io.wdata    := (io.rdata1 < io.imm).asUInt
                    io.rwen     := true.B
                }
                is("b100".U) { // XORI
                    io.wdata    := io.rdata1 ^ io.imm
                    io.rwen     := true.B
                }
                is("b110".U) { // ORI
                    io.wdata    := io.rdata1 | io.imm
                    io.rwen     := true.B
                }
                is("b111".U) { // ANDI
                    io.wdata    := io.rdata1 & io.imm
                    io.rwen     := true.B
                }
            }
        }
        is("b1100111".U) {
            //io.wdata    := io.pc + pcInc.U
            io.wdata        := adder.io.result
            adder.io.add1   := io.pc
            adder.io.add2   := pcInc.U
            io.rwen         := true.B
            io.is_jump      := true.B
            io.target       := (io.rdata1 + io.imm) & ~1.U(DATA_WIDTH.W)
            //printf("io.target = 0x%x\n", io.target)
            //printf("io.rdata1 + io.imm = 0x%x\n", io.rdata1 + io.imm)
            //printf("~0x1.U = 0x%x\n", ~1.U(DATA_WIDTH.W))
            //printf("(io.rdata1 + io.imm) & ~0x1.U = 0x%x\n", (io.rdata1 + io.imm) & ~1.U(DATA_WIDTH.W))
        }
        is("b0010111".U) {
            //io.wdata    := io.pc + io.imm
            io.wdata        := adder.io.result
            adder.io.add1   := io.pc
            adder.io.add2   := io.imm
            io.rwen         := true.B
        }
        is("b0110111".U) {
            io.wdata    := io.imm
            io.rwen     := true.B
        }
        is("b1101111".U) {
            //io.wdata    := io.pc + pcInc.U
            io.wdata        := adder.io.result
            adder.io.add1   := io.pc
            adder.io.add2   := pcInc.U
            io.rwen         := true.B
            io.is_jump      := true.B
            io.target       := io.pc + io.imm
        }
    }

}