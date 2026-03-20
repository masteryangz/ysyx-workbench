package npc

import chisel3._
import chisel3.util._

class EX(pcInc: Int = 4, ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new EXIO())
    val exdpi = Module(new EXdpi())
    val CSR = Module(new CSR(ADDR_WIDTH, DATA_WIDTH))

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
    exdpi.io.isEbreak := false.B
    exdpi.io.pc       := io.pc
    exdpi.io.R10      := io.R10 // pass R10 to DPI
    CSR.io.pc        := io.pc
    CSR.io.addr      := io.imm(11,0) // use imm as addr
    CSR.io.wdata     := io.rdata2
    CSR.io.wen       := false.B
    CSR.io.ecall     := false.B
    CSR.io.mret      := false.B

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
                is("b101".U) { // SRLI, SRAI
                    switch(io.funct7) {
                        is("b0000000".U) { // SRLI
                            io.wdata    := (io.rdata1 >> io.imm(4,0))(DATA_WIDTH-1,0)
                            io.rwen     := true.B
                        }
                        is("b0100000".U) { // SRAI
                            io.wdata    := (io.rdata1.asSInt >> io.imm(4,0)).asUInt
                            io.rwen     := true.B
                        }
                    }
                }
                is("b001".U) { // SLLI
                    io.wdata    := (io.rdata1 << io.imm(4,0))(DATA_WIDTH-1,0)
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
        is("b1110011".U) {
            switch(io.pc) {
                is("b00000000000000000000000001110011".U) { // ECALL
                    CSR.io.ecall        := true.B
                }
                is("b00000000000100000000000001110011".U) { // EBREAK
                    exdpi.io.isEbreak   := true.B
                }
                is("b00010000001000000000000001110011".U) { // MRET
                    CSR.io.mret         := true.B
                }
            }
            switch(io.funct3) {
                is("b001".U) { // CSRRW
                    CSR.io.wen       := true.B
                }
                is("b010".U) { // CSRRS
                    CSR.io.wen       := true.B
                    CSR.io.wdata     := io.rdata1 | io.rdata2
                }
                is("b011".U) { // CSRRC
                    CSR.io.wen       := true.B
                    CSR.io.wdata     := (~io.rdata1).asUInt & io.rdata2
                }
            }
        }
        // S-type
        is("b0100011".U) {
            switch(io.funct3) {
                is("b000".U) { // SB
                    io.mwen         := true.B
                    io.valid        := true.B
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.wdata        := Cat(Fill(24, io.rdata2(7)), io.rdata2(7,0))
                }
                is("b001".U) { // SH
                    io.mwen         := true.B
                    io.valid        := true.B
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.wdata        := Cat(Fill(16, io.rdata2(15)), io.rdata2(15,0))
                }
                is("b010".U) { // SW
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.wdata        := io.rdata2
                    io.valid        := true.B
                    io.mwen         := true.B
                }
            }
        }
        // L
        is("b0000011".U) {
            switch(io.funct3) {
                is("b000".U) { // LB
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.valid        := true.B
                    io.rwen         := true.B
                }
                is("b001".U) { // LH
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.valid        := true.B
                    io.rwen         := true.B
                }
                is("b010".U) { // LW
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.valid        := true.B
                    io.rwen         := true.B
                }
                is("b100".U) { // LBU
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.valid        := true.B
                    io.rwen         := true.B
                }
                is("b101".U) { // LHU
                    io.addr         := adder1.io.result
                    adder1.io.add1  := io.rdata1
                    adder1.io.add2  := io.imm
                    io.valid        := true.B
                    io.rwen         := true.B
                }
            }
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
                is("b0011000".U) { // SLT
                    io.wdata    := (io.rdata1.asSInt < io.rdata2.asSInt).asUInt
                    io.rwen     := true.B
                }
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
                is("b0000001000".U) { // MUL
                    io.wdata    := (io.rdata1.asSInt * io.rdata2.asSInt)(DATA_WIDTH-1,0).asUInt
                    io.rwen     := true.B
                }
                is("b0000001001".U) { // MULH
                    io.wdata    := (io.rdata1.asSInt * io.rdata2.asSInt)(2*DATA_WIDTH-1,DATA_WIDTH).asUInt
                    io.rwen     := true.B
                }
                is("b0000001100".U) { // DIV
                    when(io.rdata2 === 0.U) {
                        io.wdata := (-1).S(DATA_WIDTH.W).asUInt
                    } .elsewhen(io.rdata1 === (-1).S(DATA_WIDTH.W).asUInt && io.rdata2 === (-1).S(DATA_WIDTH.W).asUInt) {
                        io.wdata := io.rdata1
                    } .otherwise {
                        io.wdata := (io.rdata1.asSInt / io.rdata2.asSInt).asUInt
                    }
                    io.rwen     := true.B
                }
                is("b0000001101".U) { // DIVU
                    when(io.rdata2 === 0.U) {
                        io.wdata := (-1).S(DATA_WIDTH.W).asUInt
                    } .otherwise {
                        io.wdata := (io.rdata1.asUInt / io.rdata2.asUInt).asUInt
                    }
                    io.rwen     := true.B
                }
                is("b0000001110".U) { // REM
                    when(io.rdata2 === 0.U) {
                        io.wdata := io.rdata1
                    } .elsewhen(io.rdata1 === (-1).S(DATA_WIDTH.W).asUInt && io.rdata2 === (-1).S(DATA_WIDTH.W).asUInt) {
                        io.wdata := 0.U
                    } .otherwise {
                        io.wdata := (io.rdata1.asSInt % io.rdata2.asSInt).asUInt
                    }
                    io.rwen     := true.B
                }
                is("b0000001111".U) { // REMU
                    when(io.rdata2 === 0.U) {
                        io.wdata := io.rdata1
                    } .otherwise {
                        io.wdata := (io.rdata1.asUInt % io.rdata2.asUInt).asUInt
                    }
                    io.rwen     := true.B
                }
            }
        }
    }

}