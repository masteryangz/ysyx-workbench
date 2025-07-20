module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [8:0] sw,
  output [2:0] seg,
  output [15:0] ledr
);

  reg [2:0] r;
  pr_encoder pr_encoder_inst(
    .en(sw[8]),
    .x(sw[7:0]),
    .y(r)
  );

  assign seg = r;
  assign ledr = {11'b0, sw[8], 1'b0, r};
endmodule
