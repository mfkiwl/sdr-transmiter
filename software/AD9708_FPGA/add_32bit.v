module add_32bit (
	clk,
	rst,
	fr_k,
	adder
);


input   clk;
input   rst;
input  [9:0] fr_k;

output [9:0] adder;

reg  [9:0] add;

always @(posedge clk) begin
	if (!rst) begin
		add <= 10'd0;
	end
	else if (add===10'd1022) begin
		add <= 10'd0;
	end
	else begin
		add <= add + 10'd1;
	end
end

assign   adder = add;

endmodule 
