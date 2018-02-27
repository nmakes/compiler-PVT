// Naveen Venkat
// 2015A7PS0078P

#ifndef __PARSERDEF_H
#define __PARSERDEF_H

// #include "lexerDef.h"
#include "lexer.h"

struct __SET;
struct __LINKED_LIST_NODE;
struct __LINKED_LIST;
struct __GR_RHS;
struct __GR_LHS;
struct __GRAMMAR;
struct __FIRST_AND_FOLLOW;
struct __PARSE_TREE_NODE;
struct __PARSE_TREE;
typedef struct __SET * dt_set;
typedef struct __LINKED_LIST_NODE * dt_linkedListNode;
typedef struct __LINKED_LIST * dt_linkedList;
typedef struct __GR_RHS * gr_rhs;
typedef struct __GR_LHS * gr_lhs;
typedef struct __GRAMMAR * grammar;
typedef struct __FIRST_AND_FOLLOW * firstAndFollow;
typedef struct __PARSE_TREE_NODE * parseTreeNode;
typedef struct __PARSE_TREE * parseTree;

typedef int ** parseTable;

struct __SET
{
	dt_str elements;
	int size;
};

struct __LINKED_LIST_NODE
{
	void * data;
	int size;

	struct __LINKED_LIST_NODE * next;
	struct __LINKED_LIST_NODE * prev;
};

struct __LINKED_LIST
{
	// front [0]-[1]-[2]-...-[n] back
	struct __LINKED_LIST_NODE * front;
	struct __LINKED_LIST_NODE * back;
	int count;
};


// grammar

struct __GR_RHS
{
	// symbol sym;
	dt_id sym;
	gr_rhs next;
	gr_rhs prev;
};

struct __GR_LHS
{
	// symbol sym;
	dt_id sym;
	gr_rhs head;
	gr_rhs tail;
	int size;
};

struct __GRAMMAR
{
	gr_lhs * lhsArray; // array of LHS
	int size; // size of lhsArray
	int numTerminals;
	int numNonTerminals;
};

struct __FIRST_AND_FOLLOW
{
	dt_set * firstNT;
	dt_set * firstRules;
	dt_set * followNT;
};

// parse tree

struct __PARSE_TREE_NODE
{
	dt_id sym;
	struct __PARSE_TREE_NODE * parent;
	dt_linkedList children;
};

struct __PARSE_TREE
{
	parseTreeNode root;
};

#endif