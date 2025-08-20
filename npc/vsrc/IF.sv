module IF #(
    parameter pcInc = 4,
    parameter ADDR_WIDTH = 5,
    parameter DATA_WIDTH = 32
) (
    input   clock,                   // clock signal
    input   reset,                   // reset signal
    input   is_jump,
    input   valid,
    input   wen,
    input   [DATA_WIDTH-1:0] target,
    output  [DATA_WIDTH-1:0] pc,     // program counter
    output  [DATA_WIDTH-1:0] instr   // fetched instruction
);

    import "DPI-C" context function int unsigned pmem_read(input int unsigned addr);
    import "DPI-C" context function int unsigned pmem_write(input int unsigned waddr, input int unsigned wdata, input byte wmask);

    reg [DATA_WIDTH-1:0] pcReg;
    reg [DATA_WIDTH-1:0] rdata;

    always @(posedge clock) begin
        if (reset) begin
            pcReg   <= '0;
            instr   <= '0; // Reset instruction to 0
        end else begin
            pcReg   <= is_jump ? (pcReg + target) : (pcReg + pcInc);
            instr   <= pmem_read(pc); // Convert byte address to word address
        end
    end

    always @(*) begin
        if (valid) begin // 有读写请求时
            rdata = pmem_read(raddr);
            if (wen) begin // 有写请求时
                pmem_write(waddr, wdata, wmask);
            end
        end
        else begin
            rdata = 0;
        end
    end

    assign pc = pcReg;
endmodule
