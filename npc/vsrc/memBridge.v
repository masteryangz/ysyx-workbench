module MemBridge #(
  parameter DEPTH = 256,
  parameter DATA_WIDTH = 32
)(
  input [DATA_WIDTH-1:0] mem [0:DEPTH-1]
);

  import "DPI-C" function void set_instr_mem_ptr(input longint unsigned ptr);

  longint unsigned c_instr_ptr;

  initial begin
    set_instr_mem_ptr(c_instr_ptr);
  end

  always @(*) begin
    if (c_instr_ptr != 0) begin
      for (int i = 0; i < DEPTH; i++) begin
        c_instr_ptr[i] = mem[i];
      end
    end
  end

endmodule
