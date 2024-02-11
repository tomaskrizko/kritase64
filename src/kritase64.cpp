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

#include "kritase64.hpp"
#include <cstring>
#include <unordered_map>
#include <unordered_set>

namespace kritase64
{
	const std::string BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const std::string BASE64_ALTERNATIVE_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	const std::string BASE64_IGNORED = " \t\n\r\0"; // These characters will be ignored when decoding.

	const char PAD = '=';

	class AlphabetConverter
	{
	private:
		std::unordered_map<int, char> VALUE_TO_ALPHABET;
		std::unordered_map<char, int> ALPHABET_TO_VALUE;
		std::unordered_set<char> IGNORED;

	public:
		AlphabetConverter()
		{
			for (int index = 0; index < BASE64_ALPHABET.length(); ++index)
			{
				VALUE_TO_ALPHABET[index] = BASE64_ALPHABET[index];
				ALPHABET_TO_VALUE[BASE64_ALPHABET[index]] = index;
				ALPHABET_TO_VALUE[BASE64_ALTERNATIVE_ALPHABET[index]] = index;
			}

			for (char c : BASE64_IGNORED)
			{
				IGNORED.insert(c);
			}
		}

		bool isInAlphabet(char character) const
		{
			return (ALPHABET_TO_VALUE.count(character) > 0);
		}
		bool isInRange(int value) const
		{
			return (VALUE_TO_ALPHABET.count(value) > 0);
		}
		bool isIgnored(char c) const
		{
			return (IGNORED.count(c) > 0);
		}

		int alphabetToValue(char character) const
		{
			if (!isInAlphabet(character))
			{
				throw kritase64::Base64Exception(kritase64::ERROR_INVALID_BASE64_CHARACTER, "Invalid base64 character");
			}
			return ALPHABET_TO_VALUE.at(character);
		}
		char valueToAlphabet(int value) const
		{
			if (!isInRange(value))
			{
				throw kritase64::Base64Exception(kritase64::ERROR_VALUE_OUT_OF_RANGE, "Value out of range for base64");
			}
			return VALUE_TO_ALPHABET.at(value);
		}
		std::string stripIgnored(std::string base64) const
		{
			for (auto it = base64.begin(); it != base64.end();)
			{
				if (isIgnored(*it))
				{
					it = base64.erase(it);
					continue;
				}
				++it;
			}
			return base64;
		}
	};
	AlphabetConverter alphabetConverter;
}

kritase64::Base64Exception::Base64Exception(kritase64::Base64ErrorTypes type, std::string message) : std::runtime_error(message)
{
	this->type = type;
}

bool kritase64::check(const std::string& string)
{
	for (int index = 0; index < string.length(); ++index)
	{
		char c = string[index];
		bool paddingReached = false;
		if (paddingReached)
		{
			if (c != PAD)
			{
				return false;
			}
		}
		else
		{
			if (c == PAD)
			{
				paddingReached = true;
			}
			else
			{
				if (!alphabetConverter.isInAlphabet(c))
				{
					return false;
				}
			}
		}
	}
	return true;
}

std::string kritase64::encode(const uint8_t* buffer, size_t size)
{
	std::string result = "";

	int triplets = size / 3;
	for (int index = 0; index < triplets; ++index)
	{
		int startIndex = index * 3;

		int charValue = 0; // hextet 1
		charValue = (buffer[startIndex] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 2
		charValue = ((buffer[startIndex] & (2 | 1)) << 4) | ((buffer[startIndex + 1] & (128 | 64 | 32 | 16)) >> 4);
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 3
		charValue = ((buffer[startIndex + 1] & (8 | 4 | 2 | 1)) << 2) | ((buffer[startIndex + 2] & (128 | 64)) >> 6);
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 4
		charValue = ((buffer[startIndex + 2] & (32 | 16 | 8 | 4 | 2 | 1)));
		result += alphabetConverter.valueToAlphabet(charValue);
	}

	int remainder = size % 3;
	if (remainder > 0)
	{
		int startIndex = size - remainder;

		int charValue = 0; // hextet 1
		charValue = (buffer[startIndex] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		result += alphabetConverter.valueToAlphabet(charValue);

		if (remainder >= 1)
		{
			charValue = 0; // hextet 2
			charValue = (buffer[startIndex] & (2 | 1)) << 4;
			if (remainder >= 2) charValue |= ((buffer[startIndex + 1] & (128 | 64 | 32 | 16)) >> 4);
			result += alphabetConverter.valueToAlphabet(charValue);
		}

		if (remainder >= 2)
		{
			/*charValue = 0; // hextet 2
			charValue = ((buffer[startIndex] & (2 | 1)) << 4) | ((bytes[startIndex + 1] & (128 | 64 | 32 | 16)) >> 4);
			result += alphabetConverter.valueToAlphabet(charValue);*/

			charValue = 0; // hextet 3
			charValue = (buffer[startIndex + 1] & (8 | 4 | 2 | 1)) << 2;
			result += alphabetConverter.valueToAlphabet(charValue);
		}

		for (int p = 0; p <(3 - remainder); ++p)
		{
			result += PAD;
		}
	}

	return result;
}
std::string kritase64::encode(const Buffer& buffer)
{
	return encode(buffer.data(), buffer.size());
}
std::string kritase64::encode(const std::string& string)
{
	return encode((uint8_t*)string.data(), string.size());
}

kritase64::Buffer kritase64::decode(std::string string)
{
	string = alphabetConverter.stripIgnored(string);
	if (!check(string))
	{
		throw Base64Exception(ERROR_INVALID_BASE64_STRING, "Decoding invalid base64 string");
	}

	Buffer result;

	int padding = 0;
	while (string[string.size() - 1 - padding] == PAD)
	{
		++padding;
	}
	int stringSize = string.size() - padding;
	std::string data = string.substr(0, stringSize);

	int quartets = stringSize / 4;
	for (int index = 0; index < quartets; ++index)
	{
		int startIndex = index * 4;

		uint8_t octetValue = 0; // octet 1
		octetValue = (alphabetConverter.alphabetToValue(data[startIndex]) << 2) | ((alphabetConverter.alphabetToValue(data[startIndex + 1]) & (32 | 16)) >> 4);
		result.push_back(octetValue);

		octetValue = 0; // octet 2
		octetValue = ((alphabetConverter.alphabetToValue(data[startIndex + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabetConverter.alphabetToValue(data[startIndex + 2]) & (32 | 16 | 8 | 4 )) >> 2);
		result.push_back(octetValue);

		octetValue = 0; // octet 3
		octetValue = ((alphabetConverter.alphabetToValue(data[startIndex + 2]) & (2 | 1)) << 6) | alphabetConverter.alphabetToValue(data[startIndex + 3]);
		result.push_back(octetValue);
	}

	if (padding > 0)
	{
		int startIndex = quartets * 4;

		uint8_t octetValue = 0;
		if (padding == 2)
		{
			octetValue = 0; // octet 1
			octetValue = (alphabetConverter.alphabetToValue(data[startIndex]) << 2) |((alphabetConverter.alphabetToValue(data[startIndex + 1]) & (32 | 16)) >> 4);
			result.push_back(octetValue);
		}

		if (padding == 1)
		{
			octetValue = 0; // octet 1
			octetValue = (alphabetConverter.alphabetToValue(data[startIndex]) << 2) |((alphabetConverter.alphabetToValue(data[startIndex + 1]) & (32 | 16)) >> 4);
			result.push_back(octetValue);

			octetValue = 0; // octet 2
			octetValue = ((alphabetConverter.alphabetToValue(data[startIndex + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabetConverter.alphabetToValue(data[startIndex + 2]) & (32 | 16 | 8 | 4 )) >> 2);
			result.push_back(octetValue);
		}
	}

	return result;
}

std::string kritase64::Stream::str() const
{
	return std::stringstream::str();
}

kritase64::Stream::Stream(const std::string& base64, std::ios_base::openmode mode) : std::stringstream(mode)
{
	this->base64(base64);
}

kritase64::Buffer kritase64::Stream::buffer() const
{
	kritase64::Buffer res;
	res.resize(str().size());
	memcpy(res.data(), str().data(), str().size());
	return res;
}
void kritase64::Stream::buffer(const Buffer& buffer)
{
	std::stringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Stream::base64() const
{
	return encode(str());
}
void kritase64::Stream::base64(const std::string& base64)
{
	buffer(decode(base64));
}

std::string kritase64::Istream::str() const
{
	return std::istringstream::str();
}

kritase64::Istream::Istream(const std::string& base64, std::ios_base::openmode mode) : std::istringstream(mode)
{
	this->base64(base64);
}

kritase64::Buffer kritase64::Istream::buffer() const
{
	kritase64::Buffer res;
	res.resize(str().size());
	memcpy(res.data(), str().data(), str().size());
	return res;
}
void kritase64::Istream::buffer(const Buffer& buffer)
{
	std::istringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Istream::base64() const
{
	return encode(str());
}
void kritase64::Istream::base64(const std::string& base64)
{
	buffer(decode(base64));
}

std::string kritase64::Ostream::str() const
{
	return std::ostringstream::str();
}

kritase64::Ostream::Ostream(const std::string& base64, std::ios_base::openmode mode) : std::ostringstream(mode)
{
	this->base64(base64);
}

kritase64::Buffer kritase64::Ostream::buffer() const
{
	kritase64::Buffer res;
	res.resize(str().size());
	memcpy(res.data(), str().data(), str().size());
	return res;
}
void kritase64::Ostream::buffer(const Buffer& buffer)
{
	std::ostringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Ostream::base64() const
{
	return encode(str());
}
void kritase64::Ostream::base64(const std::string& base64)
{
	buffer(decode(base64));
}
