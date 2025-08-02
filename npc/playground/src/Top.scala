package npc

import chisel3._
import chisel3.util._

class Top(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32, romFile: String = "rom.txt") extends Module {
  val io = IO(new Bundle {
    val dpi_mem_addr  = Input(UInt(ADDR_WIDTH.W))   // expose memory address
    val dpi_mem_data  = Output(UInt(DATA_WIDTH.W))  // expose memory data
    //val instr         = Output(UInt(DATA_WIDTH.W))  // expose instruction
    val pc            = Output(UInt((1<<ADDR_WIDTH).W))
    val rf_out        = Output(Vec(1 << ADDR_WIDTH, UInt(DATA_WIDTH.W))) // expose the entire register file
    val goodTrap      = Output(Bool()) // expose goodTrap signal
  })

  // create new module
  val instrfet  = Module(new IF())
  val decoder   = Module(new Decoder())
  val alu       = Module(new ALU())
  val memU      = Module(new MemU(memDepth = 256, DATA_WIDTH = DATA_WIDTH, romFile = romFile))
  val dpiEnd    = Module(new DPIEnd)
  val isEbreak  = memU.io.instr === "h00100073".U
  val trapReg   = RegNext(isEbreak, false.B)
  val trapPulse = isEbreak && !trapReg // 只在 isEbreak 从 0 变成 1 的时钟沿为 true
  val memReader = Module(new MemReadBridge(DATA_WIDTH))
  //val regBridge = Module(new regBridge(ADDR_WIDTH, DATA_WIDTH))

  // connect
  io.dpi_mem_data         := memU.io.dpi_mem_data
  //io.dpi_mem_addr         := memReader.io.addr
  memU.io.dpi_mem_addr    := memReader.io.addr
  //memReader.io.data       := memU.io.dpi_mem_data
  io.rf_out               := decoder.io.rf_out
  //regBridge.io.gpr        := decoder.io.rf_out
  //io.instr                := memU.io.instr
  io.pc                   := instrfet.io.pc
  io.goodTrap             := decoder.io.goodTrap
  memU.io.pc              := instrfet.io.pc
  decoder.io.pc           := instrfet.io.pc
  decoder.io.instr        := memU.io.instr
  decoder.io.wdata        := alu.io.wdata
  decoder.io.wen          := alu.io.wen
  //decoder.io.In_trapPulse := instrfet.io.trapPulse
  alu.io.rdata1           := decoder.io.rdata1
  alu.io.rdata2           := decoder.io.rdata2
  alu.io.Op               := decoder.io.Op
  alu.io.funct3           := decoder.io.funct3
  alu.io.imm              := decoder.io.imm
  alu.io.pc               := instrfet.io.pc
  //alu.io.In_trapPulse     := decoder.io.trapPulse
  instrfet.io.target      := alu.io.target
  instrfet.io.is_jump     := alu.io.is_jump
  //dpiEnd.io.trap          := alu.io.trapPulse
  dpiEnd.io.trap          := trapPulse

  //when (alu.io.trapPulse) {
  //  printf(p"[NPC] trapPulse asserted! instr = 0x${Hexadecimal(instrfet.io.instr)}\n")
  //}


  dontTouch(instrfet.io)
  dontTouch(decoder.io)
  dontTouch(alu.io)
  dontTouch(dpiEnd.io.trap)
  dontTouch(memU.io)
  //dontTouch(memReader.io)
}
