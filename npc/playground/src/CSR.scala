package npc

import chisel3._
import chisel3.util._
import chisel3.experimental.IntParam


class CSR(ADDR_WIDTH: Int = 12, DATA_WIDTH: Int = 32) extends Module {
  val io = IO(new CSRIO())

  val mstatus = RegInit(0.U(32.W))
  val mtvec   = RegInit(0.U(32.W))
  val mepc    = RegInit(0.U(32.W))
  val mcause  = RegInit(0.U(32.W))

  // read
  io.rdata := MuxLookup(io.addr, 0.U, Seq(
    "h300".U -> mstatus,
    "h305".U -> mtvec,
    "h341".U -> mepc,
    "h342".U -> mcause
  ))

  // write
  when(io.wen) {
    switch(io.addr) {
      is("h300".U) { mstatus := io.wdata }
      is("h305".U) { mtvec   := io.wdata }
      is("h341".U) { mepc    := io.wdata }
      is("h342".U) { mcause  := io.wdata }
    }
  }

  // ecall
  when(io.ecall) {
    mepc   := io.pc
    mcause := 11.U
  }

  io.nextPC := Mux(io.ecall, mtvec,
               Mux(io.mret, mepc, 0.U))

}
