module seg(
  input clk,
  input rst,
  input [3:0] hex0,
  input [3:0] hex1,
  output [7:0] o_seg0,
  output [7:0] o_seg1
);

wire [7:0] segs [0:15];

assign segs[0] = 8'b11111100;
assign segs[1] = 8'b01100000;
assign segs[2] = 8'b11011010;
assign segs[3] = 8'b11110010;
assign segs[4] = 8'b01100110;
assign segs[5] = 8'b10110110;
assign segs[6] = 8'b10111110;
assign segs[7] = 8'b11100000;
assign segs[8] = 8'b11111110;
assign segs[9] = 8'b11110110;
assign segs[10] = 8'b11101110;
assign segs[11] = 8'b00111110;
assign segs[12] = 8'b10011100;
assign segs[13] = 8'b01111010;
assign segs[14] = 8'b10011110;
assign segs[15] = 8'b10001110;

assign o_seg0 = ~segs[hex0];
assign o_seg1 = ~segs[hex1];
/*
parameter CLK_NUM = 5000000;

reg [31:0] count;
reg [2:0] offset;

always @(posedge clk) begin
  if(rst) begin count <= 0; offset <= 0; end
  else begin
    if(count == CLK_NUM) begin offset <= offset + 1; end
    count <= (count == CLK_NUM) ? 0 : count + 1;
  end
end

//assign o_seg0 = ~segs[offset + 3'd0];
assign o_seg0 = ~segs[seg];
assign o_seg1 = ~segs[offset + 3'd1];
assign o_seg2 = ~segs[offset + 3'd2];
assign o_seg3 = ~segs[offset + 3'd3];
assign o_seg4 = ~segs[offset + 3'd4];
assign o_seg5 = ~segs[offset + 3'd5];
assign o_seg6 = ~segs[offset + 3'd6];
assign o_seg7 = ~segs[offset + 3'd7];
*/

endmodule
