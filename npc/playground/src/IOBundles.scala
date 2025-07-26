package npc

import chisel3._
import chisel3.util._

class IFIO(memDepth: Int = 256, pcInc: Int = 4, DATA_WIDTH: Int = 32) extends Bundle {
    val pc          = Output(UInt(DATA_WIDTH.W))    // current PC
    val instr       = Output(UInt(DATA_WIDTH.W))    // fetched instruction
    //val trapPulse   = Output(Bool())                // trap pulse for ebreak instruction
    val target      = Input(UInt(DATA_WIDTH.W))     // target address for jump
    val is_jump     = Input(Bool())                 // is jump instruction
}

class RegFileIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val rs1         = Input(UInt(ADDR_WIDTH.W))
    val rs2         = Input(UInt(ADDR_WIDTH.W))
    val rd          = Input(UInt(ADDR_WIDTH.W))
    val wdata       = Input(UInt(DATA_WIDTH.W))
    val wen         = Input(Bool())
    val rdata1      = Output(UInt(DATA_WIDTH.W))
    val rdata2      = Output(UInt(DATA_WIDTH.W))
    val goodTrap    = Output(Bool()) 
}

class DecoderIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val instr           = Input(UInt(DATA_WIDTH.W))
    val wdata           = Input(UInt(DATA_WIDTH.W))
    val wen             = Input(Bool())
    //val In_trapPulse    = Input(Bool())
    val rdata1          = Output(UInt(DATA_WIDTH.W))
    val rdata2          = Output(UInt(DATA_WIDTH.W))
    val Op              = Output(UInt(7.W))
    val imm             = Output(UInt(DATA_WIDTH.W))
    val funct3          = Output(UInt(3.W))
    val goodTrap        = Output(Bool()) 
    //val trapPulse       = Output(Bool())
}

class ALUIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
    val wdata           = Output(UInt(DATA_WIDTH.W))
    val wen             = Output(Bool())
    val is_jump         = Output(Bool())
    val target          = Output(UInt(DATA_WIDTH.W))
    //val trapPulse       = Output(Bool())
    val rdata1          = Input(UInt(DATA_WIDTH.W))
    val rdata2          = Input(UInt(DATA_WIDTH.W))
    val Op              = Input(UInt(7.W))
    val imm             = Input(UInt(DATA_WIDTH.W))
    val funct3          = Input(UInt(3.W))
    val pc              = Input(UInt(DATA_WIDTH.W))
    //val In_trapPulse    = Input(Bool())
}