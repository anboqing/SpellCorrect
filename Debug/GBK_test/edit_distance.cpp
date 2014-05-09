#include <stdint.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "EncodingConverter.h"

using namespace std;

bool is_gbk_first_alpha(unsigned char c) {
	return c >= 0x81 && c <= 0xFE;  //首字节的范围
}

// string -> uint16_t array
void parse_gbk_string(const string &s, vector<uint16_t> &vec) {
	vec.clear();
	for (string::size_type ix = 0; ix != s.size(); ++ix) {
		if (is_gbk_first_alpha(s[ix])) {
			//拼接
			//检查越界的情况
			if (ix + 1 == s.size()) {
				throw runtime_error("invalid GBK string");
			}

			//进行一次拼接
			unsigned char c1 = s[ix];
			unsigned char c2 = s[ix + 1];
			uint16_t t = (c1 << 8) + c2;
			vec.push_back(t);
			++ix;
		} else {
			vec.push_back((uint16_t) (unsigned char) s[ix]);
		}
	}
}

inline int MIN(int a, int b, int c) {
	int ret = (a < b) ? a : b;
	ret = (ret < c) ? ret : c;
	return ret;
}

int edit_distance_uint_16(const vector<uint16_t> &w1,
		const vector<uint16_t> &w2) {
	int len_a = w1.size();
	int len_b = w2.size();
	int memo[100][100];
	memset(memo, 0x00, 100 * 100 * sizeof(int));
	for (int i = 1; i <= len_a; ++i) {
		memo[i][0] = i;
	}
	for (int j = 1; j <= len_b; ++j) {
		memo[0][j] = j;
	}
	for (int i = 1; i <= len_a; ++i) {
		for (int j = 1; j <= len_b; ++j) {
			if (w1[i - 1] == w2[j - 1]) {
				memo[i][j] = memo[i - 1][j - 1];
			} else {
				memo[i][j] = MIN(memo[i - 1][j - 1], memo[i][j - 1],
						memo[i - 1][j]) + 1;
			}
		}
	}
	return memo[len_a][len_b];
}

int edit_distance(const string &a, const string &b) {
	vector<uint16_t> w1, w2;
	parse_gbk_string(a, w1);
	parse_gbk_string(b, w2);
	return edit_distance_uint_16(w1, w2);
}

int edit_distance_old(const string &a, const string &b) {
	int len_a = a.size();
	int len_b = b.size();
	int memo[100][100];
	memset(memo, 0x00, 100 * 100 * sizeof(int));
	for (int i = 1; i <= len_a; ++i) {
		memo[i][0] = i;
	}
	for (int j = 1; j <= len_b; ++j) {
		memo[0][j] = j;
	}
	for (int i = 1; i <= len_a; ++i) {
		for (int j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1]) {
				memo[i][j] = memo[i - 1][j - 1];
			} else {
				memo[i][j] = MIN(memo[i - 1][j - 1], memo[i][j - 1],
						memo[i - 1][j]) + 1;
			}
		}
	}
	return memo[len_a][len_b];
}
