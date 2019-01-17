#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "utf8.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: <data>\n");
		return EXIT_FAILURE;
	}

	const char* data = argv[1];
	size_t size = strlen(data);
	if (!utf8_is_valid(data, size))
	{
		printf("Error: Invalid UTF-8\n");
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < size;) 
	{
		rune_t rune;
		size_t s = utf8_decode(&data[i], size - i, &rune);
		
		assert(s != 0);

		char ubuf[UTF8_MAX_ENCODED_LENGTH + 1] = { 0 };
		memcpy(ubuf, &data[i], s);

		printf("U+%06lX - %s\n", (unsigned long)rune, ubuf);

		i += s;
	}

	return EXIT_SUCCESS;
}