module fsm (
  input clk,
  input rst,
  input ready,
  input overflow,
  input [7:0] data,
  input [3:0] hex0,
  input [3:0] hex1,
  input [3:0] hex2,
  input [3:0] hex3,
  output reg [4:0] h0,
  output reg [4:0] h1,
  output reg [4:0] h2,
  output reg [4:0] h3,
  output [4:0] h4,
  output [4:0] h5,
  output nextdata_n
);

reg [7:0] count; // Count for total number of keys pressed
reg [7:0] tmp_data; // Temporary data storage
//reg full_cycle;
reg pressed;
reg [7:0] prev;

// State encoding (binary)
typedef enum reg [2:0] {
    S0 = 3'b001,
    S1 = 3'b011,
    S2 = 3'b111,
    S3 = 3'b110,
    S4 = 3'b100
} state_t;

state_t current_state;
/*
always @(*) begin
    h0 = 5'b10000;
    h1 = 5'b10000;
    h2 = 5'b10000;
    h3 = 5'b10000;
    count_en = 1'b0; // Default count enable
    count_rst = 1'b0; // Default count reset
    case (current_state)
        S0: begin     // pressed down
            h0 = {1'b0,hex0};
            h1 = {1'b0,hex1};
            h2 = {1'b0,hex2};
            h3 = {1'b0,hex3};
            //next_state = (({hex1,hex0} == 8'b11110000) && !overflow) ? S1 : S0;
            next_state = ({hex1,hex0} == 8'b11110000) ? S1 : S0;
        end
        S1: begin
            //h0 = {5'b000000};
            count_en = 1'b1; // Enable count increment
            next_state = S2;
        end
        S2: begin     // detected break code(F0h)
            //h0 = {5'b000000};
            //$display("ready: %b, overflow: %b", ready, overflow);
            next_state = (ready && !overflow) ? S0 : S2;
        end
        S3: begin    // default case
            next_state = S2;
            count_rst = 1'b1; // Reset count
        end
        default: begin    // default case
            next_state = S2;
            count_rst = 1'b1; // Reset count
        end
    endcase
end
*/
always @(posedge clk or posedge rst) begin
    if (rst) begin
        current_state <= S4;
        nextdata_n <= 1'b1;
        count <= 8'b0;
        tmp_data <= 8'b0;
        //full_cycle <= 1'b1;
        pressed <= 1'b0;
        prev <= 8'b0;
    end 
    /*else if (ready) begin
        current_state <= next_state;
        nextdata_n <= 1'b0;
        if (count_rst)
            count <= 8'b0; // Reset count
        else if (count_en)
            count <= count + 1'b1; // Increment count
    end
    else if (overflow) begin
        current_state <= next_state;
        nextdata_n <= 1'b0;
    end else begin
        current_state <= next_state;
        nextdata_n <= 1'b1;
    end*/
    else begin
        nextdata_n <= (!ready) ? 1'b1 : 1'b0;
        //$display("current_state: %b", current_state);
        case (current_state)
            S0: begin   // up to down
                h0 <= 5'b10000; // Clear output
                h1 <= 5'b10000; // Clear output
                h2 <= 5'b10000; // Clear output
                h3 <= 5'b10000; // Clear output
                //full_cycle <= 1'b1;
                tmp_data <= data;
                current_state <= (tmp_data != data) ? S1 : S0;
                //$display("S0 tmp_data: %h, data: %h", tmp_data, data);
            end
            S1: begin   // pressed down
                h0 <= {1'b0,hex0};
                h1 <= {1'b0,hex1};
                h2 <= {1'b0,hex2};
                h3 <= {1'b0,hex3};
                //tmp_data <= (data != 8'b11110000) ? data : tmp_data;
                //current_state <= (({hex1,hex0} == 8'b11110000) && (tmp_data != data)) ? S2 : S1;
                /*
                if (pressed == 1'b1) begin
                    $display("from S4");
                end
                */
                if (ready) begin
                    if (data == 8'b11110000) begin
                        //$display("found data = %h", data);
                        current_state <= S2;
                    end else begin
                        //$display("hi");
                        current_state <= S1;
                    end
                end
                //current_state <= (data == 8'b11110000) ? S2 : S1;
                //$display("tmp_data: %h, data: %h", tmp_data, data);
            end
            S2: begin   // down to up
                h0 <= {1'b0,hex0};
                h1 <= {1'b0,hex1};
                h2 <= {1'b0,hex2};
                h3 <= {1'b0,hex3};
                //tmp <= data;
                //current_state <= (({hex1,hex0} == 8'b11110000) && (tmp_data == data)) ? S3 : S2;
                current_state <= (data != 8'b11110000) ? S3 : S2;
                //$display("tmp_data: %h, data: %h", tmp_data, data);
                //current_state <= S3;
            end
            S3: begin   // increment state
                h0 <= {1'b0,hex0};
                h1 <= {1'b0,hex1};
                h2 <= {1'b0,hex2};
                h3 <= {1'b0,hex3};
                //full_cycle <= 1'b0;
                //tmp <= tmp_data;
                //count <= (tmp_data != tmp) ? count + 8'b00000001 : count + 8'b00000000;
                count <= count + 1;
                //current_state <= S0;
                //full_cycle <= 1'b1;
                //tmp_data <= 8'b0;
                tmp_data <= data;
                prev <= data;
                current_state <= (prev == data) ? S4 : S0;
                //$display("prev: %h, data: %h", prev, data);
            end
            S4: begin
                h0 <= 5'b10000; // Clear output
                h1 <= 5'b10000; // Clear output
                h2 <= 5'b10000; // Clear output
                h3 <= 5'b10000; // Clear output
                //tmp_data <= data;
                current_state <= (ready == 1'b1) ? S1 : S4;
                /*
                if (ready == 1'b1) begin
                    pressed <= 1'b1;
                end
                */
                //current_state <= S0;
                //$display("tmp_data: %h, data: %h, ready: %b", tmp_data, data, ready);
            end
            default: begin    // default case
                h0 <= 5'b10000; // Clear output
                h1 <= 5'b10000; // Clear output
                h2 <= 5'b10000; // Clear output
                h3 <= 5'b10000; // Clear output
                current_state <= S0;
                count <= 8'b0; // Reset count
                prev <= 8'b0;
                pressed <= 1'b0;
            end
        endcase
    end
end

assign h4 = {1'b0,count[3:0]};
assign h5 = {1'b0,count[7:4]};

endmodule
