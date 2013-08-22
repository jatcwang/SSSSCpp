// SSSSCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GF256.h"

#include <assert.h>


int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	assert(GF256elm(3) + GF256elm(4) == GF256elm(7));
	assert(GF256elm(500) == GF256elm(244));
	assert(GF256elm(3) *  GF256elm(4) == GF256elm(12));
	return 0;
}

