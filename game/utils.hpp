#ifndef __UTILS
#define __UTILS

#include <string>
#include "foliage.hpp"

void append_string(std::string &s, Sint32 n, Sint32 min_len = 1)
{
	char buf[10]; // max=2147483647
	int buf_len = 0;
	if (n == 0)
	{
		s += '0';
		return;
	}
	else if (n < 0)
	{
		s += '-';
		n = -n;
	}
	while (n != 0)
	{
		int digit = n % 10;
		n /= 10;
		buf[buf_len++] = '0' + digit;
	}
	for (Sint32 i = 0; i < (min_len - buf_len - 1); ++i)
	{
		s += '0';
	}
	while (buf_len > 0)
	{
		s += buf[--buf_len];
	}
}

#endif //__UTILS
