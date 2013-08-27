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

int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	PGF256 hi = generateRandomPolynomial(5, 0x08);
	vector<pair<UINT, UINT>> lol = encodeByte(15, 5, 3);
	vector<pair<UINT, UINT>> lol2;
	lol2.push_back(make_pair(1, 13));
	lol2.push_back(make_pair(2, 26));
	lol2.push_back(make_pair(3, 29));
	UINT result = decodeByte(lol2);
	return 0;
}

