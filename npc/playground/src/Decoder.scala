package npc

import chisel3._
import chisel3.util._

class Decoder(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new DecoderIO())

    // Parse instruction
    val rs1       = io.instr(19, 15)
    val rs2       = io.instr(24, 20)
    val rd        = io.instr(11, 7)


    val regfile = Module(new RegFile())

    // Connect RegFile
    regfile.io.rs1 := rs1
    regfile.io.rs2 := rs2
    regfile.io.rd  := rd
    regfile.io.wdata := io.wdata
    regfile.io.wen := io.wen

    // Outputs
    io.rdata1   := regfile.io.rdata1
    io.rdata2   := regfile.io.rdata2
    io.Op       := io.instr(6, 0)
    io.funct3   := io.instr(14, 12)
    io.imm      := 0.U
    switch(io.Op) {
        is("b0010011".U) {
            io.imm := io.instr(31, 20).asSInt.asUInt
        }
    }
}
