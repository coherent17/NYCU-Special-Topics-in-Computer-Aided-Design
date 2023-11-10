module AOI22 (N1, N2, N3, N4);
	input N1, N2, N3, N4;
	output OUT1;
	wire w1, w2, w3;

    NANDX1 U1 (.A1(N1), .A2(N2), .ZN(w1));
    NANDX1 U2 (.A1(N3), .A2(N4), .ZN(w2));
    NANDX1 U3 (.A1(w1), .A2(w2), .ZN(w3));
    INVX1 U4 (.I(w3), .ZN(OUT1));
endmodule