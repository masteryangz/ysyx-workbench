module  MemReadBridge #(
    parameter ADDR_WIDTH = 32,
    parameter DATA_WIDTH = 32) (
    input [ADDR_WIDTH - 1:0] addr,
    output [DATA_WIDTH - 1:0] data
);
endmodule
