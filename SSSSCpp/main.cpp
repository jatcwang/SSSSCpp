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
#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"

#include <assert.h>
#define UINT unsigned int

namespace 
{ 
	const size_t ERROR_IN_COMMAND_LINE = 1; 
	const size_t SUCCESS = 0; 
	const size_t ERROR_UNHANDLED_EXCEPTION = 2; 

} // namespace 

using std::vector; using std::string;
using boost::filesystem::path;

int main(int argc, char* argv[])
{
	GF256init();
	srand(time(NULL));

	try 
	{ 
		/** Define and parse the program options 
		*/ 
		namespace po = boost::program_options; 
		po::options_description desc("Program Usage"); 
		desc.add_options() 
			("help,h", "Print help messages") 
			("split,s", po::value<vector<string>>()->multitoken(), "Split a file")
			("reconstruct,r", po::value<vector<string>>()->multitoken(), "Reconstruct a file from its shares");
		//("reconstruct,r", po::value<vector<string>>(&reconInputPaths), "Reconstruct a file from its shares");
		//("options,o", "additional options") 

		po::variables_map vm; 
		try 
		{ 
			po::store(po::parse_command_line(argc, argv, desc),
				vm); // throws on error 

			/** --help option 
			*/ 
			if (vm.count("help")) 
			{ 
				std::cout << "Basic Command Line Parameter App" << std::endl 
					<< desc << std::endl; 
				return SUCCESS; 
			} 

			if (vm.count("split")) {
				std::cout << "Splitting" << std::endl;
				vector<string> const &splitInputs = vm["split"].as<vector<string>>();
				if (splitInputs.size() != 3) {
					throw po::error("Split usage: [Input File] [Total Share #] [Share Number Required to Reconstruct]");
				}
				path inputPath(splitInputs[0]);
				try {
					int n = boost::lexical_cast<int>(splitInputs[1].c_str());
					int k = boost::lexical_cast<int>(splitInputs[2].c_str());
					if (n <= 0 || k <= 0 || n > 255 || k > 255) {
						throw po::error("n and k must be between 1 and 255 (inclusive)");
					}
					if (n < k) {
						throw po::error("n must be larger or equal to k");
					}
					splitSecretFile(inputPath, n, k);
				}
				catch (boost::bad_any_cast &e) {
					throw po::error("[Total Share Number] and [Share Number Required to Reconstruct] must be numbers");
				}
				catch (std::exception &e) {
					throw po::error(e.what());
				}
				return SUCCESS;
			}

			if (vm.count("reconstruct")) {
				std::cout << "Reconstructing" << std::endl;
				vector<string> const &reconInputs = vm["reconstruct"].as<vector<string>>();
				if (reconInputs.size() != 2)
					throw po::error("Reconstruct Usage: [Original File Name (and the folder path containing the shares)] [Output File Name (and folder path)]");
				path inputFile(reconInputs[0]);
				path outputFile(reconInputs[1]);
				try {
					reconstructSecretFile(inputFile, outputFile);
				}
				catch (std::exception &e) {
					throw po::error(e.what());
				}
			}

			po::notify(vm); // throws on error, so do after help in case there are any problems 
		} 
		catch(po::error& e) 
		{ 
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
			std::cerr << desc << std::endl; 
			return ERROR_IN_COMMAND_LINE; 
		} 

		// application code here // 

	} 
	catch(std::exception& e) 
	{ 
		std::cerr << "Unhandled Exception reached the top of main: " 
			<< e.what() << ", application will now exit" << std::endl; 
		return ERROR_UNHANDLED_EXCEPTION; 

	} 

	return SUCCESS; 

	return 0;
}