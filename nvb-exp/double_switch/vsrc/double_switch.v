module double_switch(
  input a,
  input b,
  output f
);
  assign f = a ^ b;
//  initial begin
//    $dumpfile("dump.fst");
//    $dumpvars(0, double_switch);
//  end
endmodule
