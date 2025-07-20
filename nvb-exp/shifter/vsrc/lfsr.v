module lfsr (
    input wire clk,          // 系统时钟（用于数码管刷新等）
    input wire btn_clk,      // 按钮作为 LFSR 的时钟信号
    output reg [3:0] hex0,   // 数码管 HEX0 显示低 4 位
    output reg [3:0] hex1    // 数码管 HEX1 显示高 4 位
);

    // LFSR 寄存器（8 位）
    reg [7:0] lfsr_reg;
    //reg feedback;

    // 初始化 LFSR（避免全零）
    initial begin
        lfsr_reg = 8'b00000001; // 初始值不能为全零
    end
    
    // LFSR 更新逻辑（按钮触发）
    always @(posedge btn_clk) begin
        //feedback = lfsr_reg[4] ^ lfsr_reg[3] ^ lfsr_reg[2] ^ lfsr_reg[0];

        // 右移一位，最高位由反馈位填充
        lfsr_reg <= {lfsr_reg[4] ^ lfsr_reg[3] ^ lfsr_reg[2] ^ lfsr_reg[0], lfsr_reg[7:1]};
    end

    // 将 LFSR 输出拆分为两个 4 位十六进制数（用于数码管显示）
    always @(posedge clk) begin
        hex0 <= lfsr_reg[3:0];   // 低 4 位 -> HEX0
        hex1 <= lfsr_reg[7:4];   // 高 4 位 -> HEX1
    end

endmodule
