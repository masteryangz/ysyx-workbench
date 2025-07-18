module ps2_keyboard(clk,clrn,ps2_clk,ps2_data,data,sampling,
                    ready,nextdata_n,ledr,overflow);
    input clk,clrn,ps2_clk,ps2_data;
    input nextdata_n;
    //output [7:0] data;
    output reg [7:0] data;
    output sampling;
    output reg ready;
    output [2:0] ledr;       // led for debug
    output reg overflow;     // fifo overflow
    // internal signal, for test
    reg [9:0] buffer;        // ps2_data bits
    reg [7:0] fifo[7:0];     // data fifo
    reg [2:0] w_ptr,r_ptr;   // fifo write and read pointers
    reg [3:0] count;  // count ps2_data bits
    // detect falling edge of ps2_clk
    reg [2:0] ps2_clk_sync;

    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    always @(posedge clk) begin
        if (clrn == 0) begin // reset
            count <= 0; w_ptr <= 0; r_ptr <= 0; overflow <= 0; ready<= 0;
        end
        else begin
            if ( ready ) begin // read to output next data
                if(nextdata_n == 1'b0) //read next data
                begin
                    data <= fifo[r_ptr];
                    //$display("r_ptr: %d, fifo[r_ptr]: %h, data: %h", r_ptr, fifo[r_ptr], data);
                    r_ptr <= r_ptr + 3'b1;
                    if(w_ptr==(r_ptr+1'b1)) //empty
                        ready <= 1'b0;
                end
            end
            if (sampling && !overflow) begin
                if (count == 4'd10) begin
                    if ((buffer[0] == 0) &&  // start bit
                        (ps2_data)       &&  // stop bit
                        (^buffer[9:1])) begin      
                        fifo[w_ptr] <= buffer[8:1];  // kbd scan code
                        //$display("w_ptr: %d, fifo[w_ptr]: %h", w_ptr, fifo[w_ptr]);
                        w_ptr <= w_ptr+3'b1;
                        ready <= 1'b1;
                        overflow <= overflow | (r_ptr == (w_ptr + 3'b1));
                        //$display("ready: %b, overflow: %b", ready, overflow);
                    end
                    count <= 0;     // for next
                end else begin
                    buffer[count] <= ps2_data;  // store ps2_data
                    count <= count + 3'b1;
                end
            end
        end
    end
    //assign data = fifo[r_ptr]; //always set output data
    assign ledr = {overflow, sampling, ready}; // led for debug

endmodule