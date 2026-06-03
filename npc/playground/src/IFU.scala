package npc

import chisel3._
import chisel3.experimental.IntParam
import parameters._

//class IFU(pcInc: Int = 4, ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends BlackBox(Map(
class IFU extends BlackBox(Map(
    "pcInc" -> IntParam(pcInc),
    "ADDR_WIDTH" -> IntParam(ADDR_WIDTH),
    "DATA_WIDTH" -> IntParam(DATA_WIDTH)
  )){
    val io = IO(new IFUIO())
    //val isEbreak    = io.instr === "h00100073".U
    //val trapReg     = RegNext(isEbreak, false.B)
    //io.trapPulse    := isEbreak && !trapReg // 只在 isEbreak 从 0 变成 1 的时钟沿为 true
    // Reg for Program Counter (PC) 
    //val pcReg = RegInit(0x80000000L.U(DATA_WIDTH.W))

    // PC update logic
    //pcReg := pcReg + pcInc.U
    /*
    when(io.is_jump) {
        pcReg := io.target
    } .otherwise {
        pcReg := pcReg + pcInc.U
    }

    // Output current PC
    io.pc := pcReg
    */
}