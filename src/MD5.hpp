/*
 * MD5.hpp
 *
 *  Created on: 06.09.2012
 *      Author: Dmitriy Khaustov aka Dиmon (khaustov.dm@gmail.com)
 */

#ifndef MD5_HPP_
#define MD5_HPP_

#include "stddef.h"
#include "stdint.h"

typedef uint8_t md5_byte_t; /* 8-bit byte */
typedef uint32_t md5_word_t; /* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s {
    md5_word_t count[2];	/* message length in bits, lsw first */
    md5_word_t abcd[4];		/* digest buffer */
    md5_byte_t buf[64];		/* accumulate block */
} md5_state_t;

/*
 *
 */
class MD5
{
public:
	static const size_t BytesHashLength = 16;
	static const size_t StringHashLength = BytesHashLength<<1;

private:
	md5_state_t _state;
	md5_byte_t _digest[BytesHashLength];
	char _str_digest[StringHashLength+1];
	bool _finalized;

public:
	MD5();
	MD5(const void *data, size_t len);
	virtual ~MD5();

	bool append(const void *data, size_t len);
	const char * getBytesHash();
	const char * getStringHash();

};

#endif /* MD5_HPP_ */
