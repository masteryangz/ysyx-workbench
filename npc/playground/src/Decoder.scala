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
    regfile.io.rs1      := rs1
    regfile.io.rs2      := rs2
    regfile.io.rd       := rd
    regfile.io.wdata    := io.wdata
    regfile.io.wen      := io.wen
    regfile.io.pc       := io.pc

    // Outputs
    //io.trapPulse    := io.In_trapPulse
    io.rdata1       := regfile.io.rdata1
    io.rdata2       := regfile.io.rdata2
    io.Op           := io.instr(6, 0)
    io.funct3       := io.instr(14, 12)
    io.imm          := 0.U
    io.goodTrap     := regfile.io.goodTrap
    switch(io.Op) {
        is("b0010011".U) {
            io.imm := io.instr(DATA_WIDTH-1, 20).asSInt.pad(DATA_WIDTH).asUInt
        }
        is(BitPat("b0?10111").value.asUInt) {
            io.imm := (io.instr(DATA_WIDTH-1, 12).asSInt.pad(DATA_WIDTH).asUInt << 12)(DATA_WIDTH-1, 0)
        }
        is("b1101111".U) {
            io.imm := Cat(io.instr(DATA_WIDTH-1), io.instr(19, 12), io.instr(20), io.instr(30, 21)).asSInt.pad(DATA_WIDTH).asUInt << 1
        }
    }
}
