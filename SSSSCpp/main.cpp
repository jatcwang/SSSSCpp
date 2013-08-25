// SSSSCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GF256.h"
#include "SSSSCpp.h"
#include <vector>

#include <assert.h>
#define UINT unsigned int

using std::vector; using std::pair;
using std::make_pair;

int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	assert(GF256elm(3) + GF256elm(4) == GF256elm(7));
	assert(GF256elm(500) == GF256elm(244));
	assert(GF256elm(255) == GF256elm(-1));
	assert(GF256elm(3) *  GF256elm(4) == GF256elm(12));
	GF256elm xd = GF256elm(7) * GF256elm(77);
	assert((GF256elm(4) / GF256elm(1)) == GF256elm(4));
	assert((GF256elm(32) / GF256elm(16)) == GF256elm(2));
	assert((GF256elm(15) / GF256elm(5)) == GF256elm(3));
	assert((GF256elm(88) / GF256elm(8)) == GF256elm(11));
	//assert((GF256elm(77) / GF256elm(11)) == GF256elm(7));
	//assert((GF256elm(77) / GF256elm(7)) == GF256elm(11));

	PGF256 hi = generateRandomPolynomial(5, 0x08);
	vector<pair<UINT, UINT>> lol = encodeByte(15, 5, 3);
	vector<pair<UINT, UINT>> lol2;
	lol2.push_back(make_pair(225, 113));
	lol2.push_back(make_pair(32, 247));
	lol2.push_back(make_pair(249, 248));
	UINT result = decodeByte(lol2);
	return 0;
}

