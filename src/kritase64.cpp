#include "kritase64.hpp"

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

std::string kritase64::encode(std::vector<uint8_t> bytes)
{
	//initialize();
	std::string result = "";

	int triplets = bytes.size() / 3;
	for (int index = 0; index < triplets; ++index)
	{
		int startIndex = index * 3;

		int charValue = 0; // hextet 1
		charValue = (bytes[startIndex] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 2
		charValue = ((bytes[startIndex] & (2 | 1)) << 4) | (bytes[startIndex + 1] & ((128 | 64 | 32 | 16) >> 4));
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 3
		charValue = ((bytes[startIndex + 1] & (8 | 4 | 2 | 1)) << 2) | ((bytes[startIndex + 2] & (128 | 64)) >> 6);
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);

		charValue = 0; // hextet 4
		charValue = ((bytes[startIndex + 2] & (32 | 16 | 8 | 4 | 2 | 1)));
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);

		std::cout << std::endl;

		result += " ";
	}

	int remainder = bytes.size() % 3;
	if (remainder > 0)
	{
		bytes.push_back(0);

		int startIndex = bytes.size() - remainder - 1;

		int charValue = 0; // hextet 1
		charValue = (bytes[startIndex] & (128 | 64 | 32 | 16 | 8 | 4)) >> 2;
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);

		if (remainder >= 1)
		{
			charValue = 0; // hextet 2
			charValue = ((bytes[startIndex] & (2 | 1)) << 4) | (bytes[startIndex + 1] & ((128 | 64 | 32 | 16) >> 4));
			std::cout << charValue <<  " " << std::flush;
			result += alphabetConverter.valueToAlphabet(charValue);
		}

		if (remainder >= 2)
		{
			charValue = 0; // hextet 3
			charValue = ((bytes[startIndex + 1] & (8 | 4 | 2 | 1)) << 2) | ((bytes[startIndex + 2] & (128 | 64)) >> 6);
			std::cout << charValue <<  " " << std::flush;
			result += alphabetConverter.valueToAlphabet(charValue);
		}

		/*charValue = 0; // hextet 4
		charValue = ((bytes[startIndex + 2] & (32 | 16 | 8 | 4 | 2 | 1)));
		std::cout << charValue <<  " " << std::flush;
		result += alphabetConverter.valueToAlphabet(charValue);*/

		std::cout <<remainder << std::endl;
		for (int p = 0; p < (3 - remainder); ++p)
		{
			result += PAD;
		}
	}

	return result;
}
std::vector<uint8_t> kritase64::decode(std::string string)
{
	if (!check(string))
	{
		throw Base64Exception(ERROR_INVALID_BASE64_STRING, "Decoding invalid base64 string");
	}
}