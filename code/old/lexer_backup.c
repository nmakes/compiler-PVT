#include "lexerDef.h"

dt_flag shouldRun = True;

char input;
int verbose = 0;

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

dt_str strmake(dt_str src)
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
{ printf("%d \t %d \t %s \n", token->lineNo, token->tokenID, token->lexeme); }

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
		-readNextChunk
		-getNextChar
		-getNextToken
*/

void removeComments(FILE *testCaseFile, FILE *cleanFile)
{
	dt_flag readingComment = False;

	char c;

	while(!feof(testCaseFile))
	{
		fscanf(testCaseFile, "%c", &c);

		if(readingComment==False)
		{
			if(c=='#')
			{
				readingComment = True;
			}
			else
			{
				fprintf(cleanFile, "%c", c);
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
	fclose(cleanFile);
}

// read the next chunk of file into the buffer
void readNextChunk(FILE * f, char * buffer, int bufSize)
{
	if(verbose==1)
		printf("\nrunning lexer.c::readNextChunk...\n");
	
	int pos = 0;
	char c;

	// if the file is not empty and there is still space left in the buffer, read the next character
	while(!feof(f) && pos<bufSize-1)
	{
		fscanf(f, "%c", &c);
		if(!feof(f))
		{
			printf("%c",c);
			buffer[pos++] = c;
		}
	}

	buffer[pos] = '\0';

	if(verbose==1)
		printf("\nexiting lexer.c::readNextChunk...\n");
}

char getNextChar(FILE * F, char * buffer, int * begin, int * fwd, int bufSize)
{
	int b = *begin;
	int f = *fwd;
	if(verbose==1)
		printf("\nrunning lexer.c::getNextChar...\n");

	if ((f >= (bufSize - 1)) || (buffer[0]=='\0')) 
	{
		// either all letters have been seen or the buffer is empty
		readNextChunk(F, buffer, bufSize);
		//printf("\ngetNextChar: Reads buffer: %s\n", buffer);
		*begin = 0;
		*fwd = 0;
		char c = buffer[(*fwd)++];
		return c; 
	}
	else if(buffer[b+f]=='\0')
	{
		(*fwd)++;
		return -1;
	}
	else
	{
		int b = *begin;
		int f = *fwd;
		*fwd += 1;
		return buffer[b+f];
	}
}

dt_token getNextToken(FILE * inputFile, char * buffer, int * begin, int bufSize)
{
	//if(verbose==1)
	printf("\nrunning lexer.c::getNextToken... buffer = %s\n", buffer);

	int fwd = 0; // overshoot fwd to read the next chunk
	char lexeme[MAX_LEXEME_SIZE];
	int s = 1; // dfa state

	// BASIC LOOP of the LEXER
	while(shouldRun)
	{
		input = getNextChar(inputFile, buffer, begin, &fwd, bufSize);
		printf("loop: s=%d begin=%ud fwd=%d bufSize=%d input=%c ascii=%d line=%d\n",s,buffer,*begin,fwd,bufSize,input,(int)input,line_number);

		if ((int)input< 0) // NO MORE INPUT
		{
			printf("INPUT NULL");
			return NULL;
		}
		else
		{
			switch(s)
			{
				case 1: // NONFINAL
				{
					if (input==' ' || input=='\t' || input=='\r')
						s=1;
					else if (input=='\n')
					{
						s=1;
						line_number++;
					}
					else if(input=='#')
						s=2;
					else if(input=='[')
						s=4;
					else if(input==']')
						s=5;
					else if(input=='(')
						s=6;
					else if(input==')')
						s=7;
					else if(input==';')
						s=8;
					else if(input==',')
						s=9;
					else if(input=='+')
						s=10;
					else if(input=='-')
						s=11;
					else if(input=='*')
						s=12;
					else if(input=='/')
						s=13;
					else if(input=='@')
						s=14;
					else if(input=='<')
						s=15;
					else if(input=='>')
						s=17;
					else if(input=='=')
						s=19;
					else if(input=='\"')
						s=23;
					else if(input>='0' && input<='9')
						s=26;
					else if(input=='.')
						s=30;
					else if(input=='_')
						s=40;
					else if((input>='a' && input<='z') || (input>='A' && input<='Z'))
						s=44;
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
						*begin += (fwd - 1);
						return (dt_token) makeToken(lexeme, TK_EXIT, line_number);
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
						continue;
				}
				break;

				case 3: // FINAL
				{
					// transit back to state 1
					s = 1;
					continue;
					// comment is discarded automataically
				}
				break;

				case 4: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_SQO, line_number);
				}
				break;

				case 5: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_SQC, line_number);
				}
				break;

				case 6: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_OP, line_number);
				}
				break;

				case 7: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_CL, line_number);
				}
				break;

				case 8: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					printf("\n%s\n", lexeme);
					s = 1;
					*begin += (fwd - 1);
					printf("RETURNING ;");
					return (dt_token)makeToken(lexeme, TK_SEMICOLON, line_number);
				}
				break;

				case 9: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_COMMA, line_number);
				}
				break;

				case 10: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_PLUS, line_number);
				}
				break;

				case 11: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_MINUS, line_number);
				}
				break;

				case 12: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_MUL, line_number);
				}
				break;

				case 13: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_DIV, line_number);
				}
				break;

				case 14: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_SIZE, line_number);
				}
				break;

				case 15: // FINAL
				{
					if(input=='=')
					{
						s=16;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
						return (dt_token)makeToken(lexeme, TK_LT, line_number);
					}
				}
				break;

				case 16: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_LE, line_number);
				}
				break;

				case 17: // FINAL
				{
					if(input=='=')
					{
						s=18;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
						return (dt_token)makeToken(lexeme, TK_GT, line_number);
					}
				}
				break;

				case 18: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_GE, line_number);
				}
				break;

				case 19: // FINAL
				{
					if(input=='=')
					{
						s=20;
					}
					else if(input=='/')
					{
						s=21;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
						return (dt_token)makeToken(lexeme, TK_ASSIGNOP, line_number);
					}
				}
				break;

				case 20: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_EQ, line_number);
				}
				break;

				case 21: // FINAL
				{
					if(input=='=')
					{
						s=22;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}
				break;

				case 22: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_NE, line_number);
				}
				break;

				case 23: // FINAL
				{
					if(input>='a' && input<='z')
					{
						s=24;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}
				break;

				case 24: // FINAL
				{
					if(input>='a' && input<='z')
					{
						s=24;
					}
					else if (input=='\"')
					{
						s=25;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}
				break;

				case 25: // FINAL
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_STR, line_number);
				}
				break;

				case 26: // FINAL
				{
					if(input>='0' && input<='9')
					{
						s=26;
					}
					else if (input=='.')
					{
						s=27;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
						dt_token tok = makeToken(lexeme, TK_NUM, line_number);
						tok->value = malloc(sizeof(dt_NUM));
						*((dt_NUM*) tok->value) = getNUM(lexeme);
						return (dt_token)tok;
					}
				}
				break;

				case 27: // FINAL
				{
					if(input>='0' && input<='9')
					{
						s=28;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}
				break;

				case 28:
				{
					if(input>='0' && input<='9')
					{
						s=28;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 29:
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					dt_token tok = makeToken(lexeme, TK_RNUM, line_number);
					tok->value = malloc(sizeof(dt_RNUM));
					*((dt_NUM*) tok->value) = getRNUM(lexeme);
					return (dt_token)tok;
				}

				case 30:
				{
					if (input=='a')
					{
						s=31;
					}
					else if (input=='n')
					{
						s=35;
					}
					else if (input=='o')
					{
						s=39;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 31:
				{
					if (input=='n')
					{
						s=32;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 32:
				{
					if (input=='d')
					{
						s=33;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 33:
				{
					if (input=='.')
					{
						s=34;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 34:
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_AND, line_number);
				}

				case 35:
				{
					if (input=='o')
					{
						s=36;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 36:
				{
					if (input=='t')
					{
						s=37;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 37:
				{
					if (input=='.')
					{
						s=38;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 38:
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_NOT, line_number);
				}

				case 39:
				{
					if (input=='r')
					{
						s=40;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 40:
				{
					if (input=='.')
					{
						s=41;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 41:
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_OR, line_number);
				}

				case 42:
				{
					if ((input>='a' && input<='z') || (input>='A' && input<='Z'))
					{
						s=43;
					}
					else
					{
						printf("LEXICAL_ERROR: Unrecognized symbol %c (ascii: %d) at state %d", input, (int) input, s);
					}
				}

				case 43:
				{
					if ((input>='a' && input<='z') || (input>='A' && input<='Z') || (input>='0' && input<='9'))
					{
						s=43;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
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

				case 44:
				{
					if (input>='0' && input<='9')
					{
						s=45;
					}
					else if ( (input>='a' && input<='z') || (input>='A' && input<='Z') )
					{
						s=44;
					}
					else
					{
						dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
						s = 1;
						*begin += (fwd - 1);
						printf("BEGINVALUEWHILERETURNING: %d", *begin);
						dt_id tok_id = getTokID(lexeme);
						return (dt_token)makeToken(lexeme, tok_id, line_number);
					}
				}

				case 45:
				{
					dt_str lexeme = strslice(buffer, *begin, *begin + fwd -1);
					s = 1;
					*begin += (fwd - 1);
					return (dt_token)makeToken(lexeme, TK_ID, line_number);
				}
			}
		}
	}
}