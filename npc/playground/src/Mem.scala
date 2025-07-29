package npc

import chisel3._
import chisel3.util.experimental.loadMemoryFromFileInline

class MemU(memDepth: Int = 256, DATA_WIDTH: Int = 32, romFile: String = "rom.txt") extends Module {
    val io = IO(new MemUIO())

    //val instrMem = Mem(memDepth, UInt(DATA_WIDTH.W)) // memory
    //val instrMem = SyncReadMem(memDepth, UInt(DATA_WIDTH.W))

    // ROM
    //loadMemoryFromFile(instrMem, romFile)

    //io.instr := instrMem.read(io.pc >> 2)
    //printf(p"PC = 0x${Hexadecimal(pcReg)}, instr = 0x${Hexadecimal(io.instr)}\n")

    //val bridge = Module(new MemBridge(memDepth, DATA_WIDTH))
    val instrMem = RegInit(VecInit(Seq.fill(memDepth)(0.U(DATA_WIDTH.W))))
    loadMemoryFromFileInline(instrMem, romFile)
    io.instr := instrMem((io.pc >> 2).asUInt)
    val bridge = Module(new MemBridge(memDepth, DATA_WIDTH))
    bridge.io.mem := instrMem

}