// SSSSCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GF256.h"
#include "SSSSCpp.h"
#include <vector>
#include <stdio.h>
#include <iostream>

#include <assert.h>
#define UINT unsigned int

using std::vector; using std::pair;
using std::make_pair;

void testF() {
	GF256elm a = GF256elm(34) * GF256elm(3);
	GF256elm b = (GF256elm(2) - GF256elm(1)) * (GF256elm(2) - GF256elm(3));
	GF256elm c = a / b;
	std::cout << c.getVal() << std::endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	testF();
	PGF256 hi = generateRandomPolynomial(5, 0x08);
	vector<pair<UINT, UINT>> lol = encodeByte(15, 5, 3);
	vector<pair<UINT, UINT>> lol2;
	lol2.push_back(make_pair(1, 17));
	lol2.push_back(make_pair(2, 34));
	lol2.push_back(make_pair(3, 61));
	//lol2.push_back(make_pair(225, 113));
	//lol2.push_back(make_pair(32, 247));
	//lol2.push_back(make_pair(249, 248));
	UINT result = decodeByte(lol2);
	return 0;
}

