/*
 * EncodingConverter.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: haha
 */

#include "EncodingConverter.h"
#include <iostream>

const int MAX_SIZE = 1000;

using namespace std;

EncodingConverter::EncodingConverter() {

}

EncodingConverter::~EncodingConverter() {

}

std::string EncodingConverter::gbk_to_utf8(const std::string &s) {
	char *from = new char[s.size() + 1];
	std::strcpy(from, s.c_str());
	char *to = new char[MAX_SIZE];
	int from_len = strlen(from);
	int to_len = MAX_SIZE;
	code_convert("gbk", "utf-8", from, from_len, to, to_len);
	string ret(to);
	delete[] from;
	delete[] to;
	return ret;
}

std::string EncodingConverter::utf8_to_gbk(const std::string &s) {
	char *from = new char[s.size() + 1];
	std::strcpy(from, s.c_str());
	char *to = new char[MAX_SIZE];
	int from_len = strlen(from);
	int to_len = MAX_SIZE;
	code_convert("utf-8", "gbk", from, from_len, to, to_len);
	string ret(to);
	delete[] from;
	delete[] to;
	return ret;
}

int EncodingConverter::code_convert(const char *from_charset,
		const char *to_charset, char *inbuf, int inlen, char *outbuf,
		int outlen) {
	iconv_t cd;
	//int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	iconv(cd, pin, (std::size_t*) &inlen, pout, (std::size_t*) &outlen);
	iconv_close(cd);
	return 0;
}

