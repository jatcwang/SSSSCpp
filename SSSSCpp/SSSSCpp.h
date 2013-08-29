#ifndef SSSSCPP_H
#define SSSSCPP_H

#include "stdafx.h"
#include "PGF256.h"
#include "GF256.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "boost/lexical_cast.hpp"

#define UINT unsigned int

PGF256 generateRandomPolynomial(unsigned int degree, GF256elm secret);
std::vector<std::pair<UINT, UINT>> encodeByte(UINT byte, int n, int k);
std::vector<std::pair<UINT, UINT>> encodeByte(UINT byte, std::vector<UINT> xs, int k);
UINT decodeByte(std::vector<std::pair<UINT, UINT>> keys);
UINT decodeByte(std::vector<UINT> xs, std::vector<UINT> ys);
void splitSecretFile(boost::filesystem::path pathToFile, int n, int k);
void reconstructSecretFile(std::vector<boost::filesystem::path> pathToFiles,
						   boost::filesystem::path outputPath);
void reconstructSecretFile(boost::filesystem::path pathToFile,
						   boost::filesystem::path outputPath);


#endif