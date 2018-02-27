// Naveen Venkat
// 2015A7PS0078P

#include "parser.h"

// dt_str inputFile = "samplecode.nv";

void printMenu()
{
	printf("\n\n");
	printf("NCC compiler : v1.0");
	printf("1. ");
}

int main(int argc, char const *argv[])
{

	dt_str tcfile = "testcases/testcase1.txt";

	/*

	dt_str testCaseFileName = strmake(argv[1]);
	removeComments(testCaseFileName);
	FILE * f = fopen(testCaseFileName, "r");
	
	int shouldRun = 1;

	if(f==NULL)
	{
		printf("ERROR::driver.c::main: Couldn\'t open code file %s", argv[1]);
		return 0;
	}

	dt_str buf = (dt_str) malloc(sizeof(char) * BUFFER_SIZE);

	memset(buf,'\0',BUFFER_SIZE);
	int begin = 0;

	printf("\n~~~ TOKEN LIST ~~~\n\n");
	printf("%-10s %-20s %-20s\n","line","token", "lexeme");
	printf("%-10s %-20s %-20s\n","----","-----", "------");

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

	*/

	// dt_str testCaseFileName = strmake(argv[1]);
	dt_str testCaseFileName = strmake(tcfile);
	removeComments(testCaseFileName);

	FILE * grammarFile = fopen("grammarFile.txt", "r");
	grammar g = loadGrammar(grammarFile);
	// printGrammar(g);

	FILE * firstNT = fopen("firstNT.txt", "r");
	FILE * firstRules = fopen("firstRules.txt", "r");
	FILE * followNT = fopen("followNT.txt", "r");
	
	firstAndFollow ffSets = computeFirstAndFollowSets(firstNT, firstRules, followNT);
	printFirstFollowSets(ffSets, g);

	parseTable T = ptInitEmptyTable(g->numNonTerminals, g->numTerminals);
	createParseTable(ffSets, g, T);
	printParseTable(T, g);

	printf("\n");

	parseInputSourceCode(testCaseFileName, T, g);

	return 0;
}