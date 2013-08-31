#include "stdafx.h"
#include "SSSSCpp.h"


using std::vector; using std::pair;
using std::make_pair;
using std::cout; using std::endl;
using std::string; using std::ifstream;
using std::ofstream; using std::ios;
PGF256 generateRandomPolynomial(UINT degree, GF256elm secret) {
	vector<GF256elm> coeffs(degree + 1);
	coeffs[0] = secret;

	for (UINT i = 1; i < degree; i++) {
		coeffs[i] = GF256elm(rand() % 256);
	}

	//pick a non zero coefficient for the highest degree
	coeffs[degree] = GF256elm(rand() % 255 + 1);
	return PGF256(coeffs);
}

vector<pair<UINT, UINT>> encodeByte(UINT byte, int n, int k) {
	vector<bool> picked(256, false); //initialise vector with all false

	PGF256 poly = generateRandomPolynomial(k - 1, GF256elm(byte));
	vector<pair<UINT, UINT>> result;

	for (int i = 0; i < n; i++) {
		UINT x;
		do { //pick a random number thath hasn't been used before
			x = rand() %255 + 1; //generate random number between 1 and 255 inclusive
		}while (picked[x] == true);
		picked[x] = true;

		//calculate y from x using the random polynomial generated before
		UINT y = poly.compute(GF256elm(x)).getVal();

		//add this pair to the vector of pairs
		result.push_back(std::make_pair(x, y));
	}
	return result;
}

//encode byte using predefined x's
vector<pair<UINT, UINT>> encodeByte(UINT byte, vector<UINT>& xs, int k) {
	//we're assuming xs contains no duplicates here thus not doing error check
	PGF256 poly = generateRandomPolynomial(k - 1, GF256elm(byte));
	size_t n = xs.size();
	vector<pair<UINT, UINT>> result(n);

	//iterate through all xs and calculate the y values for each x
	for (int i = 0; i < n; i++) {
		UINT y = poly.compute(GF256elm(xs[i])).getVal();
		result[i] = make_pair(xs[i], y);
	}
	return result;
}

UINT decodeByte(vector<pair<UINT, UINT>> keys) {
	size_t numKeys = keys.size();
	//extract the x's and y's from the vector
	vector<int> x;
	vector<int> y;
	for (int i = 0; i < numKeys; i++) {
		x.push_back(keys[i].first); //extract x
		y.push_back(keys[i].second); //extract y
	}

	//now calculate the constant value (which is the secret) using
	//a simplified equation of Lagrange's Interpolation technique
	//(It's simplified because we don't need to know what the polynomial is,
	//just the constant itself, which can be easily deduced.
	GF256elm result(0);
	for (int i = 0; i < numKeys; ++i) {
		//calculate the constant term of lagrange interpolation polynomial
		GF256elm l(1);
		for (int j = 0; j < numKeys; ++j) {
			if (i == j)
				continue;
			GF256elm nxj = GF256elm(x[j]);
			GF256elm xi = GF256elm(x[i]);
			GF256elm xj = GF256elm(x[j]);
			GF256elm ximxj = xi - xj;
			GF256elm prod = nxj / ximxj;
			l *= prod;
		}
		GF256elm product = GF256elm(y[i]) * l;
		result += product;
	}
	return result.getVal();
}

UINT decodeByte(vector<UINT> xs, vector<UINT> ys) {
	if (xs.size() != ys.size())
		throw;
	size_t numKeys = xs.size();

	//now calculate the constant value (which is the secret) using
	//a simplified equation of Lagrange's Interpolation technique
	//(It's simplified because we don't need to know what the polynomial is,
	//just the constant itself, which can be easily deduced.
	GF256elm result(0);
	for (int i = 0; i < numKeys; ++i) {
		//calculate the constant term of lagrange interpolation polynomial
		GF256elm l(1);
		for (int j = 0; j < numKeys; ++j) {
			if (i == j)
				continue;
			GF256elm nxj = GF256elm(xs[j]);
			GF256elm xi = GF256elm(xs[i]);
			GF256elm xj = GF256elm(xs[j]);
			GF256elm ximxj = xi - xj;
			GF256elm prod = nxj / ximxj;
			l *= prod;
		}
		GF256elm product = GF256elm(ys[i]) * l;
		result += product;
	}
	return result.getVal();
}

void splitSecretFile(boost::filesystem::path pathToFile, int n, int k) {
	vector<string> outFileNames(n);
	string inFileName = pathToFile.string();

	//generate the output files' names
	for (int i = 0; i < n; ++i) {
		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') << i + 1;
		outFileNames[i] = inFileName + "." + ss.str();
	}

	ifstream inFile;
	char* memBlock = NULL;
	vector<ofstream> outFiles(n);

	try {
		inFile.open(inFileName, ios::binary | ios::ate);
		if (!inFile.is_open())
			throw std::exception("Error in opening input file, please make sure the file exists and is not in use");
		for (int i = 0; i < n; ++i) {
			outFiles[i].open(outFileNames[i], ios::binary);
			if (!outFiles[i].is_open())
				throw std::exception("One of the output files cannot be opened for writing");
		}

		//read the input file into memory and close it
		int fileSize = (int) inFile.tellg(); //handles up to 2GB file size
		memBlock = new char[fileSize];
		inFile.seekg(0, ios::beg);
		inFile.read(memBlock, fileSize);
		inFile.close();

		vector<UINT> xs(n);
		for (int i = 0; i < n; ++i) {
			xs[i] = i + 1; // x points are 1 to n inclusive
		}
		for (int i = 0; i < fileSize; ++i) {
			//cout << (double)i/fileSize * 100 << endl;
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
	catch (std::exception &e) { //close all files and free memory if exception happens
		if (inFile.is_open()) {
			inFile.close();
		}
		for (int i = 0; i < n; i++) {
			if (outFiles[i].is_open()) {
				outFiles[i].close();
			}
		}
		if (memBlock != NULL) 
			delete[] memBlock;
		throw std::exception(e.what());
	}
}

void reconstructSecretFile(std::vector<boost::filesystem::path> pathToFiles, boost::filesystem::path outputPath) {
	//check pathToFiles is not empty
	if (pathToFiles.empty()) 
		throw std::exception("No valid input file found");
	if (outputPath.string() == "")
		throw std::exception("No output file specified");
	//split the file names into its stem and extension
	//example.exe.001 will split into "example.exe" and ".001"
	size_t k = pathToFiles.size();
	vector<string> filePaths(k);
	vector<string> fileNames(k);
	vector<string> fileExtensions(k);
	for (int i = 0; i < k; ++i) {
		filePaths[i] = pathToFiles[i].string();
		fileNames[i] = pathToFiles[i].stem().string();
		fileExtensions[i] = pathToFiles[i].extension().string();
	}

	vector<ifstream> inputStreams(k);
	vector<UINT> xs; //vector of xs

	for (int i = 0; i < k; ++i) {
		string fileNumber = fileExtensions[i].substr(1, 3);
		UINT x = (UINT)atoi(fileNumber.c_str());
		xs.push_back(x);
		inputStreams[i].open(filePaths[i] ,ios::binary | ios::ate);
	}

	//copies the data in each file shares into memory
	int fileSize = (int) inputStreams[0].tellg(); //handles up to 2GB file size
	char** inputMemBlock;
	inputMemBlock = new char*[k];
	for (int i = 0; i < k; ++i) {
		inputMemBlock[i] = new char[fileSize];
		inputStreams[i].seekg(0, ios::beg);
		inputStreams[i].read(inputMemBlock[i], fileSize);
		inputStreams[i].close();
	}

	//now decode and reconstruct the original file
	ofstream outputStream;
	outputStream.open(outputPath.string(), ios::binary);
	char* outputMemBlock = new char[fileSize];

	//collect the bytes from each file and reconstruct 
	//their original secret byte
	for (int i = 0; i < fileSize; ++i) {
		vector<UINT> reconYs(k);
		for (int j = 0; j < k; ++j) {
			reconYs[j] = inputMemBlock[j][i]; //read current byte
		}
		UINT secretByte = decodeByte(xs, reconYs);
		outputMemBlock[i] = secretByte;
	}
	outputStream.write(outputMemBlock, fileSize);

	//close files
	outputStream.close();

	//free memories
	for (int i = 0; i < k; ++i) {
		delete[] inputMemBlock[i];
	}
	delete[] inputMemBlock;
	delete[] outputMemBlock;
}

void reconstructSecretFile(boost::filesystem::path pathToFile, boost::filesystem::path outputPath) {
	namespace fs = boost::filesystem;
	fs::path directory(pathToFile.parent_path());
	if (directory.string() == "") //if input argument doesn't contain folder path, set it as current directory
		directory = ".";
	fs::directory_iterator endIterator;

	vector<fs::path> inputPaths;
	string inputFileName = pathToFile.filename().string();

	for (fs::directory_iterator it(directory); it != endIterator; ++it) {
		if (fs::is_regular_file(it->status())) { //is a file
			fs::path currentFile = it->path();

			string fileName = currentFile.stem().string();
			if (fileName != inputFileName)
				continue;

			string extension = currentFile.extension().string();
			if (extension.size() != 4) // extension is of the form ".001", so size must be 4
				continue;

			extension = extension.substr(1, 3); //get "001" from ".001"
			try {
				int i = boost::lexical_cast<int>(extension);
				if (i < 1 || i > 255) //file number must be between 001 and 255 (inclusive)
					continue;
			}
			catch (boost::bad_lexical_cast _) {
				continue;
			}
			//test passed, add this file to the list of input files
			inputPaths.push_back(currentFile);
		}
	}
	reconstructSecretFile(inputPaths, outputPath);
}