package npc

import chisel3._
import chisel3.experimental._

class DPIEnd extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
  })
}
/*
class regBridge(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32)
  extends BlackBox(Map(
    "ADDR_WIDTH" -> IntParam(ADDR_WIDTH),
    "DATA_WIDTH" -> IntParam(DATA_WIDTH)
  )) {
  val pc  = Input(UInt(DATA_WIDTH.W))
  val gpr = Input(Vec(1<<ADDR_WIDTH, UInt(DATA_WIDTH.W)))
}
class MemReadBridge(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends BlackBox(Map(
    "ADDR_WIDTH" -> IntParam(ADDR_WIDTH),
    "DATA_WIDTH" -> IntParam(DATA_WIDTH)
  )) {
  val io = IO(new Bundle {
    //val addr = Input(UInt((1<<ADDR_WIDTH).W))  
    //val wdata = Input(UInt(DATA_WIDTH.W))
    val rdata = Input(UInt(DATA_WIDTH.W))
    val wen = Output(Bool())
    val rdata_out = Output(UInt(DATA_WIDTH.W)) 
  })
}
*/
