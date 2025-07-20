module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [15:0] sw,
  output [3:0] hex0,
  output [3:0] hex1,
  output [15:0] ledr
);

  lfsr my_lfsr(
    .clk(clk),
    .btn_clk(sw[0]),
    .hex0(hex0),
    .hex1(hex1)
  );

endmodule
