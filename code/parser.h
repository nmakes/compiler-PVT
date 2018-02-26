#ifndef __PARSER_H
#define __PARSER_H

#include "parserDef.h"

// set datatype
dt_set setInit(int size);
void setKill(dt_set s);
void setAdd(dt_set s, int idx);
void setRemove(dt_set s, int idx);
dt_set setUnion(dt_set s1, dt_set s2);
dt_set setIntersection(dt_set s1, dt_set s2);
dt_set setDifference(dt_set s1, dt_set s2);

// linked list functions
dt_linkedListNode llMakeNode(void * data, int size);
void llFreeNode(dt_linkedListNode node);
void llPushBack(dt_linkedList ll, void * data, int size);
void llPushFront(dt_linkedList ll, void * data, int size);
void llPopFront(dt_linkedList ll);
void llPopBack(dt_linkedList ll);

// stack functions - stack is a linkedList with wrapper functions push and pop which use linkedList's pushFront and popFront
void * stackInit();
void * stackTopNode(dt_linkedList stack);
void stackPush(dt_linkedList stack, void * data, int size); // assuming data is newly malloc'd
void stackPop(dt_linkedList stack);

// grammar functions
gr_rhs grRHSMakeNode(dt_id sym);
void grRHSFreeNode(gr_rhs head);
int grRHSSize(gr_rhs head);
gr_lhs grLHSMakeNode(dt_id sym); // assuming sym is a newly malloc'd object
void grLHSAppendRHS(gr_lhs lhs, gr_rhs node); // assuming node is a newly malloc'd object
gr_lhs * grLHSInitArray(int size);
grammar grInitGrammar(int lhsArraySize, int numNonTerminals, int numTerminals);
grammar loadGrammar(FILE * grammarFile);
void printGrammar(grammar gr);
void printRule(grammar gr, int rule);

// first follow set computation
firstAndFollow computeFirstAndFollowSets(FILE * firstNT, FILE * firstRules, FILE * followNT);
void printFirstFollowSets(firstAndFollow ffSets, grammar gr);

// parse table creation
parseTable ptInitEmptyTable(int rows, int cols);
void createParseTable(firstAndFollow F, grammar gr, parseTable T);
void printParseTable(parseTable T, grammar gr);
parseTree parseInputSourceCode(char *testcaseFile, parseTable T);

#endif