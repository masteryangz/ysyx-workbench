package npc

import chisel3._
import chisel3.experimental._

class EXdpi extends BlackBox {
  val io = IO(new Bundle {
    val isEbreak = Input(Bool()) 
  })
}

