package npc

import chisel3._
import chisel3.util.experimental.loadMemoryFromFile

class IF(memDepth: Int = 256, pcInc: Int = 4, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new IFIO())

    // Reg for Program Counter (PC) 
    val pcReg = RegInit(0x80000000L.U(DATA_WIDTH.W))

    // PC update logic
    pcReg := pcReg + pcInc.U

    // Output current PC
    //io.pc := pcReg
    io.pc := Mux(io.is_jump, io.target, pcReg)

    val instrMem = Mem(memDepth, UInt(DATA_WIDTH.W)) // memory
    //val instrMem = SyncReadMem(memDepth, UInt(DATA_WIDTH.W))

    // ROM
    loadMemoryFromFile(instrMem, "rom.txt")

    // Fetch instruction from memory (pcReg >> 2 gives the index of instr)
    //val instrReg = Reg(UInt(DATA_WIDTH.W))
    //instrReg := instrMem.read(pcReg >> 2)
    //io.instr := instrReg
    io.instr := instrMem.read(pcReg >> 2)
    //printf(p"PC = 0x${Hexadecimal(pcReg)}, instr = 0x${Hexadecimal(io.instr)}\n")

}