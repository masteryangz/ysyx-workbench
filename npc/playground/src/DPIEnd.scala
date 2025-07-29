package npc

import chisel3._
import chisel3.experimental._

class DPIEnd extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
  })
}

class regBridge(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends BlackBox {
  val io = IO(new Bundle {
    val pc = Input(UInt(DATA_WIDTH.W))
    val gpr = Input(Vec(1 << ADDR_WIDTH, UInt(DATA_WIDTH.W)))
  })
}

class MemBridge(val depth: Int = 256, val width: Int = 32) extends BlackBox(
  Map("DEPTH" -> depth, "DATA_WIDTH" -> width)
) {
  val io = IO(new Bundle {
    val mem = Input(Vec(depth, UInt(width.W)))
  })
}