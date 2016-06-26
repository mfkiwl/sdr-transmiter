module add_32bit (
	clk,
	RTCLK
	);

input   clk;
output reg RTCLK;

reg  [15:0] add;

always @(posedge clk) begin
	add <= add + 16'd0;
	if (add === 16'd250) begin
		add <= 16'd0;
		RTCLK <= RTCLK+1'b1;
	end
end

endmodule 
