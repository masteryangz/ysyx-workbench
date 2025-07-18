module adder_5bit_signed (
    input  wire [4:0] A,
    input  wire [4:0] B,
    input  wire       Cin,
    output reg  [4:0] Sum,
    output reg        Overflow,
    output reg        CarryOut
);
    wire [4:0] t;
    wire [5:0] temp_result;
    wire       temp_carry;

    assign t = ( {5{Cin}} ^ B );
    assign temp_result = A + t + {4'b0000, Cin};
    //assign t = ( {5{Cin}} ^ B ) + {4'b000,Cin};
    //assign temp_result = A + t;
    assign temp_carry = temp_result[5];

    always @(*) begin
        Sum = temp_result[4:0];
        Overflow = (A[4] == t[4]) && (temp_result[4] != A[4]);
        CarryOut = temp_carry;
    end

endmodule
