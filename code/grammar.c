#include <stdio.h>
#include "grammarCfg.h"

#define MAX_BUF_SIZE 256

typedef char * dt_str;

struct __SYMBOL;
{
	dt_str lexeme;
	dt_id tokID;
	int isTerminal;
};
typedef struct __SYMBOL * symbol;

symbol makeSymbol(dt_str lexeme, dt_id tokID, int isTerminal)
{
	symbol s = (symbol) malloc(sizeof(struct __SYMBOL));
	s->lexeme = strmake(lexeme);
	s->tokID = tokID;
	s->isTerminal = isTerminal;
	return s;
}

// ========================================

struct __GR_RHS;
struct __GR_LHS;
struct __GRAMMAR;
typedef struct __GR_RHS * gr_rhs;
typedef struct __GR_LHS * gr_lhs;
typedef struct __GRAMMAR * grammar;

struct __GR_RHS
{
	symbol sym;
	gr_rhs next;
};

gr_rhs gr_rhs_makeNode(symbol sym) // assuming sym is a newly malloc'd object
{	
	gr_rhs rhs = (gr_rhs) malloc(sizeof(struct __GR_RHS));
	rhs->sym = sym;
	rhs->next = NULL;
	return rhs;
}

void gr_rhs_freeNode(gr_rhs head)
{
	if(head->next!=NULL)
	{
		freeRHS(head->next);
	}
	
	free(head);
}

int gr_rhs_countNodes(gr_rhs head)
{
	gr_rhs mov = head;
	int count = 0;
	while(mov!=NULL)
	{
		count++;
		mov = mov->next;
	}
	return count;
}

// ========================================

struct __GR_LHS
{
	symbol sym;
	gr_rhs head;
	gr_rhs tail;
	int count;
};

gr_lhs gr_lhs_makeNode(symbol sym) // assuming sym is a newly malloc'd object
{	
	gr_lhs lhs = (gr_lhs) malloc(sizeof(struct __GR_LHS));
	lhs->sym = sym;
	lhs->head = NULL;
	lhs->tail = NULL;
	lhs->count = 0;
	return lhs;
}

void gr_lhs_appendRHS(gr_lhs lhs, gr_rhs node) // assuming node is a newly malloc'd object
{
	if (lhs->size==0)
	{
		lhs->head = lhs->tail = node;
		lhs->count = 1;
	}
	else
	{
		lhs->tail->next = node;
		lhs->count++;
		lhs->tail = node;
	}
}

// ========================================

struct __GRAMMAR
{
	int num_NT;
	int num_T;
	gr_lhs lhsArray; // array of LHS
	int count;
};
typedef struct __GRAMMAR * grammar;

grammar gr_makeGrammar(

gr_lhs gr_makeLHSArray(grammar gr, int count)
{
	gr->lhsArray = (gr_lhs) malloc(sizeof(__GR_RHS)*count);
}

// ========================================

struct __BUFFER
{
	dt_str string;
	int length;
};
typedef struct __BUFFER * buffer;

readGrammarFromFile(FILE * f)

