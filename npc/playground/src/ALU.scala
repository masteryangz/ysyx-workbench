package npc

import chisel3._
import chisel3.util._

class ALU(pcInc: Int = 4, ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new ALUIO())

    //concatenate together
    val opFunct3 = Cat(io.Op, io.funct3)

    // default
    io.wdata    := 0.U
    io.wen      := false.B
    io.is_jump  := false.B
    io.target   := 0.U

    // case switch
    switch(opFunct3) {
        is("b0010011000".U) {
            io.wdata    := io.rdata1 + io.imm
            io.wen      := true.B
        }
        is("b1100111000".U) {
            io.wdata    := io.pc + pcInc.U
            io.wen      := true.B
            io.is_jump  := true.B
            io.target   := (io.rdata1 + io.imm) & ~0x1.U
        }
    }

}