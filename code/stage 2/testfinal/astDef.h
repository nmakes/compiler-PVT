// Naveen Venkat
// 2015A7PS0078P

#ifndef __ASTDEF_H
#define __ASTDEF_H

#include "parser.h"

struct __AST_NODE;
struct __AST;
typedef struct __AST_NODE * astNode;
typedef struct __AST * ast;

// ast
struct __AST_NODE
{
	dt_token token;
	int rule;
	astNode parent;
	astNode link;
	astNode children; // pointer to first child

	// attributes
	dt_str tag;
	dt_str type;
	dt_str code;
};

struct __AST
{
	astNode root;
	int numNodes;
};

#endif