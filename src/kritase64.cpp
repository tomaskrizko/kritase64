/*
 *	MIT License
 *
 *	Copyright (c) 2025 kritomas
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
 *	Source code: https://gitlab.com/kritomas/kritase64
 */

#include "kritase64.hpp"
#include <cstring>
#include <unordered_map>
#include <unordered_set>

namespace kritase64
{
	constexpr const char BASE64_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	constexpr const char BASE64_ALTERNATIVE_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	constexpr const char BASE64_IGNORED[] = " \t\n\r\0"; // These characters will be ignored when decoding.

	const char PAD = '=';

	class Alphabet_Converter
	{
	private:
		std::unordered_map<int, char> VALUE_TO_ALPHABET, ALTERNATIVE_VALUE_TO_APLHABET;
		std::unordered_map<char, int> ALPHABET_TO_VALUE;
		std::unordered_set<char> IGNORED;

	public:
		Alphabet_Converter()
		{
			for (int index = 0; index < sizeof(BASE64_ALPHABET) - 1; ++index)
			{
				VALUE_TO_ALPHABET[index] = BASE64_ALPHABET[index];
				ALTERNATIVE_VALUE_TO_APLHABET[index] = BASE64_ALTERNATIVE_ALPHABET[index];
				ALPHABET_TO_VALUE[BASE64_ALPHABET[index]] = index;
				ALPHABET_TO_VALUE[BASE64_ALTERNATIVE_ALPHABET[index]] = index;
			}

			for (char c : BASE64_IGNORED)
			{
				IGNORED.insert(c);
			}
		}

		bool is_in_alphabet(char character) const
		{
			return (ALPHABET_TO_VALUE.count(character) > 0);
		}
		bool is_in_range(int value, bool alternative = false) const
		{
			if (alternative)
			{
				return (ALTERNATIVE_VALUE_TO_APLHABET.count(value) > 0);
			}
			return (VALUE_TO_ALPHABET.count(value) > 0);
		}
		bool is_ignored(char c, bool ignore_all = false) const
		{
			if (ignore_all)
			{
				if (c == PAD)
				{
					return false;
				}
				return !is_in_alphabet(c);
			}
			else
			{
				return (IGNORED.count(c) > 0);
			}
		}

		int alphabet_to_value(char character) const
		{
			if (!is_in_alphabet(character))
			{
				throw kritase64::Base64_Exception(kritase64::ERROR_INVALID_BASE64_CHARACTER, "Invalid base64 character");
			}
			return ALPHABET_TO_VALUE.at(character);
		}
		char value_to_alphabet(int value, bool alternative = false) const
		{
			if (!is_in_range(value, alternative))
			{
				throw kritase64::Base64_Exception(kritase64::ERROR_VALUE_OUT_OF_RANGE, "Value out of range for base64");
			}
			if (alternative)
			{
				return ALTERNATIVE_VALUE_TO_APLHABET.at(value);
			}
			return VALUE_TO_ALPHABET.at(value);
		}
		std::string strip_ignored(std::string base64, bool ignore_illegal = false) const
		{
			for (auto it = base64.begin(); it != base64.end();)
			{
				if (is_ignored(*it, ignore_illegal))
				{
					it = base64.erase(it);
					continue;
				}
				++it;
			}
			return base64;
		}
	};
	Alphabet_Converter& alphabet_converter()
	{
		static Alphabet_Converter instance;
		return instance;
	}
}

kritase64::Base64_Exception::Base64_Exception(kritase64::Base64_Error_Types type, std::string message) : std::runtime_error(message)
{
	this->type = type;
}

bool kritase64::check(const std::string& string)
{
	bool padding_reached = false;
	for (int index = 0; index < string.length(); ++index)
	{
		char c = string[index];
		if (padding_reached)
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
				padding_reached = true;
			}
			else
			{
				if (!alphabet_converter().is_in_alphabet(c))
				{
					return false;
				}
			}
		}
	}
	return true;
}

std::string kritase64::encode(const uint8_t* buffer, size_t size, bool use_alternative)
{
	std::string result = "";

	int triplets = size / 3;
	for (int index = 0; index < triplets; ++index)
	{
		int start_index = index * 3;

		int char_value = 0; // hextet 1
		char_value = (buffer[start_index] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		result += alphabet_converter().value_to_alphabet(char_value, use_alternative);

		char_value = 0; // hextet 2
		char_value = ((buffer[start_index] & (2 | 1)) << 4) | ((buffer[start_index + 1] & (128 | 64 | 32 | 16)) >> 4);
		result += alphabet_converter().value_to_alphabet(char_value, use_alternative);

		char_value = 0; // hextet 3
		char_value = ((buffer[start_index + 1] & (8 | 4 | 2 | 1)) << 2) | ((buffer[start_index + 2] & (128 | 64)) >> 6);
		result += alphabet_converter().value_to_alphabet(char_value, use_alternative);

		char_value = 0; // hextet 4
		char_value = ((buffer[start_index + 2] & (32 | 16 | 8 | 4 | 2 | 1)));
		result += alphabet_converter().value_to_alphabet(char_value, use_alternative);
	}

	int remainder = size % 3;
	if (remainder > 0)
	{
		int start_index = size - remainder;

		int char_value = 0; // hextet 1
		char_value = (buffer[start_index] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		result += alphabet_converter().value_to_alphabet(char_value, use_alternative);

		if (remainder >= 1)
		{
			char_value = 0; // hextet 2
			char_value = (buffer[start_index] & (2 | 1)) << 4;
			if (remainder >= 2) char_value |= ((buffer[start_index + 1] & (128 | 64 | 32 | 16)) >> 4);
			result += alphabet_converter().value_to_alphabet(char_value, use_alternative);
		}

		if (remainder >= 2)
		{
			/*char_value = 0; // hextet 2
			char_value = ((buffer[start_index] & (2 | 1)) << 4) | ((bytes[start_index + 1] & (128 | 64 | 32 | 16)) >> 4);
			result += alphabet_converter().value_to_alphabet(char_value);*/

			char_value = 0; // hextet 3
			char_value = (buffer[start_index + 1] & (8 | 4 | 2 | 1)) << 2;
			result += alphabet_converter().value_to_alphabet(char_value, use_alternative);
		}

		for (int p = 0; p <(3 - remainder); ++p)
		{
			result += PAD;
		}
	}

	return result;
}
std::string kritase64::encode(const Buffer& buffer, bool use_alternative)
{
	return encode(buffer.data(), buffer.size(), use_alternative);
}
std::string kritase64::encode(const std::string& string, bool use_alternative)
{
	return encode((uint8_t*)string.data(), string.size(), use_alternative);
}

kritase64::Buffer kritase64::decode(std::string string, bool is_ignored)
{
	string = alphabet_converter().strip_ignored(string, is_ignored);
	if (!check(string))
	{
		throw Base64_Exception(ERROR_INVALID_BASE64_STRING, "Decoding invalid base64 string");
	}

	Buffer result;

	int padding = 0;
	while (string[string.size() - 1 - padding] == PAD)
	{
		++padding;
	}
	int string_size = string.size() - padding;
	std::string data = string.substr(0, string_size);

	int quartets = string_size / 4;
	for (int index = 0; index < quartets; ++index)
	{
		int start_index = index * 4;

		uint8_t octet_value = 0; // octet 1
		octet_value = (alphabet_converter().alphabet_to_value(data[start_index]) << 2) | ((alphabet_converter().alphabet_to_value(data[start_index + 1]) & (32 | 16)) >> 4);
		result.push_back(octet_value);

		octet_value = 0; // octet 2
		octet_value = ((alphabet_converter().alphabet_to_value(data[start_index + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabet_converter().alphabet_to_value(data[start_index + 2]) & (32 | 16 | 8 | 4 )) >> 2);
		result.push_back(octet_value);

		octet_value = 0; // octet 3
		octet_value = ((alphabet_converter().alphabet_to_value(data[start_index + 2]) & (2 | 1)) << 6) | alphabet_converter().alphabet_to_value(data[start_index + 3]);
		result.push_back(octet_value);
	}

	if (padding > 0)
	{
		int start_index = quartets * 4;

		uint8_t octet_value = 0;
		if (padding == 2)
		{
			octet_value = 0; // octet 1
			octet_value = (alphabet_converter().alphabet_to_value(data[start_index]) << 2) |((alphabet_converter().alphabet_to_value(data[start_index + 1]) & (32 | 16)) >> 4);
			result.push_back(octet_value);
		}

		if (padding == 1)
		{
			octet_value = 0; // octet 1
			octet_value = (alphabet_converter().alphabet_to_value(data[start_index]) << 2) |((alphabet_converter().alphabet_to_value(data[start_index + 1]) & (32 | 16)) >> 4);
			result.push_back(octet_value);

			octet_value = 0; // octet 2
			octet_value = ((alphabet_converter().alphabet_to_value(data[start_index + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabet_converter().alphabet_to_value(data[start_index + 2]) & (32 | 16 | 8 | 4 )) >> 2);
			result.push_back(octet_value);
		}
	}

	return result;
}
std::string kritase64::decode_to_string(const std::string& string, bool is_ignored)
{
	Buffer buffer = decode(string, is_ignored);
	std::string result;
	result.resize(buffer.size());
	memcpy(result.data(), buffer.data(), buffer.size());
	return result;
}

std::string kritase64::Stream::str() const
{
	return std::stringstream::str();
}

kritase64::Stream::Stream(const std::string& base64, std::ios_base::openmode openmode, Base64_Mode mode) : std::stringstream(openmode)
{
	this->mode = mode;
	this->base64(base64);
}

kritase64::Buffer kritase64::Stream::buffer() const
{
	kritase64::Buffer result;
	result.resize(str().size());
	memcpy(result.data(), str().data(), str().size());
	return result;
}
void kritase64::Stream::buffer(const Buffer& buffer)
{
	std::stringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Stream::base64() const
{
	return encode(str(), mode & MODE_USEALTERNATIVE);
}
void kritase64::Stream::base64(const std::string& base64)
{
	buffer(decode(base64, mode & MODE_IGNOREALL));
}

std::string kritase64::Istream::str() const
{
	return std::istringstream::str();
}

kritase64::Istream::Istream(const std::string& base64, std::ios_base::openmode openmode, Base64_Mode mode) : std::istringstream(openmode)
{
	this->mode = mode;
	this->base64(base64);
}

kritase64::Buffer kritase64::Istream::buffer() const
{
	kritase64::Buffer result;
	result.resize(str().size());
	memcpy(result.data(), str().data(), str().size());
	return result;
}
void kritase64::Istream::buffer(const Buffer& buffer)
{
	std::istringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Istream::base64() const
{
	return encode(str(), mode & MODE_USEALTERNATIVE);
}
void kritase64::Istream::base64(const std::string& base64)
{
	buffer(decode(base64, mode & MODE_IGNOREALL));
}

std::string kritase64::Ostream::str() const
{
	return std::ostringstream::str();
}

kritase64::Ostream::Ostream(const std::string& base64, std::ios_base::openmode openmode, Base64_Mode mode) : std::ostringstream(openmode)
{
	this->mode = mode;
	this->base64(base64);
}

kritase64::Buffer kritase64::Ostream::buffer() const
{
	kritase64::Buffer result;
	result.resize(str().size());
	memcpy(result.data(), str().data(), str().size());
	return result;
}
void kritase64::Ostream::buffer(const Buffer& buffer)
{
	std::ostringstream::str(std::string((char*)buffer.data(), buffer.size()));
}

std::string kritase64::Ostream::base64() const
{
	return encode(str(), mode & MODE_USEALTERNATIVE);
}
void kritase64::Ostream::base64(const std::string& base64)
{
	buffer(decode(base64, mode & MODE_IGNOREALL));
}
