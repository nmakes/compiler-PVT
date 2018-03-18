// Naveen Venkat
// 2015A7PS0078P

#include "parser.h"

// dt_str inputFile = "samplecode.nv";

void printMenu()
{
	printf("\n\n");
	printf("Please select an option:\n");
	printf("1. Remove Comments\n");
	printf("2. Print Tokens\n");
	printf("3. Parse Input Source Code\n");
	printf("4. Exit\n");
	printf("> ");
}

int main(int argc, char *argv[])
{
	dt_str testCaseFileName = argv[1];

	printMenu();
	int inp;
	scanf("%d", &inp);

	if(inp==1)
	{
		removeComments(testCaseFileName);
	}

	else if(inp==2)
	{
		FILE * f = fopen(testCaseFileName, "r");
		
		int shouldRun = 1;

		if(f==NULL)
		{
			printf("ERROR::driver.c::main: Couldn\'t open code file %s", argv[1]);
			return 0;
		}

		dt_str BUFFERBASE = (dt_str) malloc(sizeof(char) * BUFFER_SIZE);
		memset(BUFFERBASE,'\0',BUFFER_SIZE);

		dt_str buf = BUFFERBASE;

		int begin = 0;

		printf("\n~~~ TOKEN LIST ~~~\n\n");
		printf("%-10s %-20s %-20s\n","line","token", "lexeme");
		printf("%-10s %-20s %-20s\n","----","-----", "------");

		dt_token tk = NULL;
		while(!feof(f) && shouldRun)
		{
			// printf("begin: %d", begin);
			while(buf!='\0'){
				tk = (dt_token) getNextToken(f, &buf, &begin, BUFFER_SIZE, BUFFERBASE);
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

	else if(inp==3)
	{

		FILE * grammarFile = fopen("grammarFile.txt", "r");
		grammar g = loadGrammar(grammarFile);

		FILE * firstNT = fopen("firstNT.txt", "r");
		FILE * firstRules = fopen("firstRules.txt", "r");
		FILE * followNT = fopen("followNT.txt", "r");
		
		firstAndFollow ffSets = computeFirstAndFollowSets(firstNT, firstRules, followNT);

		parseTable T = ptInitEmptyTable(g->numNonTerminals, g->numTerminals);
		createParseTable(ffSets, g, T);

		parseInputSourceCode(testCaseFileName, T, g);
		freeParseTable(T);
	}

	else if(inp==4)
	{
		return 0;
	}
	
	return 0;
}
