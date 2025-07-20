package npc

import chisel3._
import chisel3.util.experimental.loadMemoryFromFile

class IF(memDepth: Int = 256, pcInc: Int = 4) extends Module {
    val io = IO(new Bundle {
        val pc      = Output(UInt(32.W))    // current PC
        val instr   = Output(UInt(32.W))    // fetched instruction
    })

    // Reg for Program Counter (PC) 
    val pcReg = RegInit(0.U(32.W))

    // PC update logic
    pcReg := pcReg + pcInc.U

    // Output current PC
    io.pc := pcReg

    val instrMem = Mem(memDepth, UInt(32.W)) // memory
    //val instrMem = SyncReadMem(memDepth, UInt(32.W))

    // ROM
    loadMemoryFromFile(instrMem, "rom.txt")

    // Fetch instruction from memory (pcReg >> 2 gives the index of instr)
    //val instrReg = Reg(UInt(32.W))
    //instrReg := instrMem.read(pcReg >> 2)
    //io.instr := instrReg
    io.instr := instrMem.read(pcReg >> 2)
    //printf(p"PC = 0x${Hexadecimal(pcReg)}, instr = 0x${Hexadecimal(io.instr)}\n")

}