package npc

import chisel3._
import chisel3.experimental._
import chisel3.util.HasBlackBoxInline

class DPIEnd extends BlackBox {
  val io = IO(new Bundle {
    val trap = Input(Bool()) 
  })
}
/*
class regBridge(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends BlackBox {
  val io = IO(new Bundle {
    val pc = Input(UInt(DATA_WIDTH.W))
    val gpr = Input(Vec(1 << ADDR_WIDTH, UInt(DATA_WIDTH.W)))
  })
}
*/
class regBridgeIO(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Bundle {
  val pc  = Input(UInt(DATA_WIDTH.W))
  val gpr = Input(Vec(1<<ADDR_WIDTH, UInt(DATA_WIDTH.W)))
}

class regBridge(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32)
  extends BlackBox(Map(
    "ADDR_WIDTH" -> IntParam(ADDR_WIDTH),
    "DATA_WIDTH" -> IntParam(DATA_WIDTH)
  )) {
  val io = IO(new regBridgeIO(DATA_WIDTH))
}
class MemReadBridge(ADDR_WIDTH: Int = 32, DATA_WIDTH: Int = 32) extends BlackBox {
  val io = IO(new Bundle {
    val addr = Output(UInt(ADDR_WIDTH.W))  // 虚拟地址（vaddr）
    val data = Input(UInt(DATA_WIDTH.W)) // 输出读取结果
  })
}
