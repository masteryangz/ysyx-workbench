module DPIEnd #(
  parameter DATA_WIDTH = 32
)(
  input wire trap,
  input wire [DATA_WIDTH-1:0] pc
);
  import "DPI-C" context function void sim_end(input int pc);
  reg called;

  initial called = 0;

  always @(posedge trap) begin
    $display("[VERILOG] dpiEnd trap = %b", trap);
    if (trap && !called) begin
      sim_end(pc);
      called = 1;
    end
  end
endmodule
