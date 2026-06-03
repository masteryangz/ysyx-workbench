package npc

import chisel3._
import chisel3.util._
import parameters._

//class IFUIO(pcInc: Int = 4, DATA_WIDTH: Int = 32) extends Bundle {
class IFUIO extends Bundle {
    val pc          = Output(UInt(DATA_WIDTH.W))    // current PC
    val instr       = Output(UInt(DATA_WIDTH.W))    // fetched instruction
    val rdata       = Output(UInt(DATA_WIDTH.W))    // read data (for store instructions)
    val target      = Input(UInt(DATA_WIDTH.W))     // target address for jump
    val addr        = Input(UInt(DATA_WIDTH.W))
    val wdata       = Input(UInt(DATA_WIDTH.W))
    val is_jump     = Input(Bool())                 // is jump instruction
    val valid       = Input(Bool())
    val wen         = Input(Bool())
    val clock       = Input(Clock())
    val reset       = Input(Bool())
    val wmask       = Input(UInt(4.W))
}

//class RegFileIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
class RegFileIO extends Bundle {
    val pc          = Input(UInt(DATA_WIDTH.W))    // current PC
    val rs1         = Input(UInt(ADDR_WIDTH.W))
    val rs2         = Input(UInt(ADDR_WIDTH.W))
    val rd          = Input(UInt(ADDR_WIDTH.W))
    val wdata       = Input(UInt(DATA_WIDTH.W))
    val wen         = Input(Bool())
    val rdata1      = Output(UInt(DATA_WIDTH.W))
    val rdata2      = Output(UInt(DATA_WIDTH.W))
    val R10         = Output(UInt(DATA_WIDTH.W))    // expose R10 for DPI use
    //val goodTrap    = Output(Bool()) 
}

//class IDUIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
class IDUIO extends Bundle {
    val pc              = Input(UInt(DATA_WIDTH.W)) // current PC
    val instr           = Input(UInt(DATA_WIDTH.W))
    val wdata           = Input(UInt(DATA_WIDTH.W))
    val rdata           = Input(UInt(DATA_WIDTH.W))
    val raddr           = Input(UInt(DATA_WIDTH.W))
    val rwen            = Input(Bool())
    val valid           = Input(Bool())
    val rdata1          = Output(UInt(DATA_WIDTH.W))
    val rdata2          = Output(UInt(DATA_WIDTH.W))
    val Op              = Output(UInt(7.W))
    val imm             = Output(UInt(DATA_WIDTH.W))
    val funct3          = Output(UInt(3.W))
    val funct7          = Output(UInt(7.W))
    //val goodTrap        = Output(Bool())
    val wmask           = Output(UInt(4.W))
    val R10             = Output(UInt(DATA_WIDTH.W)) // pass R10 to IDIO for DPI use
}

//class CSRIO(ADDR_WIDTH: Int = 12, DATA_WIDTH: Int = 32) extends Bundle {
class CSRIO extends Bundle {
    val pc          = Input(UInt(DATA_WIDTH.W))    // current PC
    val addr        = Input(UInt(ADDR_WIDTH.W))
    val wdata       = Input(UInt(DATA_WIDTH.W))
    val rdata       = Output(UInt(DATA_WIDTH.W))
    val wen         = Input(Bool())
    val ecall        = Input(Bool())
    val mret         = Input(Bool())
    val nextPC       = Output(UInt(DATA_WIDTH.W))
}

//class EXUIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
class EXUIO extends Bundle {
    val wdata           = Output(UInt(DATA_WIDTH.W))
    val addr            = Output(UInt(DATA_WIDTH.W))
    val rwen            = Output(Bool())
    val is_jump         = Output(Bool())
    val target          = Output(UInt(DATA_WIDTH.W))
    val valid           = Output(Bool())
    val mwen            = Output(Bool())
    val rdata1          = Input(UInt(DATA_WIDTH.W))
    val rdata2          = Input(UInt(DATA_WIDTH.W))
    val Op              = Input(UInt(7.W))
    val imm             = Input(UInt(DATA_WIDTH.W))
    val funct3          = Input(UInt(3.W))
    val funct7          = Input(UInt(7.W))
    val pc              = Input(UInt(DATA_WIDTH.W))
    val R10             = Input(UInt(DATA_WIDTH.W)) // pass R10 to EX for DPI use
}

//class adderIO(DATA_WIDTH: Int = 32) extends Bundle {
class adderIO extends Bundle {
    val add1   = Input(UInt(DATA_WIDTH.W))
    val add2   = Input(UInt(DATA_WIDTH.W))
    val result = Output(UInt(DATA_WIDTH.W))
}
