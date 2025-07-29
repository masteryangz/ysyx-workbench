package npc

import chisel3._
//import chisel3.util._

class adder(DATA_WIDTH: Int = 32) extends Module {
    val io = IO(new adderIO())

    io.result := io.add1 + io.add2

}