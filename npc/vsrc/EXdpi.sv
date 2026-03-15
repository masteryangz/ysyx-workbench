module EXdpi (
  input wire isEbreak
);
  import "DPI-C" context function void NPCTRAP(input int unsigned pc, input int unsigned halt_ret);
  reg called;

  initial called = 0;

  always @(isEbreak) begin
    $display("[VERILOG] Ebreak triggered = %b", isEbreak);
    if (isEbreak && !called) begin
      NPCTRAP(pc, R(10));
      called = 1;
    end
  end
endmodule
