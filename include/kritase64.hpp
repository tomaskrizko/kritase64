/*
 *	This project implements the base64 encoding (RFC 4648).
 *	The base64 alphabet is as follows:
 *	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
 *	Pad: =
 */

#ifndef KRITASE64_HPP
#define KRITASE64_HPP

#include <sstream>
#include <vector>
#include <string>
#include <stdint.h>
#include <exception>
#include <stdexcept>

namespace kritase64
{
	enum Base64ErrorTypes { ERROR_UNKNOWN, ERROR_INVALID_BASE64_STRING, ERROR_INVALID_BASE64_CHARACTER, ERROR_VALUE_OUT_OF_RANGE };

	class Base64Exception : public std::runtime_error
	{
	public:
		Base64ErrorTypes type;

		Base64Exception(Base64ErrorTypes type, std::string message);
	};

	typedef std::basic_string<uint8_t> Buffer;

	/**
	 * Checks, whether a given string is a valid base64 string.
	 *
	 * @param string The string to check.
	 * @return true, if the given string is a valid base64 string, false otherwise.
	 */
	bool check(std::string string);

	/**
	 * Encodes bytes as a base64 string.
	 *
	 * @param bytes The bytes to encode.
	 * @return The resulting base64 string.
	 */
	std::string encode(Buffer bytes);

	/**
	 * Decodes a base64 string to bytes.
	 *
	 * @param string The base64 string to decode.
	 * @return The resulting bytes.
	 */
	Buffer decode(std::string string);
};

#endif