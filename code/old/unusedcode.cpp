/*
struct __STRING
{
	char * c_str;
	unsigned long int length;
	unsigned long int maxlength;
};

typedef struct __STRING * dt_str;

dt_num str_length_ceil(unsigned dt_num size)
{
	dt_rnum logval = log(size)/log(2);
	dt_num power = logval;
	power++;
	dt_num upper = (dt_num) pow(2, power);
	return upper-1;
}

dt_num str_clen(const char * s)
{
	dt_num l = 0;
	while(s[l]!=0)
		l++;
	return l;
}

void str_ccopy(const char * dst, const char * src)
{
	dt_num l = str_clen(src);
	dt_num i=0;

	for(i=0; i<l; i++)
	{
		dst[i] = src[i];
	}

	dst[i] = 0;
}

dt_str str_empty(unsigned dt_num size)
{
	dt_str s = (dt_str) malloc(sizeof(__STRING));
	s.c_str = (char *) malloc(str_length_ceil(size));
	
	unsigned dt_num i;
	for (i=0; i<size; i++)
		s.c_str[i] = 0;

	s.maxlength = str_length_ceil(size);
	s.length = 0;
}

dt_str str_init(const char * s)
{
	dt_str S = str_empty(str_clen(s));
	str_ccopy(S.c_str, s);
	return S;
}

dt_str str_append(dt_str s1, dt_str s2)
{	
	if (s1.length + s2.length > s1.maxlength)
	{
		
	}
}

*/