package npc

import chisel3._
import chisel3.util._

class ALU(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new ALUIO())

    //concatenate together
    val opFunct3 = Cat(io.Op, io.funct3)

    // default
    io.wdata    := 0.U
    io.wen      := false.B

    // case switch
    switch(opFunct3) {
        is("b0010011000".U) {
            io.wdata    := io.rdata1 + io.imm
            io.wen      := true.B
        }
    }

}