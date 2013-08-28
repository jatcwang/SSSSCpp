#include "stdafx.h"
#include "SSSSCpp.h"


using std::vector; using std::pair;
using std::make_pair;
PGF256 generateRandomPolynomial(UINT degree, GF256elm secret) {
	vector<GF256elm> coeffs;
	coeffs.push_back(secret);

	for (UINT i = 1; i < degree; i++) {
		coeffs.push_back(GF256elm(rand() % 256));
	}

	//pick a non random coefficient for the highest degree
	coeffs.push_back(GF256elm(rand() % 256));
	return PGF256(coeffs);
}

vector<pair<UINT, UINT>> encodeByte(UINT byte, int n, int k) {
	vector<bool> picked(256, false); //initialise vector with all false

	PGF256 poly = generateRandomPolynomial(k - 1, GF256elm(byte));
	vector<pair<UINT, UINT>> result;

	for (int i = 0; i < n; i++) {
		UINT x;
		do { //pick a random number thath hasn't been used before
			x = rand() %255 + 1; //generate random number between 1 and 255 inclusive
		}while (picked[x] == true);
		picked[x] = true;

		//calculate y from x using the random polynomial generated before
		UINT y = poly.compute(GF256elm(x)).getVal();

		//add this pair to the vector of pairs
		result.push_back(std::make_pair(x, y));
	}
	return result;
}

//encode byte using predefined x's
vector<pair<UINT, UINT>> encodeByte(UINT byte, vector<UINT> xs, int k) {
	//we're assuming xs contains no duplicates here thus not doing error check
	PGF256 poly = generateRandomPolynomial(k - 1, GF256elm(byte));

	vector<pair<UINT, UINT>> result;

	//iterate through all xs and calculate the y values for each x
	for (vector<UINT>::const_iterator it = xs.begin(); it != xs.end(); ++it) {
		UINT y = poly.compute(GF256elm(*it)).getVal();
		result.push_back(make_pair(*it, y));
	}
	return result;
}

UINT decodeByte(vector<pair<UINT, UINT>> keys) {
	size_t numKeys = keys.size();
	//extract the x's and y's from the vector
	vector<int> x;
	vector<int> y;
	for (int i = 0; i < numKeys; i++) {
		x.push_back(keys[i].first); //extract x
		y.push_back(keys[i].second); //extract y
	}

	//now calculate the constant value (which is the secret) using
	//a simplified equation of Lagrange's Interpolation technique
	//(It's simplified because we don't need to know what the polynomial is,
	//just the constant itself, which can be easily deduced.
	GF256elm result(0);
	for (int i = 0; i < numKeys; ++i) {
		//calculate the constant term of lagrange interpolation polynomial
		GF256elm l(1);
		for (int j = 0; j < numKeys; ++j) {
			if (i == j)
				continue;
			GF256elm nxj = GF256elm(x[j]);
			GF256elm xi = GF256elm(x[i]);
			GF256elm xj = GF256elm(x[j]);
			GF256elm ximxj = xi - xj;
			GF256elm prod = nxj / ximxj;
			l *= prod;
		}
		GF256elm product = GF256elm(y[i]) * l;
		result += product;
	}
	return result.getVal();
}

UINT decodeByte(vector<UINT> xs, vector<UINT> ys) {
	if (xs.size() != ys.size())
		throw;
	size_t numKeys = xs.size();

	//now calculate the constant value (which is the secret) using
	//a simplified equation of Lagrange's Interpolation technique
	//(It's simplified because we don't need to know what the polynomial is,
	//just the constant itself, which can be easily deduced.
	GF256elm result(0);
	for (int i = 0; i < numKeys; ++i) {
		//calculate the constant term of lagrange interpolation polynomial
		GF256elm l(1);
		for (int j = 0; j < numKeys; ++j) {
			if (i == j)
				continue;
			GF256elm nxj = GF256elm(xs[j]);
			GF256elm xi = GF256elm(xs[i]);
			GF256elm xj = GF256elm(xs[j]);
			GF256elm ximxj = xi - xj;
			GF256elm prod = nxj / ximxj;
			l *= prod;
		}
		GF256elm product = GF256elm(ys[i]) * l;
		result += product;
	}
	return result.getVal();
}