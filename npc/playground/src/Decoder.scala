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
    val result = WireDefault(0.U(DATA_WIDTH.W))   // ✅ wire with default value 0

    // Connect RegFile
    regfile.io.rs1      := rs1
    regfile.io.rs2      := rs2
    regfile.io.rd       := rd
    regfile.io.wdata    := Mux(io.valid, result, io.wdata)
    regfile.io.wen      := io.rwen
    regfile.io.pc       := io.pc

    // Outputs
    io.rdata1       := regfile.io.rdata1
    io.rdata2       := regfile.io.rdata2
    io.Op           := io.instr(6, 0)
    io.funct3       := io.instr(14, 12)
    io.funct7       := io.instr(31, 25)
    io.imm          := 0.U
    io.goodTrap     := regfile.io.goodTrap

    // Byte / halfword selection
    val byteShifted  = (io.rdata >> (io.raddr(1,0) << 3)).asUInt  // pick byte
    val halfShifted  = (io.rdata >> (io.raddr(1)   << 4)).asUInt  // pick halfword
    val wbyteSelected = byteShifted(7,0)
    val whalfSelected = halfShifted(15,0)
    val lbyteSelected = io.rdata(7,0)
    val lhalfSelected = io.rdata(15,0)

    switch(io.funct3) {
        is("b000".U) { // LB
            result := Cat(Fill(24, lbyteSelected(7)), lbyteSelected)
        }
        is("b001".U) { // LH
            result := Cat(Fill(16, lhalfSelected(15)), lhalfSelected)
        }
        is("b010".U) { // LW
            result := io.rdata
        }
        is("b100".U) { // LBU
            result := Cat(0.U(24.W), lbyteSelected)
        }
        is("b101".U) { // LHU
            result := Cat(0.U(16.W), lhalfSelected)
        }
    }

    switch(io.Op) {
        // I-type
        is(Seq("b0010011".U, "b0000011".U, "b1100111".U)) {
            io.imm := io.instr(DATA_WIDTH-1, 20).asSInt.pad(DATA_WIDTH).asUInt
        }
        // U-type
        is(Seq("b0010111".U, "b0110111".U)) {
            io.imm := (io.instr(DATA_WIDTH-1, 12).asSInt.pad(DATA_WIDTH).asUInt << 12)(DATA_WIDTH-1, 0)
        }
        // J-type
        is("b1101111".U) {
            io.imm := Cat(io.instr(DATA_WIDTH-1), io.instr(19, 12), io.instr(20), io.instr(30, 21)).asSInt.pad(DATA_WIDTH).asUInt << 1
        }
        // S-type
        is("b0100011".U) {
            io.imm := Cat(io.instr(DATA_WIDTH-1, 25), io.instr(11, 7)).asSInt.pad(DATA_WIDTH).asUInt
        }
        // R-type
        is("b0110011".U) {
            io.imm := 0.U
        }
        // B-type
        is("b1100011".U) {
            io.imm := Cat(io.instr(DATA_WIDTH-1), io.instr(7), io.instr(30, 25), io.instr(11, 8)).asSInt.pad(DATA_WIDTH).asUInt << 1
        }
    }
}
