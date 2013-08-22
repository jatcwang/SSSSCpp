#ifndef PGF256_H
#define PGF256_H
#include "stdafx.h"
#include <vector>
#include "GF256.h"

class PGF256 {
private:
	std::vector<GF256elm> coeffs;
public:
	PGF256(std::vector<GF256elm> coeffs);

	//Get a particular coefficient
	GF256elm getCoeff(int i);

	//Get all the coefficients
	std::vector<GF256elm> getCoeff();

	//Get the degree of polynomial
	unsigned int getDegree();

	//compute the y value from given x based on the polynomial
	//Polynomial is of the form y(x) = D + a(1)*x^1 + a(2)*x^2 + .....
	//D is the secret byte
	GF256elm compute(GF256elm x);
};  
#endif // !PGF256_H
