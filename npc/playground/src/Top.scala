package npc

import chisel3._
import chisel3.util._

class Top(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
  val io = IO(new Bundle {
    //val pc            = Output(UInt((1<<ADDR_WIDTH).W))
    //val goodTrap      = Output(Bool()) // expose goodTrap signal
  })

  // create new module
  val ifu  = Module(new IFU())
  val idu        = Module(new IDU())
  val exu        = Module(new EXU())
  val dpiEnd    = Module(new DPIEnd(DATA_WIDTH))
  val isEbreak  = ifu.io.instr === "h00100073".U
  val trapReg   = RegNext(isEbreak, false.B)
  val trapPulse = isEbreak && !trapReg // 只在 isEbreak 从 0 变成 1 的时钟沿为 true

  // connect
  StageConnect(ifu.io.out, idu.io.in)
  StageConnect(idu.io.out, exu.io.in)
  StageConnect(exu.io.out, wbu.io.in)
  ifu.io.clock       := clock
  ifu.io.reset       := reset
  //io.goodTrap             := id.io.goodTrap
  idu.io.rdata             := ifu.io.rdata
  idu.io.valid             := exu.io.valid
  idu.io.pc                := ifu.io.pc
  idu.io.instr             := ifu.io.instr
  idu.io.wdata             := exu.io.wdata
  idu.io.rwen              := exu.io.rwen
  idu.io.raddr             := ifu.io.addr
  exu.io.R10               := idu.io.R10 // pass R10 from ID to EX for DPI use
  exu.io.rdata1            := idu.io.rdata1
  exu.io.rdata2            := idu.io.rdata2
  exu.io.Op                := idu.io.Op
  exu.io.funct3            := idu.io.funct3
  exu.io.funct7            := idu.io.funct7
  exu.io.imm               := idu.io.imm
  exu.io.pc                := ifu.io.pc
  ifu.io.target      := exu.io.target
  ifu.io.is_jump     := exu.io.is_jump
  ifu.io.valid       := exu.io.valid
  ifu.io.wen         := exu.io.mwen
  instrfet.io.addr        := ex.io.addr
  instrfet.io.wdata       := ex.io.wdata
  instrfet.io.wmask       := id.io.wmask
  dpiEnd.io.trap          := trapPulse
  dpiEnd.io.pc            := instrfet.io.pc


  dontTouch(instrfet.io)
  dontTouch(id.io)
  dontTouch(ex.io)
  dontTouch(dpiEnd.io.trap)

}

object StageConnect {
  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T]) = {
    val arch = "single"
    // 为展示抽象的思想, 此处代码省略了若干细节
    if      (arch == "single")   { right.bits := left.bits }
    else if (arch == "multi")    { right <> left }
    else if (arch == "pipeline") { right <> RegEnable(left, left.fire) }
    else if (arch == "ooo")      { right <> Queue(left, 16) }
  }
}
