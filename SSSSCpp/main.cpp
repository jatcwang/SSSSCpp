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

void split();
void reconstruct();

int _tmain(int argc, _TCHAR* argv[])
{
	GF256init();
	srand(time(NULL));

	split();
	reconstruct();


	return 0;
}

int n = 5;
int k = 3;
string inFileName("../../test/output.txt");
vector<string> outFileNames(n);

void split() {

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
}

void reconstruct() {
	//now open the output files and attempt to reconstructs
	//take the first k files
	string reconOutputFileName("../../test/myOutput.txt");
	vector<string> reconInFileNames(outFileNames.begin(), outFileNames.begin() + k);
	vector<ifstream> reconInFiles(k);
	vector<UINT> reconXs;

	size_t numFiles = reconInFileNames.size();

	for (int i = 0; i < numFiles; ++i) {
		string hi = boost::filesystem::path(reconInFileNames[i]).extension().string();
		string hi2 = hi.substr(1, hi.size() - 1);
		UINT hi3 = (UINT)atoi(hi2.c_str());
		reconXs.push_back(hi3);
		reconInFiles[i].open(reconInFileNames[i], ios::binary | ios::ate);
	}

	int reconFileSize = (int) reconInFiles[0].tellg();
	char** reconMemBlock;
	reconMemBlock = new char*[k];
	for (int i = 0; i < numFiles; ++i) {
		reconMemBlock[i] = new char[reconFileSize];
		reconInFiles[i].seekg(0, ios::beg);
		reconInFiles[i].read(reconMemBlock[i], reconFileSize);
		reconInFiles[i].close();
	}

	//now decode and reconstruct the original file
	ofstream reconOutputFile;
	reconOutputFile.open(reconOutputFileName, ios::binary);
	char* outputMemBlock = new char[reconFileSize];

	//collect the bytes from each file and reconstruct 
	//their original secret byte
	for (int i = 0; i < reconFileSize; ++i) {
		vector<UINT> reconYs(numFiles);
		for (int j = 0; j < numFiles; ++j) {
			reconYs[j] = reconMemBlock[j][i]; //read current byte
		}
		UINT secretByte = decodeByte(reconXs, reconYs);
		outputMemBlock[i] = secretByte;
	}
	reconOutputFile.write(outputMemBlock, reconFileSize);

	//close files
	reconOutputFile.close();

	//free memories
	for (int i = 0; i < numFiles; ++i) {
		delete[] reconMemBlock[i];
	}
	delete[] reconMemBlock;
	delete[] outputMemBlock;
}