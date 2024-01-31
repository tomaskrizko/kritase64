#include "kritase64.hpp"

#include <unordered_map>

std::unordered_map<int, char> VAL_TO_ALPHABET;
std::unordered_map<char, int> ALPHABET_TO_VAL;

const std::string BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char PAD = '=';

bool initialized = false;
void kritase64::initialize()
{
	if (!initialized)
	{
		for (int index = 0; index < BASE64_ALPHABET.length(); ++index)
		{
			VAL_TO_ALPHABET[index] = BASE64_ALPHABET[index];
			ALPHABET_TO_VAL[BASE64_ALPHABET[index]] = index;
		}
		initialized = true;
	}
}

bool kritase64::check(std::string string)
{
	initialize();
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
				if (ALPHABET_TO_VAL.count(c) <= 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}