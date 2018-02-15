#include "constructs.h"

flag shouldRun = True;

dt_dfaState s = 1;
char input;

// read input file into buffer
char buffer[BUFFER_SIZE];

// define indices and global variables
int begin=0, forward=0;
int line_number = 1;

char getNextChar(char * buffer, int * fwd)
{
	return buffer[(*fwd)++];
}



// BASIC LOOP of the LEXER

while(shouldRun)
{
	input = getNextChar(buffer, &forward);

	switch(s)
	{
		case 1: // NONFINAL

			if (input==' ')
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
			}

		break;


		case 2: // NONFINAL

			if(input=='\n')
			{
				s = 3;
				line_number++;
			}
			else
				continue;

		break;


		case 3: // FINAL

			// transit back to state 1
			s = 1;

		break;

		case 4: // FINAL

			


	}
}