/*
 * EncodingConverter.h
 *
 *  Created on: Apr 4, 2014
 *      Author: haha
 */

#ifndef ENCODINGCONVERTER_H_
#define ENCODINGCONVERTER_H_

#include "iconv.h"
#include <string>
#include <stdexcept>
#include <cstring>
#include <stdlib.h>

#define OUTLEN 255

class EncodingConverter
{
public:
	EncodingConverter();
	virtual ~EncodingConverter();



	std::string gbk_to_utf8(const std::string&);
	std::string utf8_to_gbk(const std::string&);



private:

	int code_convert(const char *from_charset, const char *to_charset, char *inbuf,
				int inlen, char *outbuf, int outlen);
};

#endif /* ENCODINGCONVERTER_H_ */
