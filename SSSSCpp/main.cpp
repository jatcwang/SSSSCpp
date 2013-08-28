// SSSSCpp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "GF256.h"
#include "SSSSCpp.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "boost/filesystem.hpp"

#include <assert.h>
#define UINT unsigned int

using std::vector; using std::pair;
using std::make_pair;
using std::ifstream; using std::ofstream;
using std::ios; using std::string;
using std::cout; using std::endl;
int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	srand(time(NULL));
	int n = 5;
	int k = 3;
	string folder("../../test/");
	string inputFileName("input.jpg");
	string reconstructFileName("output.jpg");
	boost::filesystem::path inputFile(folder + inputFileName);
	boost::filesystem::path outputPath(folder + reconstructFileName);
	vector<boost::filesystem::path> shareFiles(n);
	for (int i = 0; i < n; ++i) {
		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') << i + 1;
		shareFiles[i] = inputFile.string() + "." + ss.str();
	}
	cout << "Splitting" <<endl;
	splitSecretFile(inputFile, n, k);

	//remove 2 files
	shareFiles.erase(shareFiles.begin(), shareFiles.begin() + 2);
	cout << "Reconstructing" << endl;
	reconstructSecretFile(shareFiles, outputPath);
	
	return 0;
}