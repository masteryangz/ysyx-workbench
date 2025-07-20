module alu (
    input  wire [4:0] A,       // 5-bit 输入 A（带符号补码：1位符号 + 4位数值）
    input  wire [4:0] B,       // 5-bit 输入 B（带符号补码）
    input  wire [2:0] Func,    // 3-bit 功能选择（控制 ALU 执行哪种操作）
    output reg  [4:0] Out,     // 5-bit 运算结果
    output reg        Zero,    // 结果是否为 0
    output reg        Overflow,// 是否溢出（加减运算有效）
    output reg        CarryOut // 是否有进位（加减运算有效）
);

    // ======================
    // 内部信号
    // ======================
    wire [4:0] AddSubResult;   // 加法/减法结果
    wire AddSubOverflow;       // 加法/减法溢出
    wire AddSubCarryOut;       // 加法/减法进位
    wire [4:0] NotA;           // ~A（按位取反）
    wire [4:0] AndAB;          // A & B
    wire [4:0] OrAB;           // A | B
    wire [4:0] XorAB;          // A ^ B
    wire [4:0] CompareResult;  // A - B（用于比较大小）
/* verilator lint_off UNUSEDSIGNAL */
    wire CompareOverflow;      // A - B 的溢出（忽略）
    wire CompareCarryOut;      // A - B 的进位（忽略）
/* verilator lint_on UNUSEDSIGNAL */
    wire A_less_than_B;        // A < B（带符号）
    wire A_equal_to_B;         // A == B

    // ======================
    // 实例化加法器模块（用于加法、减法、比较、相等判断）
    // ======================
    // 加法/减法（Func=000 加法，Func=001 减法）
    adder_5bit_signed adder_add_sub (
        .A(A),
        .B(B),
        .Cin((Func == 3'b001) ? 1'b1 : 1'b0), // 减法时 Cin=1，其他情况 Cin=0
        .Sum(AddSubResult),
        .Overflow(AddSubOverflow),
        .CarryOut(AddSubCarryOut)
    );

    // 取反（Func=010）
    assign NotA = ~A;

    // 逻辑与（Func=011）
    assign AndAB = A & B;

    // 逻辑或（Func=100）
    assign OrAB = A | B;

    // 逻辑异或（Func=101）
    assign XorAB = A ^ B;

    // 比较大小（A < B，带符号）（Func=110）
    // 方法：计算 A - B，若结果为负（符号位=1），则 A < B
    adder_5bit_signed adder_compare (
        .A(A),
        .B(~B + 1), // A - B = A + (-B)
        .Cin(1'b0),
        .Sum(CompareResult),
        .Overflow(CompareOverflow),
        .CarryOut(CompareCarryOut)
    );
    assign A_less_than_B = CompareResult[4]; // 符号位为1表示负数（A < B）

    // 判断相等（A == B）（Func=111）
    // 方法：计算 A - B，若结果为0，则 A == B
    // 复用 adder_compare 的 CompareResult
    assign A_equal_to_B = (CompareResult == 5'b0);

    // ======================
    // Zero 检测（结果是否为0）
    // ======================
    /*
    always @(*) begin
        case (Func)
            3'b000, 3'b001:                           Zero = (AddSubResult == 5'b0);
            3'b010:                                   Zero = (NotA == 5'b0);
            3'b011:                                   Zero = (AndAB == 5'b0);    
            3'b100:                                   Zero = (OrAB == 5'b0);
            3'b101:                                   Zero = (XorAB == 5'b0);
            3'b110, 3'b111:                           Zero = 1'b0; // 比较/相等操作，Zero无效
            default:                                  Zero = 1'b0;
        endcase
    end
    */
    // ======================
    // 最终输出逻辑
    // ======================
    always @(*) begin
        case (Func)
            3'b000: begin // 加法
                Out = AddSubResult;
                Overflow = AddSubOverflow;
                CarryOut = AddSubCarryOut;
            end
            3'b001: begin // 减法
                Out = AddSubResult;
                Overflow = AddSubOverflow;
                CarryOut = AddSubCarryOut;
            end
            3'b010: begin // 取反
                Out = NotA;
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            3'b011: begin // 与
                Out = AndAB;
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            3'b100: begin // 或
                Out = OrAB;
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            3'b101: begin // 异或
                Out = XorAB;
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            3'b110: begin // 比较大小（A < B）
                Out = {4'b0, A_less_than_B}; // 仅最低位有效
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            3'b111: begin // 判断相等（A == B）
                Out = {4'b0, A_equal_to_B}; // 仅最低位有效
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
            default: begin
                Out = 5'b0;
                Overflow = 1'b0;
                CarryOut = 1'b0;
            end
        endcase
    end
    assign Zero = ~(|Out); // Zero 检测：如果 Out 全为0，则 Zero 为1，否则为0
endmodule
