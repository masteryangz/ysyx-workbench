package npc

import chisel3._
import chisel3.util._
import chisel3.experimental.IntParam


class RegFile(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
  val io = IO(new RegFileIO())

  val rf = RegInit(VecInit(Seq.fill(1 << ADDR_WIDTH)(0.U(DATA_WIDTH.W))))
  rf(0) := 0.U  // x0 is always zero

  when(io.wen && io.rd =/= 0.U) {
    rf(io.rd) := io.wdata
  }

  io.rdata1 := rf(io.rs1)
  io.rdata2 := rf(io.rs2)
  io.goodTrap := rf(10) === 0.U
  //io.rf_out := rf // expose the entire register file

}
