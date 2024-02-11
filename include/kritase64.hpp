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
 *
 *	This project implements the base64 encoding (RFC 4648).
 *	The base64 alphabet is as follows:
 *	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
 *	Alternative: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_
 *	Pad: =
 */

#ifndef KRITASE64_HPP
#define KRITASE64_HPP

#include <exception>
#include <sstream>
#include <stdexcept>
#include <stdint.h>

namespace kritase64
{
	enum Base64ErrorTypes { ERROR_SUCCESS = 0, ERROR_UNKNOWN = -1, ERROR_INVALID_BASE64_STRING = 1, ERROR_INVALID_BASE64_CHARACTER, ERROR_VALUE_OUT_OF_RANGE };

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
	Buffer decode(std::string string);

	class Stream : public std::stringstream
	{
	protected:
		std::string str() const; // Hiding base method
		
	public:
		Stream(const std::string& base64 = "", std::ios_base::openmode mode = std::ios::in | std::ios::out);
		
		Buffer buffer() const;
		void buffer(const Buffer& buffer);

		std::string base64() const;
		void base64(const std::string& base64);
	};
	class Istream : public std::istringstream
	{
	protected:
		std::string str() const; // Hiding base method

	public:
		Istream(const std::string& base64 = "", std::ios_base::openmode = std::ios::in);
		
		Buffer buffer() const;
		void buffer(const Buffer& buffer);

		std::string  base64() const;
		void base64(const std::string& base64);
	};
	class Ostream : public std::ostringstream
	{
	protected:
		std::string str() const; // Hiding base method
		
	public:
		Ostream(const std::string& base64 = "", std::ios_base::openmode = std::ios::out);
		
		Buffer buffer() const;
		void buffer(const Buffer& buffer);

		std::string base64() const;
		void base64(const std::string& base64);
	};
};

#endif
