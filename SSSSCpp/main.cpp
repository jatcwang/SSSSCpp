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
			("split,s", po::value<vector<string>>()->multitoken(), "Split a file");
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
				path inputPath(splitInputs[0]);
				int n = atoi(splitInputs[1].c_str());
				int k = atoi(splitInputs[2].c_str());
				return SUCCESS;
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