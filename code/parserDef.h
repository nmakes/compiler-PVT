#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

struct __SET
struct __SYMBOL;
struct __GR_RHS;
struct __GR_LHS;
struct __GRAMMAR;
typedef struct __SET * dt_set;
typedef struct __SYMBOL * symbol;
typedef struct __GR_RHS * gr_rhs;
typedef struct __GR_LHS * gr_lhs;
typedef struct __GRAMMAR * grammar;

struct __SET
{
	dt_str elements;
	int size;
};

struct __SYMBOL;
{
	dt_str lexeme;
	dt_id tokID;
	int isTerminal;
};

struct __GR_RHS
{
	symbol sym;
	gr_rhs next;
};

struct __GR_LHS
{
	symbol sym;
	gr_rhs head;
	gr_rhs tail;
	int size;
};

struct __GRAMMAR
{
	gr_lhs lhsArray; // array of LHS
	int size; // size of lhsArray
};