module #(
    parameter ADDR_WIDTH = 5,
    parameter DATA_WIDTH = 32
) regBridge(
  input [DATA_WIDTH-1:0] pc,
  input [DATA_WIDTH-1:0] gpr [0:(1<<ADDR_WIDTH)-1]
);

  import "DPI-C" function void set_gpr_ptr(input longint unsigned ptr);

  longint unsigned c_gpr_ptr;
  longint unsigned c_pc_ptr;

  initial begin
    set_gpr_ptr(c_gpr_ptr);
    set_pc_ptr(c_pc_ptr);
  end
  
  assign c_pc_ptr = pc;

  always @(*) begin
    if (c_gpr_ptr != 0) begin
      for (int i = 0; i < (1<<ADDR_WIDTH); i++) begin
        c_gpr_ptr[i] = gpr[i];
      end
    end
  end

endmodule
