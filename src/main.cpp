#include <cstdint>
#include <iostream>
#include "kritase64.hpp"
#include <memory.h>
#include <fstream>
#include <sstream>

void usage()
{
	std::cout << "Usage:\n";
	std::cout << "kritase64 encode [FILE]\n";
	std::cout << "kritase64 decode [base64 string]\n";
	std::cout << "If no FILE or base64 string is provided, input is taken from standard input. Output is to standard output." << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		usage();
		return 0;
	}

	if (argv[1] == (std::string)"encode")
	{
		kritase64::Stream stream("", std::ios::out | std::ios::binary);
		while (!std::cin.eof())
		{
			char c;
			std::cin.read(&c, 1);
			if (!std::cin.eof()) stream.write(&c, 1);
		}
		std::cout << stream.base64();
	}
	else if (argv[1] == (std::string)"decode")
	{
		std::string base64;
		while (!std::cin.eof())
		{
			char c;
			std::cin.read(&c, 1);
			if (!std::cin.eof()) base64.push_back(c);
		}
		kritase64::Buffer data = kritase64::decode(base64);
		std::cout.write((char*)data.data(), data.size());
	}
	else
	{
		usage();
	}
	return 0;
}
