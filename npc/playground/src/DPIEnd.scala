package npc

import chisel3._
import chisel3.experimental._

class DPIEnd(DATA_WIDTH: Int = 32) extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
    val pc = Input(UInt(DATA_WIDTH.W))
  })
}

