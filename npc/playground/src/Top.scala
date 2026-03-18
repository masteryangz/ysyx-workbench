package npc

import chisel3._
import chisel3.util._

class Top(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
  val io = IO(new Bundle {
    //val pc            = Output(UInt((1<<ADDR_WIDTH).W))
    //val goodTrap      = Output(Bool()) // expose goodTrap signal
  })

  // create new module
  val instrfet  = Module(new IF())
  val id        = Module(new ID())
  val ex        = Module(new EX())
  val dpiEnd    = Module(new DPIEnd(DATA_WIDTH))
  val isEbreak  = instrfet.io.instr === "h00100073".U
  val trapReg   = RegNext(isEbreak, false.B)
  val trapPulse = isEbreak && !trapReg // 只在 isEbreak 从 0 变成 1 的时钟沿为 true

  // connect
  instrfet.io.clock       := clock
  instrfet.io.reset       := reset
  //io.goodTrap             := id.io.goodTrap
  id.io.rdata             := instrfet.io.rdata
  id.io.valid             := ex.io.valid
  id.io.pc                := instrfet.io.pc
  id.io.instr             := instrfet.io.instr
  id.io.wdata             := ex.io.wdata
  id.io.rwen              := ex.io.rwen
  id.io.raddr             := instrfet.io.addr
  ex.io.R10               := id.io.R10 // pass R10 from ID to EX for DPI use
  ex.io.rdata1            := id.io.rdata1
  ex.io.rdata2            := id.io.rdata2
  ex.io.Op                := id.io.Op
  ex.io.funct3            := id.io.funct3
  ex.io.funct7            := id.io.funct7
  ex.io.imm               := id.io.imm
  ex.io.pc                := instrfet.io.pc
  instrfet.io.target      := ex.io.target
  instrfet.io.is_jump     := ex.io.is_jump
  instrfet.io.valid       := ex.io.valid
  instrfet.io.wen         := ex.io.mwen
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
