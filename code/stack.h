#include "linkedList.h"

void stackPush(dt_linkedList stk, void * data, int size)
{
	llPushBack(stk, data, size);
}

void stackPop(dt_linkedList stk)
{
	llPopBack(stk);
}

void * stackTop(dt_linkedList stk)
{
	return stk->back;
}

int isEmpty(dt_linkedList stk)