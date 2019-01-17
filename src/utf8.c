#include "utf8.h"

static int rune_is_valid(rune_t rune)
{
	return rune < 0xD800 || rune > 0xDFFF;
}

static int secondary_byte(unsigned char b)
{
	return b >= 0x80 && b <= 0xBF;
}

size_t utf8_decode(const char* data, size_t size, rune_t* rune)
{
	if (size == 0)
		return 0;

	size_t rd = 0;
	unsigned char c = (unsigned char)data[rd++];

	// 1 byte ASCII.
	if (c <= 0x7F)
	{
		if (rune != NULL)
			*rune = (rune_t)c;

		return 1;
	}

	// 2 byte.
	if (c >= 0xC0 && c <= 0xDF)
	{
		if (rd == size)
			return 0;

		unsigned char more = (unsigned char)data[rd];
		if (!secondary_byte(more))
			return 0;

		rune_t rn = (((rune_t)c & 0x1F) << 6) | ((rune_t)more & 0x3F);
		if (!rune_is_valid(rn))
			return 0;

		if (rune != NULL)
			*rune = rn;

		return 2;
	}

	// 3 byte.
	if (c >= 0xE0 && c <= 0xEF)
	{
		if (rd + 2 > size)
			return 0;

		unsigned char more[2];
		for (size_t i = 0; i < sizeof(more); i++)
		{
			if (!secondary_byte(more[i] = data[rd++]))
				return 0;
		}

		rune_t rn = (((rune_t)c & 0x0F) << 12) | (((rune_t)more[0] & 0x3F) << 6) | ((rune_t)more[1] & 0x3F);
		if (!rune_is_valid(rn))
			return 0;

		if (rune != NULL)
			*rune = rn;

		return 3;
	}

	// 4 byte.
	if (c >= 0xF0 && c <= 0xF7)
	{
		if (rd + 3 > size)
			return 0;

		unsigned char more[3];
		for (size_t i = 0; i < sizeof(more); i++)
		{
			if (!secondary_byte(more[i] = data[rd++]))
				return 0;
		}

		rune_t rn = (((rune_t)c & 0x0F) << 18) | (((rune_t)more[0] & 0x3F) << 12) | (((rune_t)more[1] & 0x3F) << 6) | ((rune_t)more[2] & 0x3F);
		if (!rune_is_valid(rn))
			return 0;

		if (rune != NULL)
			*rune = rn;

		return 4;
	}

	return 0;
}

size_t utf8_encode(char* buffer, rune_t rune)
{
	// 1 byte ASCII.
	if (rune <= 0x7F)
	{	
		if (buffer != NULL)
			*buffer = (char)rune;

		return 1;
	}

	// 2 byte.
	if (rune >= 0x80 && rune <= 0x7FF)
	{
		if (buffer != NULL)
		{
			buffer[0] = (char)(0xC0 | ((rune >> 6) & 0x1F));
			buffer[1] = (char)(0x80 | (rune & 0x3F));
		}

		return 2;
	}

	// 3 byte.
	if (rune >= 0x800 && rune <= 0xFFFF)
	{
		if (buffer != NULL)
		{
			buffer[0] = (char)(0xE0 | ((rune >> 12) & 0x0F));
			buffer[1] = (char)(0x80 | ((rune >> 6) & 0x3F));
			buffer[2] = (char)(0x80 | (rune & 0x3F));
		}

		return 3;
	}

	// 4 byte.
	if (rune >= 0x10000 && rune <= 0x10FFFF)
	{
		if (buffer != NULL)
		{
			buffer[0] = (char)(0xF0 | ((rune >> 18) & 0x07));
			buffer[1] = (char)(0x80 | ((rune >> 12) & 0x3F));
			buffer[2] = (char)(0x80 | ((rune >> 6) & 0x3F));
			buffer[3] = (char)(0x80 | (rune & 0x3F));
		}

		return 4;
	}

	return 0;
}

int utf8_is_valid(const char* data, size_t size)
{
	for (size_t i = 0; i < size;) 
	{
		size_t s = utf8_decode(&data[i], size - i, NULL);
		if (s == 0)
			return 0;

		i += s;
	}

	return 1;
}