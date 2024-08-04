`include "ALU.v"
module alu_tb;

	// Inputs
	reg [3:0] S;
	reg [15:0] A;
	reg [15:0] B;

	// Outputs
	wire [31:0] Z;

	// Instantiate the Unit Under Test (UUT)
	ALU uut (
		.S(S), 
		.A(A), 
		.B(B), 
		.Z(Z)
	);

	initial begin
	
		S = 0;
		A = 78;
		B = 12;
		// Initialize Inputs
		S = 4'd0;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd1;
		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd2;
		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd3;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd4;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd5;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd6;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd7;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd8;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd9;
		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd10;
		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd11;
		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd12;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd13;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd14;

		// Wait 100 ns for global reset to finish
		#50;
        
		// Add stimulus here
		S = 4'd15;

		// Wait 100 ns for global reset to finish
		#50;

	end
      
endmodule