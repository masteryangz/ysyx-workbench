package npc

import chisel3._
import chisel3.util._

class IFIO extends Bundle {
    val pc      = Output(UInt(32.W))    // current PC
    val instr   = Output(UInt(32.W))    // fetched instruction
}

class RegFileIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val rs1    = Input(UInt(ADDR_WIDTH.W))
    val rs2    = Input(UInt(ADDR_WIDTH.W))
    val rd     = Input(UInt(ADDR_WIDTH.W))
    val wdata  = Input(UInt(DATA_WIDTH.W))
    val wen    = Input(Bool())
    val rdata1 = Output(UInt(DATA_WIDTH.W))
    val rdata2 = Output(UInt(DATA_WIDTH.W))
}

class DecoderIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val instr   = Input(UInt(DATA_WIDTH.W))
    val wdata   = Input(UInt(DATA_WIDTH.W))
    val wen     = Input(Bool())
    val rdata1  = Output(UInt(DATA_WIDTH.W))
    val rdata2  = Output(UInt(DATA_WIDTH.W))
    //val rd_out  = Output(UInt(ADDR_WIDTH.W)) // forwarding if needed
    val Op      = Output(UInt(7.W))
    val imm     = Output(UInt(DATA_WIDTH.W))
    val funct3  = Output(UInt(3.W))
}

class ALUIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val wdata   = Output(UInt(DATA_WIDTH.W))
    val wen     = Output(Bool())
    val rdata1  = Input(UInt(DATA_WIDTH.W))
    val rdata2  = Input(UInt(DATA_WIDTH.W))
    //val rd_out  = Input(UInt(ADDR_WIDTH.W))
    val Op      = Input(UInt(7.W))
    val imm     = Input(UInt(DATA_WIDTH.W))
    val funct3  = Input(UInt(3.W))
}