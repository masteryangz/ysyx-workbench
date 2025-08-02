module regBridge #(
  parameter ADDR_WIDTH = 5,
  parameter DATA_WIDTH = 32) (
  input [DATA_WIDTH-1:0] pc,
  //input [DATA_WIDTH-1:0] gpr [0:(1<<ADDR_WIDTH)-1]
  input [DATA_WIDTH-1:0] gpr_0,
  input [DATA_WIDTH-1:0] gpr_1,
  input [DATA_WIDTH-1:0] gpr_2,
  input [DATA_WIDTH-1:0] gpr_3,
  input [DATA_WIDTH-1:0] gpr_4,
  input [DATA_WIDTH-1:0] gpr_5,
  input [DATA_WIDTH-1:0] gpr_6,
  input [DATA_WIDTH-1:0] gpr_7,
  input [DATA_WIDTH-1:0] gpr_8,
  input [DATA_WIDTH-1:0] gpr_9,
  input [DATA_WIDTH-1:0] gpr_10,
  input [DATA_WIDTH-1:0] gpr_11,
  input [DATA_WIDTH-1:0] gpr_12,
  input [DATA_WIDTH-1:0] gpr_13,
  input [DATA_WIDTH-1:0] gpr_14,
  input [DATA_WIDTH-1:0] gpr_15,
  input [DATA_WIDTH-1:0] gpr_16,
  input [DATA_WIDTH-1:0] gpr_17,
  input [DATA_WIDTH-1:0] gpr_18,
  input [DATA_WIDTH-1:0] gpr_19,
  input [DATA_WIDTH-1:0] gpr_20,
  input [DATA_WIDTH-1:0] gpr_21,
  input [DATA_WIDTH-1:0] gpr_22,
  input [DATA_WIDTH-1:0] gpr_23,
  input [DATA_WIDTH-1:0] gpr_24,
  input [DATA_WIDTH-1:0] gpr_25,
  input [DATA_WIDTH-1:0] gpr_26,
  input [DATA_WIDTH-1:0] gpr_27,
  input [DATA_WIDTH-1:0] gpr_28,
  input [DATA_WIDTH-1:0] gpr_29,
  input [DATA_WIDTH-1:0] gpr_30,
  input [DATA_WIDTH-1:0] gpr_31
);

  import "DPI-C" function void set_gpr_ptr(input longint unsigned ptr);
  import "DPI-C" function void set_pc_ptr(input longint unsigned ptr);

  longint unsigned c_gpr_ptr;
  longint unsigned c_pc_ptr;

  initial begin
    set_gpr_ptr(c_gpr_ptr);
    set_pc_ptr(c_pc_ptr);
  end
  
  assign c_pc_ptr = pc;
  assign c_gpr_ptr = {gpr_31, gpr_30, gpr_29, gpr_28, gpr_27, gpr_26, gpr_25, gpr_24,
                   gpr_23, gpr_22, gpr_21, gpr_20, gpr_19, gpr_18, gpr_17, gpr_16,
                   gpr_15, gpr_14, gpr_13, gpr_12, gpr_11, gpr_10, gpr_9,  gpr_8,
                   gpr_7,  gpr_6,  gpr_5,  gpr_4,  gpr_3,  gpr_2,  gpr_1,  gpr_0};
/*
  always @(*) begin
    if (c_gpr_ptr != 0) begin
      for (int i = 0; i < (1<<DATA_WIDTH); i++) begin
        c_gpr_ptr[i] = gpr[i];
      end
    end
  end
*/
endmodule
