module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [9:0] sw,
  output [15:0] ledr
);
  reg [1:0] Y;
  reg [1:0] out;

  assign Y = sw[1:0];

  MuxKey #(4, 2, 2) mux0 (
    .out(out),
    .key(sw[1:0]),
    .lut({
      2'b00, sw[3:2],
      2'b01, sw[5:4],
      2'b10, sw[7:6],
      2'b11, sw[9:8]
    })
  );

  assign ledr = {14'b0, out};
endmodule
