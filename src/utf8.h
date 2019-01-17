#ifndef UTF8_H
#define UTF8_H
#include <stdlib.h>
#include <stdint.h>

#define UTF8_MAX_ENCODED_LENGTH	4

typedef uint32_t rune_t;

/*
	Decode an UTF-8 sequence.
	
	Parameters:
		data - pointer to data
		size - size of data
		rune - decoded unicode codepoint, can be NULL

	Return:
		>0 - size of decoded sequence
		0 - error, invalid UTF-8 supplied or data is not long enough
*/
size_t utf8_decode(const char* data, size_t size, rune_t* rune);

/*
	Encode an unicode codepoint to a UTF-8 sequence.
	
	Parameters:
		buffer - result buffer, can be NULL; if not, has to be at least UTF8_MAX_ENCODED_LENGTH of size
		rune - the unicode codepoint to be encoded
 	
 	Return:
		>0 - size of encoded sequence
		0 - error, invalid codepoint
*/
size_t utf8_encode(char* buffer, rune_t rune);

/* 
	Check if data is a valid UTF-8 string.

	Parameters:
		data - pointer to data
		size - size of data

	Return:
		1 - valid
		0 - invalid
*/
int utf8_is_valid(const char* data, size_t size);
#endif