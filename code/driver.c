#include "lexer.h"

// dt_str inputFile = "samplecode.nv";

int main(int argc, char const *argv[])
{
	FILE * f = fopen(argv[1], "r");
	
	int shouldRun = 1;

	if(f==NULL)
	{
		printf("ERROR::driver.c::main: Couldn\'t open code file %s", argv[1]);
		return 0;
	}

	dt_str buf = (dt_str) malloc(sizeof(char) * BUFFER_SIZE);

	memset(buf,'\0',BUFFER_SIZE);
	int begin = 0;

	printf("line \t\t\t token \t\t\t\t lexeme \n");
	printf("---- \t\t\t ----- \t\t\t\t ------ \n");

	dt_token tk = NULL;
	while(!feof(f) && shouldRun)
	{
		// printf("begin: %d", begin);
		while(buf!='\0'){
			tk = (dt_token) getNextToken(f, &buf, &begin, BUFFER_SIZE);
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

	return 0;
}