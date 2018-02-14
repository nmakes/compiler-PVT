#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef char * str;
typedef long long int num;
typedef double rnum;

struct __STRING
{
	char * c_str;
	unsigned long int length;
	unsigned long int maxlength;
};

typedef struct __STRING * str;

num str_length_ceil(unsigned num size)
{
	rnum logval = log(size)/log(2);
	num power = logval;
	power++;
	num upper = (num) pow(2, power);
	return upper-1;
}

num str_clen(const char * s)
{
	num l = 0;
	while(s[l]!=0)
		l++;
	return l;
}

void str_ccopy(const char * dst, const char * src)
{
	num l = str_clen(src);
	num i=0;

	for(i=0; i<l; i++)
	{
		dst[i] = src[i];
	}

	dst[i] = 0;
}

str str_empty(unsigned num size)
{
	str s = (str) malloc(sizeof(__STRING));
	s.c_str = (char *) malloc(str_length_ceil(size));
	
	unsigned num i;
	for (i=0; i<size; i++)
		s.c_str[i] = 0;

	s.maxlength = str_length_ceil(size);
	s.length = 0;
}

str str_init(const char * s)
{
	str S = str_empty(str_clen(s));
	str_ccopy(S.c_str, s);
	return S;
}

str str_append(str s1, str s2)
{	
	if (s1.length + s2.length > s1.maxlength)
	{
		
	}
}