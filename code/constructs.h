#include "helpers.h"


struct __SYMBOL_TABLE_NODE
{
	dt_str lexeme;
	dt_id tokenName;
	dt_num lineNo;
};
typedef struct __SYMBOL_TABLE_NODE dt_symbolTableNode;


struct __TOKEN
{
	dt_id tokenName;
	dt_symbolTableNode * attributeValue;
	token nextToken;
};
typedef struct __TOKEN * token;

dt_token make_token(dt_str lexeme, dt_id tokenID, dt_num lineNo)
{}