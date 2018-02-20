#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct __LINKED_LIST_NODE;
struct __LINKED_LIST;

enum dt_flag{False, True};

enum dt_stateType {FINAL, NONFINAL, ERROR};

enum __DT_ID {TK_ASSIGNOP, TK_COMMENT, TK_FUNID, TK_ID, TK_NUM, TK_RNUM, 
			TK_STR, TK_END, TK_INT, TK_REAL, TK_STRING, TK_MATRIX, TK_MAIN, 
			TK_SQO, TK_SQC, TK_OP, TK_CL, TK_SEMICOLON, TK_COMMA, TK_IF, 
			TK_ELSE, TK_ENDIF, TK_READ, TK_PRINT, TK_FUNCTION, TK_PLUS, 
			TK_MINUS, TK_MUL, TK_DIV, TK_SIZE, TK_AND, TK_OR, TK_NOT, TK_LT, 
			TK_LE, TK_EQ, TK_GT, TK_LE, TK_NE};
typedef enum __DT_ID dt_id;

typedef char * dt_str;

int strlen(dt_str s)
{
	if (s==NULL)
	{
		return 0;
	}
	else
	{
		int i =0;
		while(s[i]!=0)
		{
			i++;
		}
		return i;	
	}
}

void strfree(dt_str base)
{ free(base); }

dt_str strmake(dt_str src)
{
	int sl = strlen(src);
	dt_str * dst = (dt_str) malloc(sl+1);
	
	if (dst==NULL)
	{
		printf("ERROR::helpers.h::strmake(): couldn't malloc dst");
		return NULL;
	}
	else
	{

		int i=0;
		for(i=0; i<sl; i++)
		{
			dst[i] = src[i];
		}
		dst[i] = '\0';

		return dst;
	}
}

int strcmp(dt_str s1, dt_str s2)
{
	l1 = strlen(s1);
	l2 = strlen(s2);
	
	if (strlen(s1) != strlen(s2))
		return 0;
	else
	{
		int i=0;
		for(i=0; i<l1; i++)
		{
			if(s1[i]!=s2[i])
				return 0;
		}
		return 1;
	}
}

dt_str strslice(dt_str s, int beg, int end) // s[beg:end] inclusive
{
	dt_str slice = (dt_str) malloc(sizeof(char)*(end-beg+1));
	
	int i=0;
	for(i=beg; i<=end; i++)
	{
		slice[i] = s[i];
	}

	return slice;
}

// ====================
// LINKED LIST DATATYPE
// ====================

typedef struct __LINKED_LIST_NODE dt_linkedListNode;
struct __LINKED_LIST_NODE
{
	void * data;
	int size;

	dt_linkedListNode * next;
	dt_linkedListNode * prev;
};

dt_linkedListNode * llMakeNode(void * data, int size)
{
	dt_linkedListNode * node = (dt_linkedListNode*) malloc(sizeof(dt_linkedListNode));
	
	if(node==NULL)
	{
		printf("ERROR::helpers.h::llMakeNode: could not malloc node");
	}
	else
	{
		node->data = data;
		node->size = size;
		node->prev = NULL;
		node->next = NULL;
	}

	return node;
}

typedef __LINKED_LIST dt_linkedList;
struct __LINKED_LIST
{
	dt_linkedListNode * head;
	dt_linkedListNode * tail;
	int count;
};

void llPushBack(dt_linkedList * l, void * data, int size, int pos=-1)
{
	if(pos==-1)
	{
		pos = l->count;
	}


	if(ll->head==NULL && ll->tail==NULL)
	{
		ll->head = ll->tail = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode * mov = l->

		// move pointer and insert in location

		dt_linkedListNode * node = llMakeNode(data, size);
		node->prev = ll->tail;
		ll->tail->next = node;
		ll->tail = node;
		ll->count++;
	}
}

void llPopFront(dt_linkedList * l)
{
	if(l->count<=0)
	{
		printf("ERROR::helpers.h::llPopFront: list is empty");
	}
	else if
	{
		
	}
}




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