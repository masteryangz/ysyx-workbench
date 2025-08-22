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
    input   [DATA_WIDTH-1:0] addr,
    input   [DATA_WIDTH-1:0] wdata,
    input   [DATA_WIDTH-1:0] target,
    output  [DATA_WIDTH-1:0] pc,     // program counter
    output  reg [DATA_WIDTH-1:0] instr   // fetched instruction
);

    import "DPI-C" context function int unsigned pmem_read(input int unsigned addr);
    import "DPI-C" context function void pmem_write(input int unsigned addr, input int unsigned wdata, input byte wmask);

    reg [DATA_WIDTH-1:0] pcReg;
    reg [DATA_WIDTH-1:0] rdata;

    always @(posedge clock) begin
        if (reset) begin
            pcReg   <= 32'h80000000;
            instr   <= pmem_read(32'h80000000);
        end else begin
            pcReg   <= is_jump ? (pcReg + target) : (pcReg + pcInc);
            instr   <= pmem_read(pcReg); // Convert byte address to word address
        end
    end
    //assign instr = pmem_read(pcReg);
    always @(*) begin
        if (valid) begin // 有读写请求时
            rdata = pmem_read(addr);
            if (wen) begin // 有写请求时
                pmem_write(addr, wdata, 4'b1111);
            end
        end
        else begin
            rdata = 0;
        end
    end

    assign pc = pcReg;
endmodule
