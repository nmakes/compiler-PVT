#include "parserDef.h"

dt_set setInit(int size);
void setKill(set s);
void setAdd(dt_set s, int idx);
void setRemove(dt_set s, int idx);
dt_set setUnion(dt_set s1, dt_set s2);
dt_set setIntersection(dt_set s1, dt_set s2);
dt_set setDifference(dt_set s1, dt_set s2);

symbol makeSymbol(dt_str lexeme, dt_id tokID, int isTerminal);
gr_rhs grRHSMakeNode(symbol sym);
void grRHSFreeNode(gr_rhs head);
int grRHSSize(gr_rhs head);
gr_lhs grLHSMakeNode(symbol sym); // assuming sym is a newly malloc'd object
void grLHSAppendRHS(gr_lhs lhs, gr_rhs node); // assuming node is a newly malloc'd object
grammar grMakeGrammar();// incomplete
gr_lhs grMakeInitArray(grammar gr, int size);