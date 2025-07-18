module top(
    input clk,
    input rst,
    input [4:0] btn,
    input [15:0] sw,
    input ps2_clk,
    input ps2_data,
    input uart_rx,
    output uart_tx,
    output [2:0] ledr,
    output VGA_CLK,
    output VGA_HSYNC,
    output VGA_VSYNC,
    output VGA_BLANK_N,
    output [7:0] VGA_R,
    output [7:0] VGA_G,
    output [7:0] VGA_B,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
assign VGA_CLK = clk;

wire [9:0] h_addr;
wire [9:0] v_addr;
wire [23:0] vga_data;
wire [3:0] hex0, hex1, hex2, hex3;
wire [4:0] h0, h1, h2, h3, h4, h5;
wire ready;
wire sampling;
wire overflow;
wire [7:0] key_code;
wire [7:0] ascii_code;
wire nextdata_n;

assign hex0 = key_code[3:0];
assign hex1 = key_code[7:4];
assign hex2 = ascii_code[3:0];
assign hex3 = ascii_code[7:4];
//assign nextdata_n = clk;
led my_led(
    .clk(clk),
    .rst(rst),
    .btn(btn),
    .sw(sw),
    .ledr(ledr)
);

fsm my_fsm(
    .clk(clk),
    .rst(rst),
    .ready(ready),
    .overflow(overflow),
    .data(key_code),
    .hex0(hex0),
    .hex1(hex1),
    .hex2(hex2),
    .hex3(hex3),
    .h0(h0),
    .h1(h1),
    .h2(h2),
    .h3(h3),
    .h4(h4),
    .h5(h5),
    .nextdata_n(nextdata_n)
);

vga_ctrl my_vga_ctrl(
    .pclk(clk),
    .reset(rst),
    .vga_data(vga_data),
    .h_addr(h_addr),
    .v_addr(v_addr),
    .hsync(VGA_HSYNC),
    .vsync(VGA_VSYNC),
    .valid(VGA_BLANK_N),
    .vga_r(VGA_R),
    .vga_g(VGA_G),
    .vga_b(VGA_B)
);

ps2_keyboard my_keyboard(
    .clk(clk),
    .clrn(~rst),
    .ps2_clk(ps2_clk),
    .ps2_data(ps2_data),
    .data(key_code),
    .sampling(sampling),
    .ready(ready),
    .nextdata_n(nextdata_n),
    .ledr(ledr),
    .overflow(overflow)
);

seg my_seg(
    //.clk(clk),
    .rst(rst),
    .h0(h0),
    .h1(h1),
    .h2(h2),
    .h3(h3),
    .h4(h4),
    .h5(h5),
    //.data(key_code),
    .o_seg0(seg0),
    .o_seg1(seg1),
    .o_seg2(seg2),
    .o_seg3(seg3),
    .o_seg4(seg4),
    .o_seg5(seg5)
);

ascii_converter my_ascii_converter(
    .key_code(key_code),
    .ascii_code(ascii_code)
);

vmem my_vmem(
    .h_addr(h_addr),
    .v_addr(v_addr[8:0]),
    .vga_data(vga_data)
);

uart my_uart(
  .tx(uart_tx),
  .rx(uart_rx)
);

endmodule

module vmem(
    input [9:0] h_addr,
    input [8:0] v_addr,
    output [23:0] vga_data
);

reg [23:0] vga_mem [524287:0];

initial begin
    $readmemh("resource/picture.hex", vga_mem);
end

assign vga_data = vga_mem[{h_addr, v_addr}];

endmodule
