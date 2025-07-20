package npc

import chisel3._
import chisel3.simulator.EphemeralSimulator._
import org.scalatest.freespec.AnyFreeSpec
import org.scalatest.matchers.must.Matchers

class RegFileSpec extends AnyFreeSpec with Matchers {
  "RegFile should behave correctly" in {
    simulate(new RegFile(ADDR_WIDTH = 5, DATA_WIDTH = 32)) { dut =>
      // Reset logic (if needed)
      dut.reset.poke(true)
      dut.clock.step(2)
      dut.reset.poke(false)

      // Check that x0 is always zero
      dut.io.raddr.poke(0.U)
      dut.clock.step()
      dut.io.rdata.expect(0.U)

      // Try to write to x0
      dut.io.waddr.poke(0.U)
      dut.io.wdata.poke(123.U)
      dut.io.wen.poke(true.B)
      dut.clock.step()

      // Read x0 again; should still be zero
      dut.io.wen.poke(false.B)
      dut.io.raddr.poke(0.U)
      dut.clock.step()
      dut.io.rdata.expect(0.U)

      // Write to x5
      val regIdx = 5
      val value  = 0xABCD1234L
      dut.io.waddr.poke(regIdx.U)
      dut.io.wdata.poke(value.U)
      dut.io.wen.poke(true.B)
      dut.clock.step()

      // Read from x5
      dut.io.wen.poke(false.B)
      dut.io.raddr.poke(regIdx.U)
      dut.clock.step()
      dut.io.rdata.expect(value.U)

      // Ensure other register (x6) is still 0
      dut.io.raddr.poke(6.U)
      dut.clock.step()
      dut.io.rdata.expect(0.U)
    }
  }
}
