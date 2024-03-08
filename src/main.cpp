/*
 *	MIT License
 *
 *	Copyright (c) 2024 kritomas
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *	Source code: www.gitlab.com/kritomas/kritase64
 */

#include <fstream>
#include <iostream>
#include <memory.h>

#include "kritase64.hpp"

void usage()
{
	std::cout << "Usage:\n";
	std::cout << "kritase64 version\n";
	std::cout << "kritase64 encode [FILE] [OUTPUT FILE]\n";
	std::cout << "kritase64 decode [base64 string] [OUTPUT FILE]\n";
	std::cout << "If no FILE or base64 string is provided, input is taken from standard input.\n";
	std::cout << "Output is to OUTPUT FILE, or to standard output, if no OUTPUT FILE is provided." << std::endl;
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
		return kritase64::ERROR_SUCCESS;
	}

	try
	{
		if (argv[1] == (std::string)"version")
		{
			std::cout << PACKAGE_NAME << " v" << VERSION << "\n";
			std::cout << "Copyright (c) 2024 kritomas\n";
			std::cout << "This project is licensed under the MIT License. See source code for details: https://gitlab.com/kritomas/kritase64\n";
			std::cout <<	"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
							"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
							"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
							"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
							"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
							"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
							"SOFTWARE." << std::endl;
		}
		else if (argv[1] == (std::string)"encode")
		{
			kritase64::Stream stream("", std::ios::out | std::ios::binary);
			kritase64::Buffer data;
			if (argc <= 2)
			{
				data = everythingFromIO<uint8_t>(std::cin);
			}
			else
			{
				std::ifstream file(argv[2], std::ios::in | std::ios::binary);
				data = everythingFromIO<uint8_t>(file);
				file.close();
			}
			stream.write((char*)data.data(), data.size());
			if (argc <= 3)
			{
				std::cout << stream.base64();
			}
			else
			{
				std::ofstream output(argv[3], std::ios::out | std::ios::trunc);
				output << stream.base64();
				output.close();
			}
		}
		else if (argv[1] == (std::string)"decode")
		{
			std::string base64;
			if (argc <= 2)
			{
				base64 = everythingFromIO<char>(std::cin);
			}
			else
			{
				base64 = argv[2];
			}
			kritase64::Buffer data = kritase64::decode(base64);
			if (argc <= 3)
			{
				std::cout.write((char*)data.data(), data.size());
			}
			else
			{
				std::ofstream output(argv[3], std::ios::out | std::ios::binary | std::ios::trunc);
				output.write((char*)data.data(), data.size());
				output.close();
			}
		}
		else
		{
			std::cout << "Unknown argument \"" << argv[1] << "\"\n";
			usage();
			return kritase64::ERROR_UNKNOWN;
		}
	}
	catch (const kritase64::Base64Exception& error)
	{
		std::cout << "An error occured: " << error.what() << std::endl;
		return error.type;
	}
	catch (const std::exception& error)
	{
		std::cout << "An unknown error occured: " << error.what() << std::endl;
		return kritase64::ERROR_UNKNOWN;
	}
	catch (...)
	{
		std::cout << "An unknown unspecified error occured." << std::endl;
		return kritase64::ERROR_UNKNOWN;
	}
	return kritase64::ERROR_SUCCESS;
}
