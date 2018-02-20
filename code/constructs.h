#include "helpers.h"

#define BUFFER_SIZE 8192
#define MAX_LEXEME_SIZE 128

struct __SYMBOL_TABLE_NODE
{
	dt_str lexeme;
	dt_id tokenName;
	unsigned long int lineNo;
};
typedef struct __SYMBOL_TABLE_NODE dt_symbolTableNode;

dt_symbolTableNode make_symbolTableNode();

struct __TOKEN
{
	dt_id tokenID;
	dt_str lexeme;
	void * value;
	int lineNo;
};
typedef struct __TOKEN * dt_token;


dt_token make_token(dt_str lexeme, dt_id tokenID, int lineNo)
{
	dt_token t = (dt_token) malloc(sizeof(struct __TOKEN));

	if (t==NULL)
	{
		printf("ERROR::constructs.h::make_token(): couldn't malloc token");
		return NULL;
	}
	else
	{
		t->tokenID = tokenID;
		t->lexeme = strmake(lexeme);
		t->lineNo = lineNo;
		t->value = NULL
	}

	return t;
}