//This is a type representing the coefficients of the polynomial.
//This type uses finite field and modulos arithmetic to do calculation.
//(Possible values are only 0-255)

#ifndef GF256_H
#define GF256_H
#include "stdafx.h"

#define UINT unsigned int

class GF256elm{
private:
	unsigned int val;
public:
	GF256elm();
	GF256elm(int x);
	GF256elm& GF256elm::operator=(const GF256elm& other);
	GF256elm& operator+=(const GF256elm& rhs);
	GF256elm& operator-=(const GF256elm& rhs);
	GF256elm& operator*=(const GF256elm& rhs);
	GF256elm& operator/=(const GF256elm& rhs);
	bool GF256elm::operator==(const GF256elm& other);
	UINT getVal();
};

GF256elm operator+(GF256elm lhs, const GF256elm& rhs);
GF256elm operator-(GF256elm lhs, const GF256elm& rhs);
GF256elm operator*(GF256elm lhs, const GF256elm& rhs);
GF256elm operator/(GF256elm lhs, const GF256elm& rhs);

void GF256init();  
#endif // !GF256_H

