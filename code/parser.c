#include "parserDef.h"

#define MAX_BUF_SIZE 256

char * tokenTable2[ENUM_SEP_NONTERMINALS+1] = {
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
"rows", "other5", "row", "remainingColElements", "matrixElement", 
"logicalOp", "relationalOp", "ENUM_SEP_NONTERMINALS"
};

// SET OPERATIONS

dt_set setInit(int size)
{
	// printf("setinit: size=%d\n", size);
	dt_set s = (dt_set) malloc(sizeof(struct __SET));
	s->elements = (dt_str) malloc(sizeof(char)*(size+1));
	s->size = size;
	// printf("setinit: retsize=%lu\n", strlen(s->elements));

	memset(s->elements, '0', size);
	s->elements[size-1] = 0;
	return s;
}

void setKill(dt_set s)
{
	free(s->elements);
	free(s);
}

void setAdd(dt_set s, int idx)
{
	if((idx<0) || (idx > (s->size-1)))
	{
		printf("ERROR::parser.c::setAdd: idx out of bounds (size=%d, idx=%d)\n", s->size, idx);
	}
	else
	{
		s->elements[idx] = '1';	
	}
}

void setRemove(dt_set s, int idx)
{
	if((idx<0) || (idx > (s->size-1)))
	{
		printf("ERROR::parser.c::setRemove: idx out of bounds (size=%d, idx=%d)\n", s->size, idx);
	}
	else
	{
		s->elements[idx] = '0';	
	}
}

dt_set setUnion(dt_set s1, dt_set s2)
{
	if (s1->size != s2->size)
	{
		printf("ERROR::parser.c::setUnion: set s1 and s2 have different universe of discourse (size s1=%d, size s2=%d)\n", s1->size, s2->size);
	}
	else
	{
		dt_set s = setInit(s1->size);
		int i=0;
		for(i=0; i<s1->size; i++)
		{
			if((s1->elements[i]=='1') || (s2->elements[i]=='1'))
				s->elements[i] = '1';
			else
				s->elements[i] = '0';
		}
	}
}

dt_set setIntersection(dt_set s1, dt_set s2)
{
	if (s1->size != s2->size)
	{
		printf("ERROR::parser.c::setIntersection: set s1 and s2 have different universe of discourse (size s1=%d, size s2=%d)\n", s1->size, s2->size);
	}
	else
	{
		dt_set s = setInit(s1->size);
		int i=0;
		for(i=0; i<s1->size; i++)
		{
			if((s1->elements[i]=='1') && (s2->elements[i]=='1'))
				s->elements[i] = '1';
			else
				s->elements[i] = '0';
		}
	}
}

dt_set setDifference(dt_set s1, dt_set s2)
{
	if (s1->size != s2->size)
	{
		printf("ERROR::parser.c::setDifference: dt_set s1 and s2 have different universe of discourse (size s1=%d, size s2=%d)\n", s1->size, s2->size);
	}
	else
	{
		dt_set s = setInit(s1->size);
		int i=0;
		for(i=0; i<s1->size; i++)
		{
			if((s1->elements[i]=='1') && (s2->elements[i]=='0'))
				s->elements[i] = '1';
			else
				s->elements[i] = '0';
		}
	}
}

// PARSER CODE

// symbol makeSymbol(dt_str lexeme, dt_id tokID, int isTerminal)
// {
// 	symbol s = (symbol) malloc(sizeof(struct __SYMBOL));
// 	s->lexeme = strmake(lexeme);
// 	s->tokID = tokID;
// 	s->isTerminal = isTerminal;
// 	return s;
// }

gr_rhs grRHSMakeNode(dt_id sym)
{
	gr_rhs rhs = (gr_rhs) malloc(sizeof(struct __GR_RHS));
	rhs->sym = sym;
	rhs->next = NULL;
	rhs->prev = NULL;
	return rhs;
}

void grRHSFreeNode(gr_rhs head)
{
	if(head->next!=NULL)
	{
		grRHSFreeNode(head->next);
	}
	
	free(head);
}

int grRHSSize(gr_rhs head)
{
	gr_rhs mov = head;
	int size = 0;
	while(mov!=NULL)
	{
		size++;
		mov = mov->next;
	}
	return size;
}

gr_lhs grLHSMakeNode(dt_id sym)
{
	gr_lhs lhs = (gr_lhs) malloc(sizeof(struct __GR_LHS));
	lhs->sym = sym;
	lhs->head = NULL;
	lhs->tail = NULL;
	lhs->size = 0;
	return lhs;
}

void grLHSAppendRHS(gr_lhs lhs, gr_rhs node) // assuming node is a newly malloc'd object
{
	if (lhs->size==0)
	{
		lhs->head = lhs->tail = node;
		lhs->size = 1;
	}
	else
	{
		lhs->tail->next = node;
		node->prev = lhs->tail;
		lhs->size++;
		lhs->tail = node;
	}
}

gr_lhs * grLHSInitArray(int size)
{
	gr_lhs * lhsArray = (gr_lhs*) malloc(sizeof(gr_lhs)*size);
	return lhsArray;
}

grammar grInitGrammar(int lhsArraySize, int numNonTerminals, int numTerminals)
{
	grammar g = (grammar) malloc(sizeof(struct __GRAMMAR));
	g->lhsArray = grLHSInitArray(lhsArraySize);

	// printf("running\n");

	int i;
	for (i=0; i<lhsArraySize; i++)
	{	
		// printf("%d\n", i);
		// printf("yes\n");
		g->lhsArray[i] = grLHSMakeNode(TK_EXIT);
		// printf("yes2\n");
	}

	g->numNonTerminals = numNonTerminals;
	g->numTerminals = numTerminals;
	g->size = lhsArraySize;
	// printf("pass\n");
	return g;
}

grammar loadGrammar(FILE * grammarFile)
{
	dt_str buffer[MAX_BUF_SIZE];

	int N;
	int numNonTerminals;
	int numTerminals;

	fscanf(grammarFile, "%d", &N);
	// printf("%d", N);
	fscanf(grammarFile, "%d", &numNonTerminals);
	// printf("%d", N);
	fscanf(grammarFile, "%d", &numTerminals);
	// printf("%d", N);

	grammar gr = grInitGrammar(N, numNonTerminals, numTerminals);

	// printf("loadgrammarpass\n");

	int line = 0;
	int ruleSize;
	int i;

	int lhsID;
	int rhsID;

	for(line=0; line<N; line++)
	{
		fscanf(grammarFile, "%d", &ruleSize);
		fscanf(grammarFile, "%d", &lhsID);

		gr->lhsArray[line]->sym = lhsID;

		for(i=1; i<=ruleSize-1; i++)
		{
			fscanf(grammarFile, "%d", &rhsID);
			grLHSAppendRHS(gr->lhsArray[line], grRHSMakeNode(rhsID));
		}
		gr->lhsArray[line]->size = ruleSize-1;
	}

	fclose(grammarFile);

	return gr;
}

void printGrammar(grammar gr)
{
	int rule;

	printf("GRAMMAR RULES\n");

	for(rule = 0; rule < gr->size; rule++)
	{
		printf("%d", gr->lhsArray[rule]->sym);
		printf(" ===> ");

		gr_rhs mov = gr->lhsArray[rule]->head;

		while(mov!=NULL)
		{
			printf("%d ", mov->sym);
			mov = mov->next;
		}

		printf("\n");
	}
}

void printRule(grammar gr, int rule)
{
	printf("%s", tokenTable2[gr->lhsArray[rule]->sym]);
	printf(" ===> ");

	gr_rhs mov = gr->lhsArray[rule]->head;

	while(mov!=NULL)
	{
		printf("%s ", tokenTable2[mov->sym]);
		mov = mov->next;
	}
	printf("\n");
}

firstAndFollow computeFirstAndFollowSets(FILE * firstNT, FILE * firstRules, FILE * followNT)
{
	// Implemented as a python script. 
	// First and Follow sets are calculated and kept in firstNT.txt, firstRules.txt, followNT.txt
	
	int i; int k; int nt; int t; int elem;
	int numNonTerminals;
	int numRules;

	firstAndFollow ffSets = (firstAndFollow) malloc(sizeof(struct __FIRST_AND_FOLLOW));

	// load first sets of each non terminal
	// ------------------------------------
	fscanf(firstNT, "%d", &numNonTerminals);
	ffSets->firstNT = (dt_set*)malloc(sizeof(dt_set) * numNonTerminals);

	for(i=0; i<numNonTerminals; i++)
	{
		ffSets->firstNT[i] = setInit(countLexicalUnits);
	}

	for(i=0; i<numNonTerminals; i++)
	{
		fscanf(firstNT, "%d", &nt);
		// printf("-----NON TERMINAL : %d\n", nt);
		dt_set ptr = ffSets->firstNT[nt-ntBase]; //ntBase defined in lexerDef.h
		fscanf(firstNT, "%d", &t);

		for(k=0; k<t; k++)
		{
			fscanf(firstNT, "%d", &elem);
			setAdd(ptr, elem);
		}
	}
	fclose(firstNT);

	// load first sets of each rule
	// ----------------------------
	fscanf(firstRules, "%d", &numRules);
	ffSets->firstRules = (dt_set*)malloc(sizeof(dt_set) * numRules);

	for(i=0; i<numRules; i++)
	{
		ffSets->firstRules[i] = setInit(countLexicalUnits);
	}

	for(i=0; i<numRules; i++)
	{
		fscanf(firstRules, "%d", &nt);
		dt_set ptr = ffSets->firstRules[i];
		fscanf(firstRules, "%d", &t);

		for(k=0; k<t; k++)
		{
			fscanf(firstRules, "%d", &elem);
			setAdd(ptr, elem);
		}
	}
	fclose(firstRules);


	// load follow sets of each non terminal
	// -------------------------------------
	fscanf(followNT, "%d", &numNonTerminals);
	ffSets->followNT = (dt_set*)malloc(sizeof(dt_set) * numNonTerminals);

	for(i=0; i<numNonTerminals; i++)
	{
		ffSets->followNT[i] = setInit(countLexicalUnits);
		// printf("built: nt: %d , len: %lu \n", i, strlen(ffSets->followNT[i]->elements));
	}

	for(i=0; i<numNonTerminals; i++)
	{
		fscanf(followNT, "%d", &nt);
		dt_set ptr = ffSets->followNT[nt-ntBase]; //ntBase defined in lexerDef.h
		fscanf(followNT, "%d", &t);

		for(k=0; k<t; k++)
		{
			fscanf(followNT, "%d", &elem);
			setAdd(ptr, elem);
		}
	}
	fclose(followNT);

	return ffSets;
}

void printFirstFollowSets(firstAndFollow ffSets, grammar gr)
{
	int i;

	// first of non terminals
	printf("\nFIRST - NON TERMINALS\n");
	printf("%-10s   %-22s\n", "NT_no", "first set");
	for(i=0; i<gr->numNonTerminals; i++)
	{
		printf("  %-10d %-20s\n", i+ntBase, ffSets->firstNT[i]->elements);
	}

	// load first sets of each rule
	printf("\nFIRST - RULES\n");
	printf("%-10s   %-22s\n", "rule_no", "first set");
	for(i=0; i<gr->size; i++)
	{
		printf("  %-10d %-20s\n", i+1, ffSets->firstRules[i]->elements);
	}

	// load follow sets of each non terminal
	printf("\nFOLLOW - NON TERMINALS\n");
	printf("%-10s   %-22s\n", "NT_no", "followNT set");
	for(i=0; i<gr->numNonTerminals; i++)
	{
		printf("  %-10d %-20s\n", i+ntBase, ffSets->followNT[i]->elements);
	}
}

parseTable ptInitEmptyTable(int rows, int cols)
{
	parseTable pt;

	pt = (parseTable) malloc(sizeof(int *) * rows);

	int i=0; int j=0;
	for(i=0; i<rows; i++)
	{
		pt[i] = (int *) malloc(sizeof(int) * cols);
	}

	for(i=0; i<rows; i++)
	{
		for(j=0; j<rows; j++)
		{
			pt[i][j] = -1;
		}
	}

	return pt;
}

void createParseTable(firstAndFollow F, grammar gr, parseTable T) // redefine the function using book definition
{
	// assuming table is already constructed with dimensions
	// number_of_nonterminals x number_of_terminals

	int nt; int t; int rule;

	// for (nt=0; nt < gr->numNonTerminals; nt++)
	// {
	// 	for (t=0; t < gr->numTerminals; t++)
	// 	{
	// 		for (rule=0; rule < gr->size; rule++)
	// 		{
	// 			if ((gr->lhsArray[rule]->sym == nt))
	// 			{
	// 				if((F->firstRules[rule]->elements[t] == '1'))
	// 				{
	// 					if (T[nt][t]!=-1)
	// 					{
	// 						printf("ERROR::parser.c::createParseTable: Case1: parse table already has a rule\n");
	// 					}
	// 					else
	// 					{
	// 						T[nt][t] = rule;
	// 					}
	// 				}
	// 				else if((F->followNT[nt]->elements[t] == '1'))
	// 				{
	// 					if (T[nt][t]!=-1)
	// 					{
	// 						printf("ERROR::parser.c::createParseTable: Case2: parse table already has a rule\n");
	// 					}
	// 					else
	// 					{
	// 						T[nt][t] = rule;
	// 					}	
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	dt_str firstRuleSet, followNTSet;

	for(rule=0; rule<gr->size; rule++) // for each production rule A -> alpha do the following
	{
		nt = gr->lhsArray[rule]->sym;
		nt = nt - ntBase;

		firstRuleSet = F->firstRules[rule]->elements;
		followNTSet = F->followNT[nt]->elements;

		// 1. for each terminal a in first(alpha), add A->alpha rule to T[A,a]
		for(t=0; t<gr->numTerminals; t++)
		{
			if(firstRuleSet[t]=='1')
			{
				if(T[nt][t]!=-1)
				{
					printf("\n");
					printf("ERROR::parser.c::createParseTable:1: T[nt][t] is not -1\n");
					printf("nt=%d (%s), t=%d (%s), rule=%d\n", nt, tokenTable2[nt+ntBase], t, tokenTable2[t+tBase],rule);
					printRule(gr, rule);
				}
				else
				{
					T[nt][t] = rule;
				}
			}
		}

		// 2. if eps in first(alpha) then for each terminal b in follow(A), add A -> alpha to T[A,b].
		// if eps in first(alpha) && $ in follow(A), add A -> alpha to T[A,$] as well.
		
		if(firstRuleSet[TK_epsilon]=='1') // if eps in first(alpha)
		{
			for(t=0; t<gr->numTerminals; t++)
			{
				if(followNTSet[t]=='1') // then for each terminal b in follow(A)
				{
					// add A -> alpha to T[A,b]
					if(T[nt][t]!=-1)
					{
						printf("\n");
						printf("ERROR::parser.c::createParseTable:2: T[nt][t] is not -1\n");
						printf("nt=%d (%s), t=%d (%s), rule=%d\n", nt, tokenTable2[nt+ntBase], t, tokenTable2[t+tBase],rule);
						printRule(gr, rule);
					}
					else
					{
						T[nt][t] = rule;
					}
				}
			}
		}
	}
}

void printParseTable(parseTable T, grammar gr)
{
	int nt, t;

	printf("\nPARSE TABLE\n");

	printf("\n%2s | ", "NT");
	
	for(t=0; t < gr->numTerminals; t++)
	{
		printf("%4d", t+tBase);
	}

	printf("\n-----");
	
	for(t=0; t < gr->numTerminals; t++)
	{
		printf("----");
	}
	
	for(nt=0; nt < gr->numNonTerminals; nt++)
	{
		printf("\n%2d | ", nt+ntBase);
		for(t=0; t < gr->numTerminals; t++)
		{
			printf("%4d", T[nt][t]);
		}
	}
	printf("\n");
}

parseTree parseInputSourceCode(dt_str testCaseFileName, parseTable T)
{
	FILE * inputFile = fopen(testCaseFileName, "r");

	int shouldRun = 1;

	dt_token tok;
	dt_str buf = (dt_str) malloc(sizeof(char) * BUFFER_SIZE);
	memset(buf,'\0',BUFFER_SIZE);

	dt_token tk = NULL;

	while(!feof(inputFile) && shouldRun)
	{
		while(buf!='\0'){
			tk = (dt_token) getNextToken(inputFile, &buf, &begin, BUFFER_SIZE);
			if(tk!=NULL)
			{
				if(tk->tokenID==TK_EXIT || tk->tokenID==TK_ABRUPTEND)
				{
					break;
				}
				printToken(tk);
			}
			else
			{
				shouldRun=0;
			}
		}

	}
}