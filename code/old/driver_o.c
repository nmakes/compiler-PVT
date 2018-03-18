#include "lexer.h"

dt_str inputFile = "samplecode.nv";

int main()
{
	FILE * f = fopen(inputFile, "r");
	
	if(f==NULL)
	{
		printf("ERROR::driver.c::main: Couldn\'t open code file %s", inputFile);
		return 0;
	}

	char *buf = (char*) malloc(BUFFER_SIZE*sizeof(char));
	memset(buf,'\0',BUFFER_SIZE);
	int begin = 0;

	dt_token tk = NULL;

	do
	{
		printf("begin: %d", begin);
		tk = (dt_token) getNextToken(f, buf, &begin, BUFFER_SIZE);
		if(tk!=NULL)
		{
			if(tk->tokenID==TK_EXIT)
				break;
			printToken(tk);
		}

	} while(tk!=NULL || !feof(f));

	return 0;
}