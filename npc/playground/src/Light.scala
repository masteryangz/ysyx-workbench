import chisel3._
import chisel3.util._

class Light extends Module {
  val io = IO(new Bundle {
    val rst = Input(Bool())         // reset (synchronous in this case)
    val led = Output(UInt(16.W))    // 16-bit LED output
  })

  val count = RegInit(0.U(32.W))
  val ledReg = RegInit(1.U(16.W))   // initial LED = 0x0001

  when (io.rst) {
    ledReg := 1.U
    count := 0.U
  } .otherwise {
    when (count === 0.U) {
      ledReg := Cat(ledReg(14, 0), ledReg(15)) // rotate left
    }
    count := Mux(count >= 5000000.U, 0.U, count + 1.U)
  }

  io.led := ledReg
}
