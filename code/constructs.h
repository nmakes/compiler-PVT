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
		t->lexeme = strmake(lexeme);
		t->lineNo = lineNo;
		t->nextToken = NULL;
		t->prevToken = NULL;
	}
}

struct __TOKEN_LIST
{
	dt_token * head;
	dt_token * tail;
	dt_token * mov; // a movable pointer used for traversals
	int count;
};
typedef __TOKEN_LIST dt_tokenList;

void pushTokenList(dt_tokenList * tokenList, dt_token token)
{
	if(tokenList->head==NULL && tokenList->tail==NULL)
	{
		tokenList->head = tokenList->tail = token;
		tokenList->count = 1;
		tokenList->mov = tokenList->head;
	}
	else
	{
		token->prevToken = tokenList->tail;
		tokenList->tail->nextToken = token;
		tokenList->tail = token;
		tokenList->count++;
	}
}

dt_tokenList * popTokenList(int index=0)
{
	if(index==0)
	{
		tokenList->head = 
	}
}