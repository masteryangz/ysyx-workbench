package npc

import chisel3._
import chisel3.util._

class Top(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
  /*
  val io = IO(new Bundle {
    val pcOut   = Output(UInt(DATA_WIDTH.W))  // expose pc
    val imm     = Output(UInt(DATA_WIDTH.W))  // expose dec imm
  })
  */

  // create new module
  val instrfet  = Module(new IF())
  val decoder   = Module(new Decoder())
  val alu       = Module(new ALU())

  // connect
  decoder.io.instr  := instrfet.io.instr
  decoder.io.wdata  := alu.io.wdata
  decoder.io.wen    := alu.io.wen
  alu.io.rdata1     := decoder.io.rdata1
  alu.io.rdata2     := decoder.io.rdata2
  alu.io.Op         := decoder.io.Op
  alu.io.funct3     := decoder.io.funct3
  alu.io.imm        := decoder.io.imm

  dontTouch(instrfet.io)
  dontTouch(decoder.io)
  dontTouch(alu.io)
}
