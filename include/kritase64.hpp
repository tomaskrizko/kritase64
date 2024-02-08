/*
 *	This project implements the base64 encoding (RFC 4648).
 *	The base64 alphabet is as follows:
 *	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
 *	Pad: =
 */

#ifndef KRITASE64_HPP
#define KRITASE64_HPP

#include <cstdint>
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
	bool check(const std::string& string);

	/**
	 * Encodes a buffer as a base64 string.
	 *
	 * @param buffer Pointer to the beginning of the buffer.
	 * @param size Size of the buffer.
	 * @return The resulting base64 string.
	 */
	std::string encode(const uint8_t* buffer, size_t size);
	/**
	 * Encodes a buffer as a base64 string.
	 *
	 * @param buffsr The buffer to encode.
	 * @return The resulting base64 string.
	 */
	std::string encode(const Buffer& buffer);
	/**
	 * Encodes a string as a base64 string.
	 *
	 * @param string The string to encode.
	 * @return The resulting base64 string.
	 */
	std::string encode(const std::string& string);

	/**
	 * Decodes a base64 string to a buffer.
	 *
	 * @param string The base64 string to decode.
	 * @return The resulting buffer.
	 */
	Buffer decode(const std::string& string);

	class Stream : public std::stringstream
	{
	protected:
		std::string str() const; // Hiding base method
		
	public:
		Stream(const std::string& base64 = "", std::ios_base::openmode mode = std::ios::in | std::ios::out);
		
		std::string base64() const;
		void base64(const std::string& base64);
	};
	class Istream : public std::istringstream
	{
	public:
		std::string  base64() const;
	};
	class Ostream : public std::ostringstream
	{
	public:
		std::string base64() const;
	};
};

#endif
