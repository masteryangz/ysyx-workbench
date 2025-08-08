package npc

import chisel3._
import chisel3.experimental._

class DPIEnd extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
  })
}

