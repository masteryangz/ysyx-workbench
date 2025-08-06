module DPIEnd (
  input wire trap
);
  import "DPI-C" context function void sim_end();
  reg called;

  initial called = 0;

  always @(trap) begin
    $display("[VERILOG] dpiEnd trap = %b", trap);
    if (trap && !called) begin
      sim_end();
      called = 1;
    end
  end
endmodule
