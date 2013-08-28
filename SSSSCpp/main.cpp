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

int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	srand(time(NULL));

	int n = 5;
	int k = 3;

	string inFileName("../../test/output.txt");

	vector<string> outFileNames(n);
	for (int i = 0; i < n; ++i) {
		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') << i + 1;
		outFileNames[i] = inFileName + "." + ss.str();
	}

	ifstream inFile;
	vector<ofstream> outFiles(n);
	inFile.open(inFileName, ios::binary | ios::ate);
	if (!inFile.is_open()) {
		throw;
	}
	for (int i = 0; i < n; ++i) {
		outFiles[i].open(outFileNames[i], ios::binary);
	}

	//read the input file
	char* memBlock;
	int fileSize = (int) inFile.tellg(); //handles up to 2GB file size
	memBlock = new char[fileSize];
	inFile.seekg(0, ios::beg);
	inFile.read(memBlock, fileSize);
	inFile.close();

	vector<UINT> xs(n, 0);
	for (int i = 0; i < n; ++i) {
		xs[i] = i + 1; // x points are 1 to n inclusive
	}
	for (int i = 0; i < fileSize; ++i) {
		vector<pair<UINT, UINT>> points = encodeByte(memBlock[i], xs, k);
		for (int j = 0; j < n; ++j) {
			outFiles[j].write((const char*) &points[j].second, 1);
		}
	}

	//close all opened files and free memory
	delete[] memBlock;
	for (int i = 0; i < n; ++i)
		outFiles[i].close();

	//now open the output files and attempt to reconstructs
	//take the first k files
	string reconOutputFileName("../../test/myOutput.txt");
	vector<string> reconInFileNames(outFileNames.begin(), outFileNames.begin() + k);
	vector<ifstream> reconInFiles(k);
	vector<UINT> reConXs;
	vector<UINT> reConYs;
	for (int i = 0; i < reconInFileNames.size(); ++i) {
		string hi = boost::filesystem::path(reconInFileNames[i]).extension().string();
		string hi2 = hi.substr(1, hi.size() - 1);
		UINT hi3 = (UINT)atoi(hi2.c_str());
		reConXs.push_back(hi3);
		reconInFiles[i].open(reconInFileNames[i]);
	}

	return 0;
}

