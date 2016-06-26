module sel_wave (
	clk,
	rst_n,
	sel,
	da_ina,
	da_inb,
	da_inc,
	da_out
);


input           clk;
input           rst_n;
input  [2:0]    sel;

input  [7:0]    da_ina;
input  [7:0]    da_inb;
input  [7:0]    da_inc;
output reg  [7:0]  da_out;

always @(posedge clk) begin
	case (sel)
		3'b110 : da_out <= da_ina;
		3'b101 : da_out <= da_inb;
		3'b011 : da_out <= da_inc;
		default : da_out <= da_ina; 
	endcase 
end

endmodule 


