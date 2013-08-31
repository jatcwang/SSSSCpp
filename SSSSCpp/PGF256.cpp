//This class represents polynomial consisting of only elements from GF(2^8) (0 - 255)

#include "stdafx.h"
#include "PGF256.h"
#include "GF256.h"
#include <vector>

using std::vector;
PGF256::PGF256(vector<GF256elm> coeffs) {
	this->coeffs = coeffs;
}

//Get a particular coefficient
GF256elm PGF256::getCoeff(int i) {
	return coeffs[i];
}

//Get all the coefficients
vector<GF256elm> PGF256::getCoeff() {
	return coeffs;
}

//Get the degree of polynomial
UINT PGF256::getDegree() {
	return (UINT)(coeffs.size() - 1);
}

//compute the y value from given x based on the polynomial
//Polynomial is of the form y(x) = D + a(1)*x^1 + a(2)*x^2 + .....
//D is the secret byte
GF256elm PGF256::compute(GF256elm x) {
	GF256elm result = GF256elm(coeffs[0]); //initialize the result with the constant (x^0), which is the secret
	GF256elm xPower = GF256elm(1); //the term that calculates the power of x
	for (int i = 1; i < coeffs.size(); ++i) {
		xPower *= x;
		result += xPower * GF256elm(coeffs[i]); //add the current a(i) * x^i term to the result
	}
	return result;
}