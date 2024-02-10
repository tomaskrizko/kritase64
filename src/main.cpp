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
	std::cout << "If no FILE or base64 string is provided, input is taken from standard input.\n";
	std::cout << "Output is to standard output." << std::endl;
}

template <class T>
std::basic_string<T> everythingFromIO(std::istream& stream)
{
	std::basic_string<T> res;
	while (!stream.eof())
	{
		T byte;
		stream.read((char*)&byte, 1);
		if (!stream.eof()) res.push_back(byte);
	}
	return res;
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
		kritase64::Buffer data;
		if (argc == 2)
		{
			data = everythingFromIO<uint8_t>(std::cin);
		}
		else
		{
			std::ifstream file(argv[2], std::ios::in | std::ios::binary);
			data = everythingFromIO<uint8_t>(file);
		}
		stream.write((char*)data.data(), data.size());
		std::cout << stream.base64();
	}
	else if (argv[1] == (std::string)"decode")
	{
		std::string base64;
		if (argc == 2)
		{
			base64 = everythingFromIO<char>(std::cin);
		}
		else
		{
			base64 = argv[2];
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
