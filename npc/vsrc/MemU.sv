module MemU #(
    parameter ADDR_WIDTH = 5,
    parameter DATA_WIDTH = 32
) (
    input  clock,                   // clock signal
    input  reset,                   // reset signal
    input  [DATA_WIDTH-1:0] pc,     // program counter
    output [DATA_WIDTH-1:0] instr   // fetched instruction
);

    import "DPI-C" context function int unsigned pmem_read(input int unsigned addr);

    // Memory read operation
    /*
    always @(posedge clock) begin
        if (reset) begin
            instr <= '0; // Reset instruction to 0
        end else begin
            instr <= pmem_read(pc); // Convert byte address to word address
        end
    end
    */
    assign instr = reset ? '0 : pmem_read(pc);
endmodule
