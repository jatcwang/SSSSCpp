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
	vector<pair<UINT, UINT>> lol = encodeByte(15, 5, 3);
	UINT result = decodeByte(lol); //result == 15
	return 0;
}

