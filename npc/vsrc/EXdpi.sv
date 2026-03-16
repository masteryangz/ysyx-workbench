module EXdpi #(
  parameter DATA_WIDTH = 32
)(
  input isEbreak,
  input [DATA_WIDTH-1:0] pc,
  input [DATA_WIDTH-1:0] R10
);
  import "DPI-C" context function void NPCTRAP(input int unsigned pc, input int unsigned halt_ret);
  reg called;

  initial called = 0;

  always @(isEbreak) begin
    $display("[VERILOG] Ebreak triggered = %b", isEbreak);
    if (isEbreak && !called) begin
      NPCTRAP(pc, R10); // 1 for Ebreak
      called = 1;
    end
  end
endmodule
