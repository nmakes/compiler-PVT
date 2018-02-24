#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

struct __SET
{
	dt_str elements;
	int size;
};
typedef __SET * dt_set;