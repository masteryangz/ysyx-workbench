module EXdpi (
  input wire trap
);
  import "DPI-C" context function void set_npc_state(input int unsigned npc, input int unsigned pc, input int unsigned halt_ret);
  reg called;

  initial called = 0;

  always @(trap) begin
    $display("[VERILOG] dpiEnd trap = %b", trap);
    if (trap && !called) begin
      NPCTRAP(pc, R(10));
      called = 1;
    end
  end
endmodule
