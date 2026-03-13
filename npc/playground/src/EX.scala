package npc

import chisel3._
import chisel3.util._

class EX(ADDR_WIDTH: Int = 5, DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new EXIO())
    val alu = Module(new ALU())
    val csr = Module(new CSR())

}