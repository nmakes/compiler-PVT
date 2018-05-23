// Naveen Venkat
// 2015A7PS0078P

#ifndef __AST_H
#define __AST_H

#include "astDef.h"

astNode astMakeNode(		dt_token token, 
							int rule, 
							astNode parent,
							astNode link,
							astNode children,
							dt_str tag,
							dt_str type,
							dt_str code);

void astFreeNode(astNode node);

void astPrintNode(astNode node, int indent);
int astPrintInOrder(astNode root, int indent);
void astPrintTree(ast tree);

ast astInitNode(dt_token token);
astNode buildAstNode(parseTreeNode node, astNode parent);


#endif