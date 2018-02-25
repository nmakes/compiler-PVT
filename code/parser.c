#include "parserDef.h"
#include "lexer.h"

#define MAX_BUF_SIZE 256

// SET OPERATIONS

dt_set setInit(int size)
{
	dt_set s = (dt_set) malloc(sizeof(struct __SET));
	s->elements = (dt_str) malloc(sizeof(char)*(size+1));
	s->size = size;

	memset(s->elements, '0', size);
	return s;
}

void setKill(set s)
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
		for(i=0; i<size; i++)
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
		for(i=0; i<size; i++)
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
		printf("ERROR::parser.c::setDifference: set s1 and s2 have different universe of discourse (size s1=%d, size s2=%d)\n", s1->size, s2->size);
	}
	else
	{
		dt_set s = setInit(s1->size);
		int i=0;
		for(i=0; i<size; i++)
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
		freeRHS(head->next);
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

gr_lhs grLHSInitArray(int size)
{
	gr_lhs lhsArray = (gr_lhs) malloc(sizeof(__GR_LHS)*size);
	
	struct __GR_LHS initLHS;
	initLHS->sym = TK_epsilon;
	initLHS->head = initLHS->tail = NULL;
	initLHS->size = 0;

	memset(lhsArray, initLHS, size);

	return lhsArray;
}

grammar grInitGrammar(int lhsArraySize, int numNonTerminals, int numTerminals)
{
	grammar g = (grammar) malloc(sizeof(struct __GRAMMAR));
	g->lhsArray = grInitLHSArray(lhsArraySize);
	g->numNonTerminals = numNonTerminals;
	g->numTerminals = numTerminals;
	g->size = lhsArraySize;
	return g;
}

grammar loadGrammar(FILE * grammarFile)
{
	dt_str buffer[MAX_BUF_SIZE];

	int N;
	int numNonTerminals;
	int numTerminals;

	fscanf(grammarFile, "%d", &N);
	fscanf(grammarFile, "%d", &numNonTerminals);
	fscanf(grammarFile, "%d", &numTerminals);

	grammar gr = grInitGrammar(N, numNonTerminals, numTerminals);

	int line = 0;
	int ruleSize;
	int i;

	dt_id lhsID;
	dt_id rhsID;

	for(line=0; line<N; line++)
	{
		fscanf(grammarFile, "%d", &ruleSize);
		fscanf(grammarFile, "%d", &lhsID);
		gr->lhsArray[line]->sym = lhsID;
		gr->lhsArray[line]->size = ruleSize-1;

		for(i=1; i<=ruleSize-1; i++)
		{
			fscanf(grammarFile, "%d", &rhsID);
			grLHSAppendRHS(gr->lhsArray[line], grRHSMakeNode(rhsID));
		}
	}

	return gr;
}

void printGrammar(grammar gr)
{
	int rule;

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

firstAndFollow ComputeFirstAndFollowSets(FILE * firstNT, FILE * firstRules, FILE * followNT)
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
	ffSets->firstNT = (set*)malloc(sizeof(set) * numNonTerminals);

	for(i=0; i<numNonTerminals; i++)
	{
		ffSets->firstNT[i] = setInit(countLexicalUnits);
	}

	for(i=0; i<numNonTerminals; i++)
	{
		fscanf(firstNT, "%d", &nt);
		set ptr = ffSets->firstNT[nt-ntBase]; //ntBase defined in lexerDef.h
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
	ffSets->firstRules = (set*)malloc(sizeof(set) * numRules);

	for(i=0; i<numRules; i++)
	{
		ffSets->firstRules[i] = setInit(countLexicalUnits);
	}

	for(i=0; i<numRules; i++)
	{
		fscanf(firstRules, "%d", &nt);
		set ptr = ffSets->firstRules[i];
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
	ffSets->followNT = (set*)malloc(sizeof(set) * numNonTerminals);

	for(i=0; i<numNonTerminals; i++)
	{
		ffSets->followNT[i] = setInit(countLexicalUnits);
	}

	for(i=0; i<numNonTerminals; i++)
	{
		fscanf(followNT, "%d", &nt);
		set ptr = ffSets->followNT[nt-ntBase]; //ntBase defined in lexerDef.h
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

void createParseTable(firstAndFollow F, grammar gr, parseTable T)
{
	// assuming table is already constructed with dimensions
	// number_of_nonterminals x number_of_terminals

	int nt; int t; int rule;

	for (nt=0; nt < gr->numNonTerminals; nt++)
	{
		for (t=0; t < gr->numTerminals; t++)
		{
			for (rule=0; rule < gr->size; rule++)
			{
				if ((gr->lhsArray[rule]->sym == nt))
				{
					if((F->firstRules[rule][t] == '1'))
					{
						if (T[nt][t]!=-1)
						{
							printf("ERROR::parser.c::createParseTable: Case1: parse table already has a rule\n");
						}
						else
						{
							T[nt][t] = rule;
						}
					}
					else if((F->followNT[nt][t] == '1'))
					{
						if (T[nt][t]!=-1)
						{
							printf("ERROR::parser.c::createParseTable: Case2: parse table already has a rule\n");
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
}

