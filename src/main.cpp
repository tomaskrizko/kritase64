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
	std::cout << "kritase64 encode -i[INPUT FILE] -o[OUTPUT FILE] [-a]\n";
	std::cout << "kritase64 decode -i[INPUT FILE] -o[OUTPUT FILE] [-g]\n";
	std::cout << "Input is taken from INPUT FILE, or from standard input, if no -i is provided.\n";
	std::cout << "Output is to OUTPUT FILE, or to standard output, if no -o is provided.\n";
	std::cout << "When encoding, if -a is specified, the input will be encoded to the alternative (\"URL and Filename safe\") base64 alphabet.\n";
	std::cout << "When decoding, if -g is specified, the decoder will ignore all illegal characters (instead of just select few whitespaces)." << std::endl;
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

class CLIException : public std::runtime_error
{
public:
	CLIException(const std::string& message) : std::runtime_error(message)
	{
	}
};

struct CodingInfo
{
	bool inputSpecifided = false;
	std::string input;

	bool outputSpecified = false;
	std::string output;

	kritase64::Base64Mode mode = 0;
};

CodingInfo parseArgs(int argc, char** argv)
{
	CodingInfo res;

	for (int index = 2; index < argc; ++index)
	{
		std::string current = argv[index];
		std::string name;
		switch (current[0])
		{
			case '-':
				if (current.size() < 2) throw CLIException("Not enough arguments");
				switch (current[1])
				{
					case 'i':
						if (res.inputSpecifided) throw CLIException("Can't specify multiple inputs");
						name = current.substr(2);
						if (name.size() <= 0) throw CLIException("Input cannot be empty");
						res.inputSpecifided = true;
						res.input = name;
						break;
					case 'o':
						if (res.outputSpecified) throw CLIException("Can't specify multiple outputs");
						name = current.substr(2);
						if (name.size() <= 0) throw CLIException("Output cannot be empty");
						res.outputSpecified = true;
						res.output = name;
						break;
					case 'a':
						res.mode |= kritase64::MODE_USEALTERNATIVE;
						break;
					case 'g':
						res.mode |= kritase64::MODE_IGNOREALL;
						break;
					default:
						throw CLIException((std::string)"Unknown argument '" + current[1] + "'");
				}
				break;
			default:
				throw CLIException((std::string)"Unknown token '" + current[0] + "'");
		}
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
			kritase64::Buffer data;
			CodingInfo info = parseArgs(argc, argv);
			kritase64::Stream stream("", std::ios::out | std::ios::binary, info.mode);

			if (info.inputSpecifided)
			{
				std::ifstream file(info.input, std::ios::in | std::ios::binary);
				if (!file.is_open()) throw CLIException("Couldn't open INPUT FILE");
				data = everythingFromIO<uint8_t>(file);
				file.close();
			}
			else
			{
				data = everythingFromIO<uint8_t>(std::cin);
			}
			stream.write((char*)data.data(), data.size());
			if (info.outputSpecified)
			{
				std::ofstream output(info.output, std::ios::out | std::ios::trunc);
				if (!output.is_open()) throw CLIException("Couldn't open OUTPUT FILE");
				output << stream.base64();
				output.close();
			}
			else
			{
				std::cout << stream.base64();
			}
		}
		else if (argv[1] == (std::string)"decode")
		{
			std::string base64;
			CodingInfo info = parseArgs(argc, argv);

			if (info.inputSpecifided)
			{
				std::ifstream file(info.input, std::ios::in);
				if (!file.is_open()) throw CLIException("Couldn't open INPUT FILE");
				base64 = everythingFromIO<char>(file);
				file.close();
			}
			else
			{
				base64 = everythingFromIO<char>(std::cin);
			}
			kritase64::Buffer data = kritase64::decode(base64, info.mode);
			if (info.outputSpecified)
			{
				std::ofstream output(info.output, std::ios::out | std::ios::binary | std::ios::trunc);
				if (!output.is_open()) throw CLIException("Couldn't open OUTPUT FILE");
				output.write((char*)data.data(), data.size());
				output.close();
			}
			else
			{
				std::cout.write((char*)data.data(), data.size());
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
		std::cout << "An internal error occured: " << error.what() << std::endl;
		return error.type;
	}
	catch (const CLIException& error)
	{
		std::cout << "An error occured: " << error.what() << std::endl;
		usage();
		return kritase64::ERROR_UNKNOWN;
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
