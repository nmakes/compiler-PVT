#include "helpers.h"


#define BUFFER_SIZE 8192


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
	int lineNo;
	dt_token * prevToken;
	dt_token * nextToken;
};
typedef struct __TOKEN dt_token;

dt_token * tokenListHead = NULL;
dt_token * tokenListTail = NULL;


dt_token * make_token(dt_str lexeme, dt_id tokenID, int lineNo)
{
	token t = (token) malloc(sizeof(struct __TOKEN));

	if (t==NULL)
	{
		printf("ERROR::constructs.h::make_token(): couldn't malloc token");
		return NULL;
	}
	else
	{
		t->tokenID = tokenID;
		t->lexeme = strcpy(
		t->lineNo = lineNo;
		t->nextToken = NULL;
	}
}