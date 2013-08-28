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
	srand(clock());
	vector<UINT> xs(5, 0);
	for (int i = 0; i < 5; ++i) {
		xs[i] = i + 1;
	}
	vector<pair<UINT, UINT>> yo = encodeByte(15, xs, 3);
	UINT result = decodeByte(yo);
	return 0;
}

