module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [15:0] sw,
  output [3:0] seg1,
  output [3:0] seg2,
  output [15:0] ledr
);

  alu u(
    .A(sw[4:0]),          // 5-bit 输入 A
    .B(sw[9:5]),          // 5-bit 输入 B
    .Func(sw[12:10]),     // 3-bit 功能选择
    .Out(ledr[4:0]),      // 5-bit 运算结果
    .Zero(ledr[5]),       // 结果是否为 0（未使用）
    .Overflow(ledr[6]),   // 是否溢出（未使用）
    .CarryOut(ledr[7])    // 是否有进位（未使用）
  );

endmodule
