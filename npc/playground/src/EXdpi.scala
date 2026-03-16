package npc

import chisel3._
import chisel3.experimental._

class EXdpi(DATA_WIDTH: Int = 32) extends BlackBox {
  val io = IO(new Bundle {
    val isEbreak = Input(Bool()) 
    val pc       = Input(UInt(DATA_WIDTH.W))
    val R10      = Input(UInt(DATA_WIDTH.W))
  })
}

