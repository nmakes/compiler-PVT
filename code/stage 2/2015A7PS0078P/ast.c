// Naveen Venkat
// 2015A7PS0078P

#include "astDef.h"

char * tokenTable3[ENUM_SEP_NONTERMINALS+1] = {
// TERMINALS
"ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", 
"INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", 
"CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", 
"FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", 
"NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "_epsilon_", "ENUM_SEP_TERMINALS",

// CONTROL SYMBOLS
"EXIT", "ABRUPTEND", "ENUM_SEP_CONTROL", 

// NON TERMINALS
"mainFunction", "stmtsAndFunctionDefs", "other1", "stmtOrFunctionDef", "stmt", 
"functionDef", "parameter_list", "remainingList", "type", "declarationStmt", 
"var_list", "more_ids", "assignmentStmt_type1", "assignmentStmt_type2", 
"leftHandSide_singleVar", "leftHandSide_listVar", "rightHandSide_type1", 
"rightHandSide_type2", "sizeExpression", "ifStmt", "other2", "otherStmts", 
"ioStmt", "funCallStmt", "inputParameterList", "listVar", "arithmeticExpression", 
"other3", "arithmeticTerm", "other4", "factor", "operator_lowPrecedence", 
"operator_highPrecedence", "booleanExpression", "constrainedVars", "var", "matrix", 
"rows", "other5", "row", "remainingColElements", "matrixElementExtension", 
"logicalOp", "relationalOp", "ENUM_SEP_NONTERMINALS"
};

char * indentSymbol = "  ";

astNode astMakeNode(		dt_token token, 
							int rule, 
							astNode parent,
							astNode link,
							astNode children,
							dt_str tag,
							dt_str type,
							dt_str code)
{
	astNode node = (astNode) malloc(sizeof(struct __AST_NODE));
	node->token = token;
	node->rule = rule;
	node->parent = parent;
	node->link = link;
	node->children = children;

	node->tag = strmake(tag);
	node->type = strmake(type);
	node->code = strmake(code);

	return node;
}


astNode tempInherited = NULL; // for inheriting attributes

void astFreeNode(astNode node)
{

}

void astPrintNode(astNode node, int indent)
{
	if(node!=NULL)
	{
		int i;

		for(i=1; i<indent; i++)
		{
			printf("%s", indentSymbol);
		}

		printf("|");

		if(node->tag!=NULL)
		{ printf("%s", node->tag); }

		if(node->token!=NULL)
		{
			if(isTerminal(node->token->tokenID))
			{
				printf(" (%s)", node->token->lexeme);
			}
		}
		printf("\n");
	}
}

int astPrintInOrder(astNode root, int indent)
{
	if(root==NULL)
	{
		return 0;
	}
	else if(root->children==NULL)
	{
		astPrintNode(root, indent);
		return 1;
	}
	else
	{
		int i=0; int j;

		astPrintNode(root, indent);
		i += 1;

		astNode child = root->children;

		while(child!=NULL)
		{
			i += astPrintInOrder(child, indent+1);
			child = child->link;
		}

		return i;
	}
}

void astPrintTree(ast tree)
{
	printf("\n~~~~ ABSTRACT SYNTAX TREE ~~~~\n\n");
	// printf("-------------------------------------------------------------------------------------------------------------------------------\n");
	// printf("%-20s | %-4s | %-24s | %-10s | %-24s | %-5s | %-24s\n", "lexeme", "line", "token", "value", "parent node", "leaf", "node symbol");
	// printf("-------------------------------------------------------------------------------------------------------------------------------\n");
	int numNodes = astPrintInOrder(tree->root, 1);
	// printf("-------------------------------------------------------------------------------------------------------------------------------\n");
	tree->numNodes = numNodes;
}

ast astInitNode(dt_token token)
{
	ast tree = (ast) malloc(sizeof(struct __PARSE_TREE));
	tree->root = astMakeNode(token, -1, NULL, NULL, NULL, NULL, NULL, NULL);
	tree->numNodes = 1;
	return tree;
}

astNode buildAstNode(parseTreeNode node, astNode parent)
{
	// printf("ptn: %s\n", tokenTable3[node->token->tokenID]);

	if (node->rule==0)
	{
		// <mainFunction> ===> MAIN SQO SQC <stmtsAndFunctionDefs> END
		// {<mainFunction> = newnode("MAINFUNCTION", <stmtsAndFunctionDefs>.addr)}
	
		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "MAINFUNCTION", NULL, NULL);
		me->children = buildAstNode(node->children[3], me);
		return me;
	}
	else if (node->rule==1)
	{
		// <stmtsAndFunctionDefs> ===> <stmtOrFunctionDef> <other1>
		// {<stmtOrFunctionDef>.carry.link = <other1>.carry}, {<stmtsAndFunctionDefs> = newnode("SAFD", <stmtOrFunctionDef>.carry)}

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "SAFD", NULL, NULL);
		me->children = buildAstNode(node->children[0], me);
		me->children->link = buildAstNode(node->children[1], me);
		return me;
	}
	else if (node->rule==2)
	{
		// <other1> ===> _epsilon_
		// {<other1>.carry=NULL}, {free(_epsilon_)}

		astNode me = NULL;
		return me;
	}
	else if (node->rule==3)
	{
		// <other1> ===> <stmtOrFunctionDef> <other1>
		// {<other1>_1.carry = <stmtOrFunctionDef>.carry}, {<stmtOrFunctionDef>.carry.link = <other1>_2.carry}

		astNode me = buildAstNode(node->children[0], parent);
		me->link = buildAstNode(node->children[1], parent);
		return me;
	}
	else if (node->rule==4)
	{
		// <stmtOrFunctionDef> ===> <stmt>
		// {<stmtOrFunctionDef>.carry = <stmt>.carry}

		astNode me = buildAstNode(node->children[0], parent);
		return me;
	}
	else if (node->rule==5)
	{
		// <stmtOrFunctionDef> ===> <functionDef>
		// {<stmtOrFunctionDef>.carry = <functionDef>.addr}

		parseTreeNode fd = node->children[0];
		astNode me = buildAstNode(fd, parent);
		return me;
	}
	else if (node->rule==6)
	{
		// <stmt> ===> <declarationStmt>
		// {<stmt>.carry = <declarationStmt>.addr}

		parseTreeNode ds = node->children[0];
		astNode me = buildAstNode(ds, parent);
		return me;
	}
	else if (node->rule==7)
	{
		// <stmt> ===> <assignmentStmt_type1>
		// {<stmt>.carry = <assignmentStmt_type1>.addr}

		parseTreeNode as1 = node->children[0];
		astNode me = buildAstNode(as1, parent);
		return me;
	}
	else if (node->rule==8)
	{
		// <stmt> ===> <assignmentStmt_type2>
		// {<stmt>.carry = <assignmentStmt_type2>.addr}

		parseTreeNode as2 = node->children[0];
		astNode me = buildAstNode(as2, parent);
		return me;
	}
	else if (node->rule==9)
	{
		// <stmt> ===> <ifStmt>
		// {<stmt>.carry = <ifStmt>.addr}

		parseTreeNode as2 = node->children[0];
		astNode me = buildAstNode(as2, parent);
		return me;
	}
	else if (node->rule==10)
	{
		// <stmt> ===> <ioStmt>
		// {<stmt>.carry = <ioStmt>.addr}

		parseTreeNode ios = node->children[0];
		astNode me = buildAstNode(ios, parent);
		return me;
	}
	else if (node->rule==11)
	{
		// <stmt> ===> <funCallStmt> SEMICOLON
		// {<stmt>.carry = <funCallStmt>.addr}, {free(SEMICOLON)}

		parseTreeNode fcs = node->children[0];
		astNode me = buildAstNode(fcs, parent);
		return me;
	}
	else if (node->rule==12)
	{
		// <functionDef> ===> FUNCTION SQO <parameter_list> SQC ASSIGNOP FUNID SQO <parameter_list> SQC <stmtsAndFunctionDefs> END SEMICOLON
		// {<functionDef> = newnode(<parameter_list>_1.addr, FUNID.addr, <parameter_list>_2.addr, <stmtsAndFunctionDefs>.addr)}

		parseTreeNode pl1 = node->children[2];
		parseTreeNode pl2 = node->children[7];
		parseTreeNode safd = node->children[9];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "FUNCDEF", NULL, NULL);
		me->children = buildAstNode(pl1, me);
		me->children->link = buildAstNode(pl2, me);
		me->children->link->link = buildAstNode(safd, me);
		return me;

	}
	else if (node->rule==13)
	{
		// <parameter_list> ===> <type> ID <remainingList>
		// {<type>.link = ID.addr}, {ID.link = <remainingList>.carry}, {free(<remainingList>)}, {<parameter_list> = newnode("PL", <type>.addr)}

		parseTreeNode type = node->children[0];
		parseTreeNode id = node->children[1];
		parseTreeNode rl = node->children[2];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "PL", NULL, NULL);
		me->children = buildAstNode(type, me);
		me->children->link = astMakeNode(id->token, id->rule, me, NULL, NULL, "ID", NULL, NULL);
		me->children->link->link = buildAstNode(rl, me);
		return me;
	}
	else if (node->rule==14)
	{
		// <remainingList> ===> COMMA <type> ID <remainingList>
		// {<type>.link = ID.addr}, {ID.link = <remainingList>_2.carry}, {<remainingList>_1.carry = <type>.addr}

		parseTreeNode type = node->children[1];
		parseTreeNode id = node->children[2];
		parseTreeNode rl = node->children[3];

		astNode me = buildAstNode(type, parent);
		me->children->link = buildAstNode(id, parent);
		me->children->link->link = buildAstNode(rl, parent);
		return me;
	}
	else if (node->rule==15)
	{
		// <remainingList> ===> _epsilon_
		// {<remainingList>.carry = NULL}, {free(_epsilon_)}

		astNode me = NULL;
		return me;
	}	
	else if (node->rule==16)
	{
		// <type> ===> INT
		// {<type>.carry = INT.addr}
		
		parseTreeNode typenode = node->children[0];

		astNode me = astMakeNode(typenode->token, typenode->rule, parent, NULL, NULL, "TYPE", typenode->token->lexeme, NULL);
		return me;
	}
	else if (node->rule==17)
	{
		// <type> ===> REAL
		// {<type>.carry = REAL.addr}

		parseTreeNode typenode = node->children[0];

		astNode me = astMakeNode(typenode->token, typenode->rule, parent, NULL, NULL, "TYPE", typenode->token->lexeme, NULL);
		return me;
	}
	else if (node->rule==18)
	{
		// <type> ===> STRING
		// {<type>.carry = STRING.addr}

		parseTreeNode typenode = node->children[0];

		astNode me = astMakeNode(typenode->token, typenode->rule, parent, NULL, NULL, "TYPE", typenode->token->lexeme, NULL);
		return me;
	}
	else if (node->rule==19)
	{
		// <type> ===> MATRIX
		// {<type>.carry = MATRIX.addr}

		parseTreeNode typenode = node->children[0];

		astNode me = astMakeNode(typenode->token, typenode->rule, parent, NULL, NULL, "TYPE", typenode->token->lexeme, NULL);
		return me;
	}
	else if (node->rule==20)
	{
		// <declarationStmt> ===> <type> <var_list> SEMICOLON
		// {<declarationStmt> = newnode("DEC", <type>.carry, <var_list>.carry)}

		parseTreeNode type = node->children[0];
		parseTreeNode varlist = node->children[1];
		
		astNode me = astMakeNode(NULL, node->rule, parent, NULL, NULL, "DECLSTMT", NULL, NULL);
		me->children = buildAstNode(type, me);
		me->children->link = buildAstNode(varlist, me);
		return me;
	}
	else if (node->rule==21)
	{
		// <var_list> ===> ID <more_ids>
		// {ID.link = <more_ids>.carry}, {<var_list>.carry = ID.addr}
		
		parseTreeNode id = node->children[0];
		parseTreeNode moreids = node->children[1];

		astNode me = astMakeNode(id->token, id->rule, parent, NULL, NULL, "ID", NULL, NULL);
		me->link = buildAstNode(moreids, parent);
		return me;
	}
	else if (node->rule==22)
	{
		// <more_ids> ===> COMMA ID <more_ids>
		// {ID.link = <more_ids>.carry}, {<more_ids>.carry = ID.addr}

		parseTreeNode id = node->children[1];
		parseTreeNode moreids = node->children[2];
		
		astNode me = astMakeNode(id->token, id->rule, parent, NULL, NULL, "ID", NULL, NULL);
		me->link = buildAstNode(moreids, parent);
		return me;
	}
	else if (node->rule==23)
	{
		// <more_ids> ===> _epsilon_
		// {<more_ids>.carry = NULL}, {free(_epsilon_)}	

		astNode me = NULL;
		return me;
	}
	else if (node->rule==24)
	{
		// <assignmentStmt_type1> ===> <leftHandSide_singleVar> ASSIGNOP <rightHandSide_type1> SEMICOLON
		// {<assignmentStmt_type1> = newnode("=1", <leftHandSide_singleVar>.carry, <rightHandSide_type1>.carry)}

		parseTreeNode lhs = node->children[0];
		parseTreeNode rhs = node->children[2];
		
		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "=1", NULL, NULL);
		me->children = buildAstNode(lhs, me);
		me->children->link = buildAstNode(rhs, me);
		return me;
	}
	else if (node->rule==25)
	{
		// <assignmentStmt_type2> ===> <leftHandSide_listVar> ASSIGNOP <rightHandSide_type2> SEMICOLON
		// {<assignmentStmt_type2> = newnode("=2", <leftHandSide_listVar>.carry, <rightHandSide_type2>.carry)}

		parseTreeNode lhs = node->children[0];
		parseTreeNode rhs = node->children[2];
		
		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "=2", NULL, NULL);
		me->children = buildAstNode(lhs, me);
		me->children->link = buildAstNode(rhs, me);
		return me;
	}
	else if (node->rule==26)
	{
		// <leftHandSide_singleVar> ===> ID
		// {<leftHandSide_singleVar>.carry = ID.addr}

		parseTreeNode id = node->children[1];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "LHS_SV_ID", NULL, NULL);
		return me;
	}
	else if (node->rule==27)
	{
		// <leftHandSide_listVar> ===> SQO <var_list> SQC
		// {<leftHandSide_listVar>.carry = newnode(<var_list>.carry)}

		parseTreeNode varlist = node->children[1];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "LHS_LV", NULL, NULL);
		me->children = buildAstNode(varlist, me);
		return me;
	}
	else if (node->rule==28)
	{
		// <rightHandSide_type1> ===> <arithmeticExpression>
		// {<rightHandSide_type1>.carry = <arithmeticExpression>.carry}, {free(<arithmeticExpression>)}

		parseTreeNode ae = node->children[0];

		astNode me = buildAstNode(ae, parent);
		return me;
	}
	else if (node->rule==29)
	{
		// <rightHandSide_type1> ===> <sizeExpression>
		// {<rightHandSide_type1>.carry = <sizeExpression>.addr}

		parseTreeNode se = node->children[0];

		astNode me = buildAstNode(se, parent);
		return me;
	}
	else if (node->rule==30)
	{
		// <rightHandSide_type1> ===> <funCallStmt>
		// {<rightHandSide_type1>.carry = <funCallStmt>.addr}

		parseTreeNode fcs = node->children[0];

		astNode me = buildAstNode(fcs, parent);
		return me;
	}
	else if (node->rule==31)
	{
		// <rightHandSide_type2> ===> <sizeExpression>
		// {<rightHandSide_type2>.carry = <sizeExpression>.addr}

		parseTreeNode se = node->children[0];

		astNode me = buildAstNode(se, parent);
		return me;
	}
	else if (node->rule==32)
	{
		// <rightHandSide_type2> ===> <funCallStmt>
		// {<rightHandSide_type2>.carry = <funCallStmt>.addr}

		parseTreeNode fcs = node->children[0];

		astNode me = buildAstNode(fcs, parent);
		return me;
	}
	else if (node->rule==33)
	{
		// <sizeExpression> ===> SIZE ID
		// {free(SIZE)}, {<sizeExpression> = newnode("SIZE", ID.addr)}

		parseTreeNode size = node->children[0];
		parseTreeNode id = node->children[1];

		astNode me = astMakeNode(size->token, size->rule, parent, NULL, NULL, "@", NULL, NULL);
		me->children = astMakeNode(id->token, id->rule, parent, NULL, NULL, "ID", NULL, NULL);
		return me;
	}
	else if (node->rule==34)
	{
		// <ifStmt> ===> IF OP <booleanExpression> CL <stmt> <otherStmts> <other2>
		// {<stmt>.link = <otherStmts>.carry}, {<ifStmt> = newnode("IF", <booleanExpression>.addr, <stmt>.carry, <other2>.carry)}

		parseTreeNode ifnode = node->children[0];
		parseTreeNode be = node->children[2];
		parseTreeNode stmt = node->children[4];
		parseTreeNode otherstmts = node->children[5];
		parseTreeNode other2 = node->children[6];

		astNode me = astMakeNode(ifnode->token, ifnode->rule, parent, NULL, NULL, "IF", NULL, NULL);
		me->children = buildAstNode(be, me);
		me->children->link = buildAstNode(stmt, me);
		
		astNode temp = buildAstNode(otherstmts, me);
		astNode o2temp = buildAstNode(other2, me);

		if(temp==NULL)
		{
			me->children->link->link = o2temp;
		}
		else
		{
			me->children->link->link = temp;
			me->children->link->link->link = o2temp;
		}

		return me;
	}
	else if (node->rule==35)
	{
		// <other2> ===> ELSE <stmt> <otherStmts> ENDIF SEMICOLON
		// {<stmt>.carry.link = <otherStmts>.carry}, {<other2>.carry = newnode("ELSE", <stmt>.carry)}

		parseTreeNode elsenode = node->children[0];
		parseTreeNode stmt = node->children[1];
		parseTreeNode otherstmts = node->children[2];

		astNode me = astMakeNode(elsenode->token, elsenode->rule, parent, NULL, NULL, "ELSE", NULL, NULL);
		me->children = buildAstNode(stmt, me);
		me->children->link = buildAstNode(otherstmts, me);

		return me;
	}
	else if (node->rule==36)
	{
		// <other2> ===> ENDIF SEMICOLON
		// {<other2>.carry = NULL}, {free(ENDIF)}, {free(SEMICOLON)}

		return NULL;
	}
	else if (node->rule==37)
	{
		// <otherStmts> ===> <stmt> <otherStmts>
		// {<otherStmts>_1.carry = <stmt>}, {<stmt>.link = <otherStmts>_2.carry}

		parseTreeNode stmt = node->children[0];
		parseTreeNode otherstmts = node->children[1];

		astNode me = buildAstNode(stmt, parent);
		me->link = buildAstNode(otherstmts, parent);
		return me;
	}
	else if (node->rule==38)
	{
		// <otherStmts> ===> _epsilon_
		// {<otherStmts>.carry = NULL}

		return NULL;
	}
	else if (node->rule==39)
	{
		// <ioStmt> ===> READ OP ID CL SEMICOLON
		// {<ioStmt> = newnode("IO_READ", ID.addr)}

		parseTreeNode id = node->children[2];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "IO_READ", NULL, NULL);
		me->children = astMakeNode(id->token, id->rule, parent, NULL, NULL, "ID", NULL, NULL);
		return me;
	}
	else if(node->rule==40)
	{
		// <ioStmt> ===> PRINT OP ID CL SEMICOLON
		// {<ioStmt> = newnode("IO_PRINT", ID.addr)}

		parseTreeNode id = node->children[2];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "IO_PRINT", NULL, NULL);
		me->children = astMakeNode(id->token, id->rule, parent, NULL, NULL, "ID", NULL, NULL);
		return me;
	}
	else if (node->rule==41)
	{
		// <funCallStmt> ===> FUNID OP <inputParameterList> CL
		// {<funCallStmt> = newnode("CALL", FUNID.addr, <inputParameterList>.addr)

		parseTreeNode funid = node->children[0];
		parseTreeNode ipl = node->children[2];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "FUNCALL", NULL, NULL);
		me->children = buildAstNode(ipl, me);
		return me;
	}
	else if (node->rule==42)
	{
		// <inputParameterList> ===> <var> <listVar>
		// {<var>.carry.link = <listVar>.carry}, {free(<var>)}, {free(<listVar>)}

		parseTreeNode var = node->children[0];
		parseTreeNode lvar = node->children[1];

		astNode me = buildAstNode(var, parent);
		me->link = buildAstNode(lvar, parent);
		return me;
	}
	else if (node->rule==43)
	{
		// <inputParameterList> ===> _epsilon_
		// {<inputParameterList>.carry = NULL}, {free(_epsilon_)}

		return NULL;
	}
	else if (node->rule==44)
	{
		// <listVar> ===> _epsilon_
		// {<listVar>.carry = NULL}, {free(_epsilon_)}

		return NULL;
	}
	else if (node->rule==45)
	{
		// <listVar> ===> COMMA <var> <listVar>
		// {free(COMMA)}, {<var>.carry.link = <listVar>_2.carry}, {free(<listVar>_2)}, {free(<var>)}

		parseTreeNode var = node->children[1];
		parseTreeNode lvar = node->children[2];

		astNode me = buildAstNode(var, parent);
		me->link = buildAstNode(lvar, parent);
		return me;
	}
	else if (node->rule==46)
	{
		// <arithmeticExpression> ===> <arithmeticTerm> <other3>
		// {<other3>.inh = <arithmeticTerm>.carry}, {<arithmeticTerm>.carry = <other3>.carry}, {free(<other3>)}

		parseTreeNode at = node->children[0];
		parseTreeNode o3 = node->children[1];

		astNode ATnode = buildAstNode(at, parent);
		tempInherited = ATnode;
		
		astNode me = buildAstNode(o3, parent);
		ATnode->parent = me;
		
		return me;
	}
	else if (node->rule==47)
	{
		// <other3> ===> <operator_lowPrecedence> <arithmeticTerm> <other3>
		// {<other3>_2.inh = <arithmeticTerm>.carry}, {<other3>_1.carry = newnode(<operator_lowPrecedence>.symbol, <other3>_1.inh, <arithmeticTerm>.carry)}

		parseTreeNode olp = node->children[0];
		parseTreeNode at = node->children[1];
		parseTreeNode o3 = node->children[2];
		
		astNode me = buildAstNode(olp, parent);
		me->children = tempInherited;
		me->children->parent = me;
		
		astNode ATnode = buildAstNode(at, parent);
		tempInherited = ATnode;
		
		astNode o3node2 = buildAstNode(o3, me);
		me->children->link = o3node2;

		if(o3node2!=NULL)
		{
			o3node2->parent = me;
		}
		return me;

	}
	else if (node->rule==48)
	{
		// <other3> ===> _epsilon_
		// {<other3>.carry = <other3>.inh}, {free(_epsilon_)}

		return NULL;
	}
	else if (node->rule==49)
	{
		// <arithmeticTerm> ===> <factor> <other4>
		// {<other4>.inh = <factor>.carry}, {<arithmeticTerm>.carry = <other4>.carry}, {free(<other4>)}

		parseTreeNode f = node->children[0];
		parseTreeNode o4 = node->children[1];

		astNode Fnode = buildAstNode(f, parent);
		tempInherited = Fnode;
		
		astNode me = buildAstNode(o4, parent);
		Fnode->parent = me;
		
		return me;
	}
	else if (node->rule==50)
	{
		// <other4> ===> <operator_highPrecedence> <factor> <other4>
		// {<other4>_2.inh = <factor>.carry}, {<other4>_1.carry = newnode(<operator_highPrecedence>.symbol, <other4>_1.inh, <factor>.carry)}, {free(<other4>_2)}, {free(<operator_highPrecedence>)}, {free(<factor>)}

		parseTreeNode ohp = node->children[0];
		parseTreeNode f = node->children[1];
		parseTreeNode o4 = node->children[2];
		
		astNode me = buildAstNode(ohp, parent);
		me->children = tempInherited;
		me->children->parent = me;
		
		astNode Fnode = buildAstNode(f, parent);
		tempInherited = Fnode;
		
		astNode o4node2 = buildAstNode(o4, me);
		me->children->link = o4node2;

		if(o4node2!=NULL)
		{
			o4node2->parent = me;
		}
		return me;
	}
	else if (node->rule==51)
	{
		// <other4> ===> _epsilon_
		// {<other4>.carry = <other4>.inh}, {free(_epsilon_)}

		return NULL;
	}
	else if (node->rule==52)
	{
		// <factor> ===> OP <arithmeticExpression> CL
		// {free(OP)}, {free(CL)}, {<factor>.carry = <arithmeticExpression>.carry}, {free(<arithmeticExpression>)}

		astNode me = buildAstNode(node->children[1], parent);
		return me;
	}
	else if (node->rule==53)
	{
		// <factor> ===> <var>
		// {<factor>.carry = <var>.carry}, {free(<var>)}

		return buildAstNode(node->children[0], parent);
	}
	else if (node->rule==54)
	{
		// <operator_lowPrecedence> ===> PLUS
		// {<operator_lowPrecedence>.carry = PLUS.addr}

		parseTreeNode op = node->children[0];

		astNode me = astMakeNode(op->token, op->rule, parent, NULL, NULL, op->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==55)
	{
		// <operator_lowPrecedence> ===> MINUS
		// {<operator_lowPrecedence>.carry = MINUS.addr}

		parseTreeNode op = node->children[0];

		astNode me = astMakeNode(op->token, op->rule, parent, NULL, NULL, op->token->lexeme, NULL, NULL);
		return me;	
	}
	else if (node->rule==56)
	{
		// <operator_highPrecedence> ===> MUL
		// {<operator_highPrecedence>.carry = MUL.addr}

		parseTreeNode op = node->children[0];

		astNode me = astMakeNode(op->token, op->rule, parent, NULL, NULL, op->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==57)
	{
		// <operator_highPrecedence> ===> DIV
		// {<operator_highPrecedence>.carry = DIV.addr}

		parseTreeNode op = node->children[0];

		astNode me = astMakeNode(op->token, op->rule, parent, NULL, NULL, op->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==58)
	{
		// <booleanExpression> ===> OP <booleanExpression> CL <logicalOp> OP <booleanExpression> CL
		// {free(OP_1)}, {free(CL_1)}, {free(OP_2)}, {free(CL_2)}, {<booleanExpression>_1 = newnode(<logicalOp>.carry.symbol, <booleanExpression>_2, <booleanExpression>_3)}

		parseTreeNode lop = node->children[3];
		parseTreeNode be1 = node->children[1];
		parseTreeNode be2 = node->children[5];
		
		astNode me = astMakeNode(lop->token, lop->rule, parent, NULL, NULL, lop->token->lexeme, NULL, NULL);
		me->children = buildAstNode(be1, me);
		me->children->link = buildAstNode(be2, me);
		return me;

	}
	else if (node->rule==59)
	{
		// <booleanExpression> ===> <constrainedVars> <relationalOp> <constrainedVars>
		// {<booleanExpression> = newnode(<relationalOp>.carry.symbol, <constrinedVars>_1.carry, <constrinedVars>_2.carry)}, {free(<constrainedVars>_1)}, {free(<constrainedVars>_2)}

		parseTreeNode rop = node->children[1];
		parseTreeNode cv1 = node->children[0];
		parseTreeNode cv2 = node->children[2];
		
		astNode me = astMakeNode(rop->token, rop->rule, parent, NULL, NULL, rop->token->lexeme, NULL, NULL);
		me->children = buildAstNode(cv1, me);
		me->children->link = buildAstNode(cv2, me);
		return me;
	}
	else if (node->rule==60)
	{
		// <booleanExpression> ===> NOT OP <booleanExpression> CL
		// {free(NOT)}, {free(OP)}, {free(CL)}, {<booleanExpression>_1 = newnode("NOT", <booleanExpression>_2)}

		parseTreeNode not = node->children[0];
		parseTreeNode be = node->children[2];
		
		astNode me = astMakeNode(not->token, not->rule, parent, NULL, NULL, not->token->lexeme, NULL, NULL);
		me->children = buildAstNode(be, me);
		return me;
	}
	else if (node->rule==61)
	{
		// <constrainedVars> ===> ID
		// {<constrinedVars>.carry = ID.addr}

		parseTreeNode id = node->children[0];

		astNode me = astMakeNode(id->token, id->rule, parent, NULL, NULL, id->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==62)
	{
		// <constrainedVars> ===> NUM
		// {<constrinedVars>.carry = NUM.addr}

		parseTreeNode num = node->children[0];

		astNode me = astMakeNode(num->token, num->rule, parent, NULL, NULL, num->token->lexeme, NULL, NULL);
		return me;		
	}
	else if (node->rule==63)
	{
		// <constrainedVars> ===> RNUM
		// {<constrinedVars>.carry = RNUM.addr}

		parseTreeNode rnum = node->children[0];

		astNode me = astMakeNode(rnum->token, rnum->rule, parent, NULL, NULL, rnum->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==64)
	{
		// <var> ===> ID <matrixElementExtension>
		// {ID.link = <matrixElementExtension>.carry}, {<var>.carry = ID}, {free(<matrixElementExtension>)}

		parseTreeNode id = node->children[0];
		parseTreeNode mee = node->children[1];

		astNode me = astMakeNode(id->token, id->rule, parent, NULL, NULL, id->token->lexeme, NULL, NULL);
		me->link = buildAstNode(mee, parent);

	}
	else if (node->rule==65)
	{
		// <var> ===> NUM
		// {<var>.carry = NUM.addr}

		parseTreeNode num = node->children[0];

		astNode me = astMakeNode(num->token, num->rule, parent, NULL, NULL, num->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==66)
	{
		// <var> ===> RNUM
		// {<var>.carry = RNUM.addr}

		parseTreeNode rnum = node->children[0];

		astNode me = astMakeNode(rnum->token, rnum->rule, parent, NULL, NULL, rnum->token->lexeme, NULL, NULL);
		return me;		
	}
	else if (node->rule==67)
	{
		// <var> ===> STR
		// {<var>.carry = STR.addr}

		parseTreeNode strnode = node->children[0];

		astNode me = astMakeNode(strnode->token, strnode->rule, parent, NULL, NULL, strnode->token->lexeme, NULL, NULL);
		return me;		
	}
	else if (node->rule==68)
	{
		// <var> ===> <matrix>
		// {<var>.carry = <matrix>.addr}

		return buildAstNode(node->children[0], parent);
	}
	else if (node->rule==69)
	{
		// <matrix> ===> SQO <rows> SQC
		// {free(SQO)}, {free(SQC)}, {<matrix> = newnode("MATRIX", <rows>.carry)}, {free(<rows>)}

		parseTreeNode rows = node->children[1];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "MATRIX", NULL, NULL);
		me->children = buildAstNode(rows, me);
		return me;
	}
	else if (node->rule==70)
	{
		// <rows> ===> <row> <other5>
		// {<row>.link = <other5>.carry}, {<rows>.carry = <row>.addr}, {free(<other5>)}
		
		parseTreeNode row = node->children[0];
		parseTreeNode other5 = node->children[1];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "ROWS", NULL, NULL);

		me->children = buildAstNode(row, me);
		me->children->link = buildAstNode(other5, me);
		return me;
	}
	else if (node->rule==71)
	{
		// <other5> ===> SEMICOLON <row> <other5>
		// {<row>.link = <other5>_2.carry}, {free(<other5>_2)}, {free(SEMICOLON)}, {<other5>_1.carry = <row>.addr}
		
		parseTreeNode row = node->children[1];
		parseTreeNode other5 = node->children[2];

		astNode me = buildAstNode(row, parent);
		me->link = buildAstNode(other5, me);
		return me;
	}
	else if (node->rule==72)
	{
		// <other5> ===> _epsilon_
		// {<other5>.carry = NULL}, {free(_epsilon_)}
		
		return NULL;	
	}
	else if (node->rule==73)
	{
		// <row> ===> NUM <remainingColElements>
		// {NUM.link = <remainingColElements>.carry}, {free(<remainingColElements>)}
		
		parseTreeNode num = node->children[0];
		parseTreeNode remcolelem = node->children[1];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "ROW", NULL, NULL);
		me->children = astMakeNode(num->token, num->rule, parent, NULL, NULL, num->token->lexeme, NULL, NULL);
		me->children->link = buildAstNode(remcolelem, me);
		return me;
	}
	else if (node->rule==74)
	{
		// <remainingColElements> ===> COMMA NUM <remainingColElements>
		// {free(COMMA)}, {<remainingColElements>_1.carry = NUM.addr}, {NUM.link = <remainingColElements>_2.carry}, {free(<remainingColElements>_2)}
		
		parseTreeNode num = node->children[1];
		parseTreeNode remcolelem = node->children[2];

		astNode me = astMakeNode(node->token, node->rule, parent, NULL, NULL, "ROW", NULL, NULL);
		me->children = astMakeNode(num->token, num->rule, parent, NULL, NULL, num->token->lexeme, NULL, NULL);
		me->children->link = buildAstNode(remcolelem, me);
		return me;
	}
	else if (node->rule==75)
	{

		// <remainingColElements> ===> _epsilon_
		// {<remainingColElements>.carry = NULL}, {free(_epsilon_)}

		return NULL;
	}
	else if (node->rule==76)
	{
		// <matrixElementExtension> ===> SQO NUM COMMA NUM SQC
		// {<matrixElemenetExtension>.carry = newnode("indices", NUM_1.addr, NUM_2.addr)}

		parseTreeNode num1 = node->children[1];
		parseTreeNode num2 = node->children[3];

		astNode me = astMakeNode(num1->token, node->rule, parent, NULL, NULL, "indices", NULL, NULL);
		me->children = astMakeNode(num1->token, node->rule, parent, NULL, NULL, num1->token->lexeme, NULL, NULL);
		me->children->link = astMakeNode(num2->token, node->rule, parent, NULL, NULL, num2->token->lexeme, NULL, NULL);
		return me;
	}
	else if (node->rule==77)
	{
		// <matrixElementExtension> ===> _epsilon_
		// {<matrixElementExtension>.carry = NULL}, {free(_epsilon_)}

		astNode me = NULL;
		return me;
	}
	else if (node->rule==78)
	{
		// <logicalOp> ===> AND
		// {<logicalOp>.carry = AND.addr}

		parseTreeNode ne = node->children[0];
		astNode me = astMakeNode(ne->token, node->rule, parent, NULL, NULL, "&&", NULL, NULL);
		return me;
	}
	else if (node->rule==79)
	{
		// <logicalOp> ===> OR
		// {<logicalOp>.carry = OR.addr}

		parseTreeNode or = node->children[0];
		astNode me = astMakeNode(or->token, node->rule, parent, NULL, NULL, "||", NULL, NULL);
		return me;
	}
	else if (node->rule==80)
	{
		// <relationalOp> ===> LT
		// {<relationalOp>.carry = LT.addr}

		parseTreeNode lt = node->children[0];
		astNode me = astMakeNode(lt->token, node->rule, parent, NULL, NULL, ">=", NULL, NULL);
		return me;
	}
	else if (node->rule==81)
	{
		// <relationalOp> ===> LE
		// {<relationalOp>.carry = LE.addr}

		parseTreeNode le = node->children[0];
		astNode me = astMakeNode(le->token, node->rule, parent, NULL, NULL, "<=", NULL, NULL);
		return me;
	}
	else if (node->rule==82)
	{
		// <relationalOp> ===> EQ
		// {<relationalOp>.carry = EQ.addr}

		parseTreeNode eq = node->children[0];
		astNode me = astMakeNode(eq->token, node->rule, parent, NULL, NULL, "==", NULL, NULL);
		return me;
	}
	else if (node->rule==83)
	{
		// <relationalOp> ===> GT
		// {<relationalOp>.carry = GT.addr}	

		parseTreeNode gt = node->children[0];
		astNode me = astMakeNode(gt->token, node->rule, parent, NULL, NULL, ">", NULL, NULL);
		return me;
	}
	else if (node->rule==84)
	{
		// <relationalOp> ===> GE
		// {<relationalOp>.carry = GE.addr}

		parseTreeNode ge = node->children[0];
		astNode me = astMakeNode(ge->token, node->rule, parent, NULL, NULL, ">=", NULL, NULL);
		return me;
	}
	else if (node->rule==85)
	{
		// <relationalOp> ===> NE
		// {<relationalOp>.carry = NE.addr}

		parseTreeNode ne = node->children[0];
		astNode me = astMakeNode(ne->token, node->rule, parent, NULL, NULL, "!=", NULL, NULL);
		return me;
	}
			

}