#include "lexerDef.h"

dt_flag shouldRun = True;

char input;
int verbose = 0;

char * tokenTable[ENUM_SEP_NONTERMINALS+1] = {
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
"rows", "other5", "row", "other6", "remainingColElements", "matrixElement", 
"logicalOp", "relationalOp", "ENUM_SEP_NONTERMINALS"
};

// read input file into buffer
char buffer[BUFFER_SIZE];

// define indices and global variables
int begin=0, forward=0;
int line_number = 1;

/*
	======================
	== STRING FUNCTIONS ==
	======================
*/

dt_str strmake(const char * src)
{
	int sl = strlen(src);
	dt_str dst = (dt_str) malloc(sl+1);
	
	if (dst==NULL)
	{
		printf("ERROR::helpers.h::strmake(): couldn't malloc dst");
		return NULL;
	}
	else
	{

		int i=0;
		for(i=0; i<sl; i++)
		{
			dst[i] = src[i];
		}
		dst[i+1] = '\0';

		return dst;
	}
}

dt_str strslice(dt_str s, int beg, int end) // s[beg:end] inclusive
{
	dt_str slice = (dt_str) malloc(sizeof(char)*(end-beg+1));
	
	int i=0;
	for(i=beg; i<end; i++)
	{
		slice[i] = s[i];
	}

	return slice;
}

/*
	===========================
	== LINKED LIST FUNCTIONS ==
	===========================
*/

dt_linkedListNode llMakeNode(void * data, int size)
{
	dt_linkedListNode node = (dt_linkedListNode) malloc(sizeof(struct __LINKED_LIST_NODE));
	
	if(node==NULL)
	{
		printf("ERROR::helpers.h::llMakeNode: could not malloc node");
	}
	else
	{
		node->data = data;
		node->size = size;
		node->prev = NULL;
		node->next = NULL;
	}

	return node;
}

void llFreeNode(dt_linkedListNode node)
{
	free(node->data);
	free(node);
}

dt_linkedList llInit()
{
	dt_linkedList l = (dt_linkedList) malloc(sizeof(struct __LINKED_LIST));
	l->front = NULL;
	l->back = NULL;
	l->count = 0;
	return l;
}

void llPushBack(dt_linkedList ll, void * data, int size)
{
	if(ll->front==NULL && ll->back==NULL)
	{
		ll->front = ll->back = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode  node = llMakeNode(data, size);
		node->prev = ll->back;
		ll->back->next = node;
		ll->back = node;
		ll->count++;
	}
}

void llPushFront(dt_linkedList ll, void * data, int size)
{
	if(ll->front==NULL && ll->back==NULL)
	{
		ll->front = ll->back = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode  node = llMakeNode(data, size);
		node->next = ll->front;
		ll->front->prev = node;
		ll->front = node;
		ll->count++;
	}
}

void llPopFront(dt_linkedList ll)
{
	if(ll->count<=0)
	{
		printf("ERROR::helpers.h::llPopFront: list is empty");
	}
	else if(ll->front==ll->back)
	{
		dt_linkedListNode node = llMakeNode(ll->front->data, ll->front->size);
		dt_linkedListNode topNode = ll->front;
		ll->front = ll->back = NULL;
		ll->count = 0;
		llFreeNode(topNode);
	}
	else
	{
		dt_linkedListNode node = llMakeNode(ll->front->data, ll->front->size);
		dt_linkedListNode topNode = ll->front;
		ll->front = topNode->next;
		ll->front->prev = NULL;
		ll->count--;
		llFreeNode(topNode);
	}
}

void llPopBack(dt_linkedList ll)
{
	if(ll->count<=0)
	{
		printf("ERROR::helpers.h::llPopBack: list is empty");
	}
	else if(ll->front==ll->back)
	{
		dt_linkedListNode node = llMakeNode(ll->back->data, ll->back->size);
		dt_linkedListNode endNode = ll->back;
		ll->back = ll->front = NULL;
		ll->count = 0;
		llFreeNode(endNode);
	}
	else
	{
		dt_linkedListNode node = llMakeNode(ll->back->data, ll->back->size);
		dt_linkedListNode endNode = ll->back;
		ll->back = endNode->prev;
		ll->back->next = NULL;
		ll->count--;
		llFreeNode(endNode);
	}
}


/*
	=====================
	== TOKEN FUNCTIONS ==
	=====================
*/

dt_token makeToken(dt_str lexeme, dt_id tokenID, int lineNo)
{
	dt_token t = (dt_token) malloc(sizeof(struct __TOKEN));

	if (t==NULL)
	{
		printf("ERROR::constructs.h::make_token(): couldn't malloc token");
		return NULL;
	}
	else
	{
		t->tokenID = tokenID;
		t->lexeme = strmake(lexeme);
		t->lineNo = lineNo;
		t->value = NULL;
	}

	return t;
}

void printToken(dt_token token)
{ printf("%-10d %-20s %-20s\n", token->lineNo, tokenTable[token->tokenID], token->lexeme); }

dt_NUM getNUM(dt_str lexeme)
{
	int l = strlen(lexeme);
	int i = l-1;

	dt_NUM val = 0;

	for(i=0; i<=l-1; i++)
	{
		val *= 10;
		val += lexeme[i]-'0';
	}

	return val;
}

dt_NUM getRNUM(dt_str lexeme)
{
	int dec_pos = 0;
	int l = strlen(lexeme);

	dt_RNUM val = 0;

	while(lexeme[dec_pos]!='.')
	{
		dec_pos++;
	}

	val = getNUM(strslice(lexeme, 0, dec_pos-1));
	dt_str decimal = strslice(lexeme, dec_pos+1, l-1);
	
	dt_RNUM decival = getNUM(decimal);
	
	int i=1;

	for(i=1; i<=strlen(decimal); i++)
	{
		decival /= 10;
	}

	val += decival;

	return val;
}

long long int findHash (dt_str lexeme, int x, int m)
{
	// calculates hash using homer's
	// hash = ( a[0]*1 + a[1]*x + a[2]*(x**2) .. + a[n]*(x**n) ) mod m
	// hash = ((((a[n]*x + a[n-1])*x + a[n-1])*x .. ) + a[0]) mod m
	
	int max = strlen(lexeme);
	int cur = max-1;

	long long int hashValue = lexeme[cur];

	for(cur=max-1; cur>0; cur--)
	{
		hashValue = ( hashValue * x + lexeme[cur-1] ) % m;
	}

	return hashValue;
}

dt_id getTokID(dt_str lexeme)
{
	int hashValue = findHash(lexeme, HASH_X, HASH_M);
	
	switch(hashValue)
	{
		case 11:
			if(!strcmp(lexeme, "end"))
				return TK_END;
			else
				return TK_ID;
			break;

		case 9:
			if(!strcmp(lexeme, "int"))
				return TK_INT;
			else
				return TK_ID;
			break;

		case 6:
			if(!strcmp(lexeme, "real"))
				return TK_REAL;
			else
				return TK_ID;
			break;

		case 13:
			if(!strcmp(lexeme, "string"))
				return TK_STRING;
			else
				return TK_ID;
			break;

		case 7:
			if(!strcmp(lexeme, "matrix"))
				return TK_MATRIX;
			else
				return TK_ID;
			break;

		case 21:
			if(!strcmp(lexeme, "if"))
				return TK_IF;
			else
				return TK_ID;
			break;

		case 17:
			if(!strcmp(lexeme, "else"))
				return TK_ELSE;
			else
				return TK_ID;
			break;

		case 8:
			if(!strcmp(lexeme, "endif"))
				return TK_ENDIF;
			else
				return TK_ID;
			break;

		case 0:
			if(!strcmp(lexeme, "read"))
				return TK_READ;
			else
				return TK_ID;
			break;

		case 25:
			if(!strcmp(lexeme, "print"))
				return TK_PRINT;
			else
				return TK_ID;
			break;

		default:
			return TK_ID;
	}
}


/*
	================
	== LEXER CODE ==
	================

	LEXER MODULE FUNCTIONS
		-removeComments
		-getNextToken
*/

void removeComments(dt_str testCaseFileName) // will be printed only on the console and not in the file
{
	FILE * testCaseFile = fopen(testCaseFileName, "r");

	if(testCaseFile==NULL)
	{
		printf("ERROR::lexer.c::removeComments: Couldn\'t open code file %s", testCaseFileName);
	}
	else
	{
		printf("~~~~ Source Code Without Comments ~~~~\n");
		dt_flag readingComment = False;

		char c;

		while(!feof(testCaseFile))
		{
			fscanf(testCaseFile, "%c", &c);

			if(feof(testCaseFile))
			{
				break;
			}

			if(readingComment==False)
			{
				if(c=='#')
				{
					readingComment = True;
				}
				else
				{
					printf("%c", c);
				}
			}
			else
			{
				if(c=='\n')
				{
					readingComment=False;
				}
			}
		}

		fclose(testCaseFile);
		printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
}

FILE * getStream(FILE * f, dt_str * buffer, int bufSize)
{
	memset(*buffer, 0, bufSize);
	fread(*buffer,bufSize-1,1,f);
	return f;
}

dt_token getNextToken(FILE * inputFile, dt_str * buffer, int * begin, int bufSize)
{
	dt_str lexeme = (dt_str)malloc(MAX_LEXEME_SIZE * sizeof(char));
	memset(lexeme, 0, MAX_LEXEME_SIZE);
	
	int fwd = 0; // to move in the lexeme
	int s = 1; // dfa state
	int lastState = s;

	// BASIC LOOP of the LEXER
	while(shouldRun)
	{
		if(*(*buffer)=='\0') // if buffer is empty
		{
			// fill the buffer
			getStream(inputFile, buffer, bufSize);
			// fread(*buffer,bufSize,1,inputFile);
		}

		// read input
		input = *(*buffer);

		if(input<=0 && fwd>0)
		{
			input=EOFchar;
		}

		if ((int)input<=0 && fwd==0) // if no more input
		{
			// stop running
			shouldRun = 0;
			return (dt_token) makeToken(lexeme, TK_EXIT, line_number);
		}
		else if((int)input<=0 && fwd!=0)
		{
			shouldRun = 0;
			printf("LEXICAL_ERROR: ABRUPT END OF INPUT at state=%d, lexeme=\'%s\'\n", s, lexeme);
			return (dt_token) makeToken(lexeme, TK_ABRUPTEND, line_number);
		}
		else
		{
			switch(s) // check the state
			{
				case 1: // NONFINAL
				{
					lastState = s;
					if (input==' ' || input=='\t' || input=='\r')
					{	
						s=1;
					}
					else if (input=='\n')
					{
						s=1;
						line_number++;
					}
					else if(input=='#'){
						s=2;
					}
					else if(input=='['){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_SQO, line_number);
					}
					else if(input==']'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_SQC, line_number);
					}
					else if(input=='('){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_OP, line_number);
					}
					else if(input==')'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_CL, line_number);
					}
					else if(input==';'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_SEMICOLON, line_number);
					}
					else if(input==','){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_COMMA, line_number);
					}
					else if(input=='+'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_PLUS, line_number);
					}
					else if(input=='-'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_MINUS, line_number);
					}
					else if(input=='*'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_MUL, line_number);
					}
					else if(input=='/'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_DIV, line_number);
					}
					else if(input=='@'){
						(*buffer)++;
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_SIZE, line_number);
					}
					else if(input=='<'){
						lexeme[fwd++] = input;
						s=15;
					}
					else if(input=='>'){
						lexeme[fwd++] = input;
						s=17;
					}
					else if(input=='='){
						lexeme[fwd++] = input;
						s=19;
					}
					else if(input=='\"'){
						lexeme[fwd++] = input;
						s=23;
					}
					else if(input>='0' && input<='9'){
						lexeme[fwd++] = input;
						s=26;
					}
					else if(input=='.'){
						lexeme[fwd++] = input;
						s=30;
					}
					else if(input=='_'){
						lexeme[fwd++] = input;
						s=42;
					}
					else if((input>='a' && input<='z') || (input>='A' && input<='Z')){
						s=44;
						lexeme[fwd++] = input;	
					}
					else if(input==EOFchar)
					{
						return (dt_token) makeToken(lexeme, TK_EXIT, line_number);
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;

				case 2: // NONFINAL
				{
					if(input=='\n')
					{
						s = 3;
						line_number++;
					}
					else
					{
						s=2;
					}
					lastState = s;
				}
				break;

				case 3: // FINAL
				{
					// transit back to state 1
					s = 1;
					lexeme = (char*)realloc(lexeme, MAX_LEXEME_SIZE * sizeof(char));
					// comment is discarded automataically
					lastState = s;
				}
				break;

				case 15: // FINAL
				{
					lastState = s;
					if(input=='=')
					{
						lexeme[fwd++] = input;
						(*buffer)++;
						return (dt_token)makeToken(lexeme, TK_LE, line_number);
						// s=16;
					}
					else
					{
						s = 1;
						// (*buffer)--;
						return (dt_token)makeToken(lexeme, TK_LT, line_number);
					}
				}
				break;

				case 17: // FINAL
				{
					lastState = s;
					if(input=='=')
					{
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_GE, line_number);
						// s=18;
					}
					else
					{
						s = 1;
						// (*buffer)--;
						return (dt_token)makeToken(lexeme, TK_GT, line_number);
					}
				}
				break;

				case 19: // FINAL
				{
					lastState = s;
					if(input=='=')
					{
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_EQ, line_number);
						// s=20;
					}
					else if(input=='/')
					{
						lexeme[fwd++] = input;
						s=21;
					}
					else
					{
						s = 1;
						// (*buffer)--;
						return (dt_token)makeToken(lexeme, TK_ASSIGNOP, line_number);
					}
				}
				break;

				case 21: // NONFINAL
				{
					lastState = s;
					if(input=='=')
					{
						lexeme[fwd++] = input;
						(*buffer)++;
						return (dt_token)makeToken(lexeme, TK_NE, line_number);
						// s=22;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;

				case 23: // NONFINAL
				{
					lastState = s;
					if((input>='a' && input<='z') || input==' ' || input=='\t')
					{
						lexeme[fwd++] = input;
						s=24;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;

				case 24: // NONFINAL
				{
					lastState = s;
					if( (input>='a' && input<='z') || input==' ' || input=='\t')
					{
						lexeme[fwd++] = input;
						s=24;
					}
					else if (input=='\"')
					{
						lexeme[fwd++] = input;
						s=1;
						(*buffer)++;
						if (strlen(lexeme) > 22)
						{
							printf("LEXICAL_WARNING: String Length is more than 20 at state: %d, line: %d, lexeme: %s\n", s, line_number, lexeme);
							return (dt_token)makeToken(lexeme, TK_STR, line_number);
						}
						else
						{
							return (dt_token)makeToken(lexeme, TK_STR, line_number);
						}
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;

				case 26: // FINAL
				{
					lastState = s;
					if(input>='0' && input<='9')
					{
						lexeme[fwd++] = input;
						s=26;
					}
					else if (input=='.')
					{
						lexeme[fwd++] = input;
						s=27;
					}
					else
					{
						s = 1;

						dt_token tok = makeToken(lexeme, TK_NUM, line_number);
						tok->value = (dt_NUM*) malloc(sizeof(dt_NUM));
						*((dt_NUM*) tok->value) = getNUM(lexeme);
						return (dt_token)tok;
					}
				}
				break;

				case 27: // NONFINAL
				{
					lastState = s;
					if(input>='0' && input<='9')
					{
						lexeme[fwd++] = input;
						s=28;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;

				case 28: // NONFINAL
				{
					lastState = s;
					if(input>='0' && input<='9')
					{
						lexeme[fwd++] = input;
						(*buffer)++;
						dt_token tok = makeToken(lexeme, TK_RNUM, line_number);
						tok->value = (dt_RNUM*) malloc(sizeof(dt_RNUM));
						*((dt_RNUM*)tok->value) = getNUM(lexeme);
						return (dt_token)tok;
						// s=29;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 30: // NONFINAL
				{
					lastState = s;
					if (input=='a')
					{
						lexeme[fwd++] = input;
						s=31;
					}
					else if (input=='n')
					{
						lexeme[fwd++] = input;
						s=35;
					}
					else if (input=='o')
					{
						lexeme[fwd++] = input;
						s=39;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 31: // NONFINAL
				{
					lastState = s;
					if (input=='n')
					{
						lexeme[fwd++] = input;
						s=32;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						s=1;
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 32: // NONFINAL
				{
					lastState = s;
					if (input=='d')
					{
						lexeme[fwd++] = input;
						s=33;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 33: // NONFINAL
				{
					lastState = s;
					if (input=='.')
					{
						lexeme[fwd++] = input;
						(*buffer)++;
						return (dt_token)makeToken(lexeme, TK_AND, line_number);
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 35: // NONFINAL
				{
					lastState = s;
					if (input=='o')
					{
						lexeme[fwd++] = input;
						s=36;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 36: // NONFINAL
				{
					lastState = s;
					if (input=='t')
					{
						lexeme[fwd++] = input;
						s=37;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 37: // NONFINAL
				{
					lastState = s;
					if (input=='.')
					{
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_NOT, line_number);
						s=38;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 38: // FINAL
				{
					lastState = s;
					s = 1;
					// (*buffer)--;
					return (dt_token)makeToken(lexeme, TK_NOT, line_number);
				}
				break;
				
				case 39: // NONFINAL
				{
					lastState = s;
					if (input=='r')
					{
						lexeme[fwd++] = input;
						s=40;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 40: // NONFINAL
				{
					lastState = s;
					if (input=='.')
					{
						lexeme[fwd++] = input;
						return (dt_token)makeToken(lexeme, TK_OR, line_number);
						s=41;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 41: // FINAL
				{
					lastState = s;
					s = 1;
					return (dt_token)makeToken(lexeme, TK_OR, line_number);
				}
				break;
				
				case 42: // NONFINAL
				{
					lastState = s;
					if ((input>='a' && input<='z') || (input>='A' && input<='Z'))
					{
						lexeme[fwd++] = input;
						s=43;
					}
					else
					{
						s=1;
						printf("LEXICAL_ERROR: Unrecognized symbol \'%c\' (ascii: %d) at state: %d, line: %d, lexeme: %s\n", input, (int) input, s, line_number, lexeme);
						fwd = 0;
						memset(lexeme, 0, MAX_LEXEME_SIZE);
						(*buffer)--;
					}
				}
				break;
				
				case 43: // FINAL
				{
					lastState = s;
					if ((input>='a' && input<='z') || (input>='A' && input<='Z') || (input>='0' && input<='9'))
					{
						lexeme[fwd++] = input;
						s=43;
					}
					else
					{
						s = 1;
						// (*buffer)--;
						if(!strcmp(lexeme, "_main"))
						{
							return (dt_token)makeToken(lexeme, TK_MAIN, line_number);	
						}
						else
						{
							return (dt_token)makeToken(lexeme, TK_FUNID, line_number);
						}
					}
				}
				break;
				
				case 44: // FINAL
				{
					lastState = s;
					if (input>='0' && input<='9')
					{
						s=45;
						lexeme[fwd++] = input;
						(*buffer)++;
						if (strlen(lexeme) > 20)
						{
							printf("LEXER_WARNING: ID Length is more than 20 at state: %d, line: %d, lexeme: %s\n", s, line_number, lexeme);
							return (dt_token)makeToken(lexeme, TK_ID, line_number);
						}
						else
						{
							return (dt_token)makeToken(lexeme, TK_ID, line_number);
						}
					}
					else if ( (input>='a' && input<='z') || (input>='A' && input<='Z') )
					{
						s=44;
						lexeme[fwd++] = input;
					}
					else
					{
						s = 1;
						dt_id tok_id = getTokID(lexeme);
						return (dt_token)makeToken(lexeme, tok_id, line_number);
					}
				}
				break;
				
				case 45: // FINAL
				{
					lastState = s;
					s = 1;
					return (dt_token)makeToken(lexeme, TK_ID, line_number);
				}
			}
		}
	(*buffer)++;
	}
}