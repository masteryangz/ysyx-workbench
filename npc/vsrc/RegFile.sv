module RegFile #(
  parameter ADDR_WIDTH = 5,
  parameter DATA_WIDTH = 32
)(
  input  clock,
  input  reset,
  input  [DATA_WIDTH-1:0] pc,
  input  [ADDR_WIDTH-1:0] rs1,
  input  [ADDR_WIDTH-1:0] rs2,
  input  [ADDR_WIDTH-1:0] rd,
  input  [DATA_WIDTH-1:0] wdata,
  input  wen,
  output [DATA_WIDTH-1:0] rdata1,
  output [DATA_WIDTH-1:0] rdata2,
  output goodTrap
);
  export "DPI-C" function get_pc;
  export "DPI-C" function get_gpr;
  export "DPI-C" function get_sv_time;
  // Register file: 32 registers of 32-bit
  reg [DATA_WIDTH-1:0] rf [0:(1<<ADDR_WIDTH)-1];

  // Reset: all registers to 0 (including x0)
  integer i;
  always @(posedge clock) begin
    if (reset) begin
      for (i = 0; i < (1 << ADDR_WIDTH); i = i + 1) begin
        rf[i] <= '0;
      end
    end else if (wen && rd != {ADDR_WIDTH{1'b0}}) begin
      rf[rd] <= wdata;
    end
  end

  // x0 is hardwired to 0
  wire [DATA_WIDTH-1:0] x0 = '0;

  // Read ports
  assign rdata1 = (rs1 == {ADDR_WIDTH{1'b0}}) ? x0 : rf[rs1];
  assign rdata2 = (rs2 == {ADDR_WIDTH{1'b0}}) ? x0 : rf[rs2];

  // Trap condition: x10 == 0
  assign goodTrap = (rf[10] == '0);

  function longint get_sv_time();
    // Get the current simulation time
    get_sv_time = $time;
  endfunction

  // Export for C debugger (DPI-C)
  function int get_pc();
    get_pc = pc;
  endfunction

  function int get_gpr(input int idx);
    get_gpr = rf[idx];  
  endfunction



endmodule
