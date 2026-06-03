package npc

import chisel3._
import chisel3.experimental._
import parameters._

//class DPIEnd(DATA_WIDTH: Int = 32) extends BlackBox {
class DPIEnd extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
    val pc = Input(UInt(DATA_WIDTH.W))
  })
}

