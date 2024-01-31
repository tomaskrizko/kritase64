/*
 *	This project implements the base64 encoding (RFC 4648).
 *	The base64 alphabet is as follows:
 *	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
 *	Pad: =
 */

#ifndef KRITASE64_HPP
#define KRITASE64_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace kritase64
{
	/**
	 * Initializes lookup tables.
	 */
	void initialize();

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
	std::string encode(std::vector<uint8_t> bytes);

	/**
	 * Decodes a base64 string to bytes.
	 *
	 * @param string The base64 string to decode.
	 * @return The resulting bytes.
	 */
	std::vector<uint8_t> decode(std::string string);
};

#endif