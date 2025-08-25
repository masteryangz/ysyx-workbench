package npc

import chisel3._
import chisel3.util._

class ALU(pcInc: Int = 4, ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new ALUIO())

    val adder1 = Module(new adder(DATA_WIDTH))
    val adder2 = Module(new adder(DATA_WIDTH))

    //concatenate together
    val Funct = Cat(io.funct3, io.funct7)

    // default
    io.wdata        := 0.U
    io.addr         := 0.U
    io.rwen         := false.B
    io.is_jump      := false.B
    io.target       := 0.U
    adder1.io.add1  := 0.U
    adder1.io.add2  := 0.U
    adder2.io.add1  := 0.U
    adder2.io.add2  := 0.U
    io.valid        := false.B
    io.mwen         := false.B

    // case switch
    switch(io.Op) {
        is("b0010011".U) {
            switch(io.funct3) {
                is("b000".U) { // ADDI
                    io.wdata        := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
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
        // JALR
        is("b1100111".U) {
            io.wdata        := adder2.io.result
            adder2.io.add1  := io.pc
            adder2.io.add2  := pcInc.U
            io.rwen         := true.B
            io.is_jump      := true.B
            io.target       := adder1.io.result & ~1.U(DATA_WIDTH.W)
            adder1.io.add1  := io.rdata1
            adder1.io.add2  := io.imm
        }
        // AUIPC
        is("b0010111".U) {
            io.wdata        := adder1.io.result
            adder1.io.add1  := io.pc
            adder1.io.add2  := io.imm
            io.rwen         := true.B
        }
        // LUI
        is("b0110111".U) {
            io.wdata    := io.imm
            io.rwen     := true.B
        }
        // JAL
        is("b1101111".U) {
            io.wdata        := adder2.io.result
            adder2.io.add1  := io.pc
            adder2.io.add2  := pcInc.U
            io.rwen         := true.B
            io.is_jump      := true.B
            io.target       := adder1.io.result
            adder1.io.add1  := io.pc
            adder1.io.add2  := io.imm 
        }
        // S-type
        is("b0100011".U) {
            io.addr         := adder1.io.result
            adder1.io.add1  := io.rdata2
            adder1.io.add2  := io.imm
            io.wdata        := io.rdata1
            io.valid        := true.B
            io.mwen         := true.B
        }
        // L
        is("b0000011".U) {
            io.addr         := adder1.io.result
            adder1.io.add1  := io.rdata1
            adder1.io.add2  := io.imm
            io.valid        := true.B
            io.rwen         := true.B
        }
        // B-type
        is("b1100011".U) {
            switch(io.funct3) {
                is("b000".U) { // BEQ
                    when(io.rdata1 === io.rdata2) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
                is("b001".U) { // BNE
                    when(io.rdata1 =/= io.rdata2) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
                is("b100".U) { // BLT
                    when(io.rdata1.asSInt < io.rdata2.asSInt) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
                is("b101".U) { // BGE
                    when(io.rdata1.asSInt >= io.rdata2.asSInt) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
                is("b110".U) { // BLTU
                    when(io.rdata1 < io.rdata2) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
                is("b111".U) { // BGEU
                    when(io.rdata1 >= io.rdata2) {
                        io.is_jump  := true.B
                        io.target   := adder1.io.result
                        adder1.io.add1  := io.pc
                        adder1.io.add2  := io.imm
                    }
                }
            }
        }
        is("b0110011".U) {
            switch(Funct) {
                is("b0000000000".U) { // ADD
                    io.wdata        := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.rdata2
                    io.rwen         := true.B
                }
                is("b0000100000".U) { // SUB
                    io.wdata        := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := (~io.rdata2).asUInt + 1.U
                    io.rwen         := true.B
                }
                is("b0010000000".U) { // SLL
                    io.wdata    := (io.rdata1 << io.rdata2(4,0))(DATA_WIDTH-1,0)
                    io.rwen     := true.B
                }
                /*
                is("b0011000".U) { // SLT
                    io.wdata    := (io.rdata1.asSInt < io.rdata2.asSInt).asUInt
                    io.rwen     := true.B
                }
                */
                is("b0110000000".U) { // SLTU
                    io.wdata    := (io.rdata1 < io.rdata2).asUInt
                    io.rwen     := true.B
                }
                is("b1000000000".U) { // XOR
                    io.wdata    := io.rdata1 ^ io.rdata2
                    io.rwen     := true.B
                }
                is("b1010000000".U) { // SRL
                    io.wdata    := (io.rdata1 >> io.rdata2(4,0))(DATA_WIDTH-1,0)
                    io.rwen     := true.B
                }
                is("b1010100000".U) { // SRA
                    io.wdata    := (io.rdata1.asSInt >> io.rdata2(4,0)).asUInt
                    io.rwen     := true.B
                }
                is("b1100000000".U) { // OR
                    io.wdata    := io.rdata1 | io.rdata2
                    io.rwen     := true.B
                }
                is("b1110000000".U) { // AND
                    io.wdata    := io.rdata1 & io.rdata2
                    io.rwen     := true.B
                }
            }
        }
    }

}