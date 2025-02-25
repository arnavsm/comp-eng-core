`timescale 1ns / 1ps

// Addition Module
module SixteenBitCarryLookAheadCascade(
    input [15 : 0] a,
	 input [15 : 0] b,
	 input Cin,
	 output [16 : 0] sum
	 );
	 
	 wire C1, C2, C3, C4;
	 
	 FourBitCarryLookAheadAdder FA0(a[3 : 0], b[3 : 0], Cin, sum[3 : 0], C1);
	 FourBitCarryLookAheadAdder FA1(a[7 : 4], b[7 : 4], C1, sum[7 : 4], C2);
	 FourBitCarryLookAheadAdder FA2(a[11 : 8], b[11 : 8], C2, sum[11 : 8], C3);
	 FourBitCarryLookAheadAdder FA3(a[15: 12], b[15 : 12], C3, sum[15 : 12], C4);
	 assign sum[16] = C4;

endmodule

module FourBitCarryLookAheadAdder(
    input [3 : 0] a,
	 input [3 : 0] b,
	 input Cin,
	 output [3 : 0] sum,
	 output Cout
	 );
	 
	 wire P0, P1, P2, P3;
	 wire G0, G1, G2, G3;
	 
	 PG_Generator PG1(a[0], b[0], P0, G0);
	 PG_Generator PG2(a[1], b[1], P1, G1);
	 PG_Generator PG3(a[2], b[2], P2, G2);
	 PG_Generator PG4(a[3], b[3], P3, G3);
	 
	 wire C1, C2, C3;
	 
	 assign C1 = G0 | (P0 & Cin);
	 assign C2 = G1 | (P1 & G0) | (P1 & P0 & Cin);
	 assign C3 = G2 | (P2 & G1) | (P2 & P1 & G0) | (P2 & P1 & P0 & Cin);
	 assign Cout = G3 | (P3 & G2) | (P3 & P2 & G1) | (P3 & P2 & P1 & G0) | (P3 & P2 & P1 & P0 & Cin);
	 
	 assign sum[0] = P0 ^ Cin;
	 assign sum[1] = P1 ^ C1;
	 assign sum[2] = P2 ^ C2;
	 assign sum[3] = P3 ^ C3;

endmodule


module PG_Generator(
	 input x,
	 input y,
	 output p,
	 output g
	 );
	 
	 assign p = x ^ y;
	 assign g = x & y;
	 
endmodule
// Addition module ends here

//Multiplication Module
module BoothsMultiplier(
    input signed [15 : 0] X,
	 input signed [15 : 0] Y,
	 output signed [31 : 0] Z
	 );
	 
	 reg signed [31 : 0] Z = 0;
	 reg [1 : 0] temp;
	 integer i; 
	 reg E1;
	 reg [15 : 0] Y1;
	 
	 
	 always@(X or Y)
		begin
			
			Z = 0;
			E1 = 0;
			
			for(i = 0; i < 16; i = i + 1)
				begin
					
					temp = {X[i], E1};
					Y1 = -Y;
					
					case(temp)
						2'd2 : Z [31 : 16] = Z [31 : 16] + Y1;
						2'd1 : Z [31 : 16] = Z [31 : 16] + Y;
						default : begin end
					endcase
					
					Z = Z >> 1;
					Z[31] = Z[30];
					E1 = X[i];
				end
				
				if(Y == 16'd32768)		Z = -Z;
		end

endmodule
// Multiplication Module ends here

// Restoring Division Module starts here
module RestoringDivision(
    input [15 : 0] Dividend,
	 input [15 : 0] Divisor,
	 output [15 : 0] Quotient,
	 output [15 : 0] Remainder
	 );
	 
	 reg [15 : 0] Q, A, M;
	 reg [15 : 0] Quotient = 0;
	 reg [15 : 0] Remainder = 0;
	 integer n;
	 
	 always@(*)
		begin
			A = 0;
			Q = Dividend;
			M = Divisor;
			for(n = 16; n > 0; n = n - 1)
				begin
				
					A = {A[14 : 0], Q[15]};
					Q[15 : 1] = Q[14 : 0];
					A = A - M;
					if(A[15] == 0)			Q[0] = 1;
					else
						begin
							Q[0] = 0;
							A = A + M;
						end
				end
				
		Quotient = Q;
		Remainder = A;
				
		end
		
endmodule
// Restoring Division Module ends here


module ALU(
    input [3 : 0] S,
	 input [15 : 0] A, 
	 input [15 : 0] B,
	 output [31 : 0] Z
	 );
	 
	 reg [31 : 0] Z = 0;
	 wire [16 : 0] add;
	 wire [31 : 0] mult;
	 wire [15 : 0] div, rem;
	 
	 SixteenBitCarryLookAheadCascade inst0(A, B, 0, add);
	 
	 BoothsMultiplier inst1(A, B, mult);
	 
	 RestoringDivision(A, B, div, rem); //How to collect the remainder here?
	 
	 reg [15 : 0] X = 0;
    reg [15 : 0] Y = 0;
		
	 always@(*)
		 begin
			 X = A;
			 Y = B;
			case(S)
				 4'd0 : Z[16 : 0] = add;
				 4'd1 : Z = X - Y;
				 4'd2 : Z = mult;
				 4'd3 : Z[15 : 0] = div;
				 4'd4 : Z = A<<1;
				 4'd5 : Z = A>>1;
				 4'd6 : Z[15 : 0] = {A[14 : 0], A[15]};
				 4'd7 : Z[15 : 0] = {A[0], A[15 : 1]};
				 4'd8 : Z = A & B;
				 4'd9 : Z = A | B;
				 4'd10 : Z = A ^ B;
				 4'd11 : Z = ~(A | B);
				 4'd12 : Z = ~(A & B);
				 4'd13 : Z = ~(A ^ B);
				 4'd14 : Z = (A > B)?32'd1 : 32'd0;
				 4'd15 : Z = (A < B)?32'd1 : 32'd0;
				 default : Z = 0;
			 endcase
			 
		end
		
endmodule