
#include "kritase64.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

const std::string BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char PAD = '=';

class AlphabetConverter
{
private:
	std::unordered_map<int, char> VALUE_TO_ALPHABET;
	std::unordered_map<char, int> ALPHABET_TO_VALUE;

public:
	AlphabetConverter()
	{
		for (int index = 0; index < BASE64_ALPHABET.length(); ++index)
		{
			VALUE_TO_ALPHABET[index] = BASE64_ALPHABET[index];
			ALPHABET_TO_VALUE[BASE64_ALPHABET[index]] = index;
		}
		//initialized = true;
	}

	bool isInAlphabet(char character) const
	{
		return (ALPHABET_TO_VALUE.count(character) > 0);
	}
	bool isInRange(int value) const
	{
		return (VALUE_TO_ALPHABET.count(value) > 0);
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
};
AlphabetConverter alphabetConverter;

/*bool initialized = false;
void kritase64::initialize()
{
	if (!initialized)
	{

}*/

kritase64::Base64Exception::Base64Exception(kritase64::Base64ErrorTypes type, std::string message) : std::runtime_error(message)
{
	this->type = type;
}

bool kritase64::check(std::string string)
{
	//initialize();
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
#include <iostream> // TODO

std::string kritase64::encode(const uint8_t* buffer, size_t size)
{
	//initialize();
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
std::string kritase64::encode(Buffer buffer)
{
	return encode(buffer.data(), buffer.size());
}
std::string kritase64::encode(std::string string)
{
	return encode((uint8_t*)string.data(), string.size());
}

kritase64::Buffer kritase64::decode(std::string string)
{
	if (!check(string))
	{
		throw Base64Exception(ERROR_INVALID_BASE64_STRING, "Decoding invalid base64 string");
	}

	Buffer result;

	int padding = 0;
	while (string[string.size() - 1 - padding] == '=')
	{
		++padding;
	}
	int stringSize = string.size() - padding;
	string = string.substr(0, stringSize);

	int quartets = stringSize / 4;
	for (int index = 0; index < quartets; ++index)
	{
		int startIndex = index * 4;

		uint8_t octetValue = 0; // octet 1
		octetValue = (alphabetConverter.alphabetToValue(string[startIndex]) << 2) | ((alphabetConverter.alphabetToValue(string[startIndex + 1]) & (32 | 16)) >> 4);
		result.push_back(octetValue);

		octetValue = 0; // octet 2
		octetValue = ((alphabetConverter.alphabetToValue(string[startIndex + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabetConverter.alphabetToValue(string[startIndex + 2]) & (32 | 16 | 8 | 4 )) >> 2);
		result.push_back(octetValue);

		octetValue = 0; // octet 3
		octetValue = ((alphabetConverter.alphabetToValue(string[startIndex + 2]) & (2 | 1)) << 6) | alphabetConverter.alphabetToValue(string[startIndex + 3]);
		result.push_back(octetValue);
	}

	if (padding > 0)
	{
		int startIndex = quartets * 4;

		uint8_t octetValue = 0;
		if (padding == 2)
		{
			octetValue = 0; // octet 1
			octetValue = (alphabetConverter.alphabetToValue(string[startIndex]) << 2) |((alphabetConverter.alphabetToValue(string[startIndex + 1]) & (32 | 16)) >> 4);
			result.push_back(octetValue);
		}

		if (padding == 1)
		{
			octetValue = 0; // octet 1
			octetValue = (alphabetConverter.alphabetToValue(string[startIndex]) << 2) |((alphabetConverter.alphabetToValue(string[startIndex + 1]) & (32 | 16)) >> 4);
			result.push_back(octetValue);

			octetValue = 0; // octet 2
			octetValue = ((alphabetConverter.alphabetToValue(string[startIndex + 1]) & (8 | 4 | 2 | 1)) << 4) | ((alphabetConverter.alphabetToValue(string[startIndex + 2]) & (32 | 16 | 8 | 4 )) >> 2);
			result.push_back(octetValue);
		}
	}

	return result;
}
