package npc

import chisel3._
import chisel3.util.experimental.loadMemoryFromFile

class IF(memDepth: Int = 256, pcInc: Int = 4, DATA_WIDTH: Int = 32, romFile: String = "rom.txt") extends Module {
    val io = IO(new IFIO())
    //val isEbreak    = io.instr === "h00100073".U
    //val trapReg     = RegNext(isEbreak, false.B)
    //io.trapPulse    := isEbreak && !trapReg // 只在 isEbreak 从 0 变成 1 的时钟沿为 true
    // Reg for Program Counter (PC) 
    val pcReg = RegInit(0x80000000L.U(DATA_WIDTH.W))

    // PC update logic
    //pcReg := pcReg + pcInc.U
    when(io.is_jump) {
        pcReg := io.target
    } .otherwise {
        pcReg := pcReg + pcInc.U
    }

    // Output current PC
    io.pc := pcReg


    val instrMem = Mem(memDepth, UInt(DATA_WIDTH.W)) // memory
    //val instrMem = SyncReadMem(memDepth, UInt(DATA_WIDTH.W))

    // ROM
    loadMemoryFromFile(instrMem, romFile)

    // Fetch instruction from memory (pcReg >> 2 gives the index of instr)
    //val instrReg = Reg(UInt(DATA_WIDTH.W))
    //instrReg := instrMem.read(pcReg >> 2)
    //io.instr := instrReg
    io.instr := instrMem.read(pcReg >> 2)
    //printf(p"PC = 0x${Hexadecimal(pcReg)}, instr = 0x${Hexadecimal(io.instr)}\n")

}