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
	//vector<pair<UINT, UINT>> lol = encodeByte(16, 5, 3);
	//UINT result = decodeByte(lol); //result == 15
	while (1) {
		time_t num = rand() % 255 + 1;
		vector<pair<UINT, UINT>> lol = encodeByte(num, 5, 3);
		vector<pair<UINT, UINT>> lol2(lol.begin(), lol.begin() + 3);
		UINT result = decodeByte(lol2);
		std::cout << num << "\t" << result << std::endl;
	}
	//for (int num = 1; num < 256; ++num) {
	//	vector<pair<UINT, UINT>> lol = encodeByte(num, 5, 3);
	//	vector<pair<UINT, UINT>> lol2(lol.begin(), lol.begin() + 3);
	//	UINT result = decodeByte(lol2);
	//	if (result != num)
	//		std::cout << num << "\t" << result << std::endl;
	//}
	return 0;
}

