package npc

import chisel3._
import chisel3.util._
import chisel3.experimental.IntParam

class MemU(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends BlackBox(Map(
    "ADDR_WIDTH" -> IntParam(ADDR_WIDTH),
    "DATA_WIDTH" -> IntParam(DATA_WIDTH)
  )){
  val io = IO(new MemUIO())
}
