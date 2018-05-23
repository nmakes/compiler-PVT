// Naveen Venkat
// 2015A7PS0078P

#include "ast.h"

// dt_str inputFile = "samplecode.nv";
extern int line_number;
int exitoption = 0;

void printMenu()
{
	printf("\n\n");
	printf("Please select an option:\n");
	printf("0. Exit\n");
	printf("1. Print Tokens List\n");
	printf("2. Parse Input Source Code & Print Parse Tree\n");
	printf("3. Print Abstract Syntax Tree\n");
	printf("4. View Memory Usage\n");
	printf("> ");
}

int main(int argc, char *argv[])
{
	dt_str testCaseFileName = argv[1];
	int inp = -1;

	parseTree ptree = NULL;
	ast astree = (ast) malloc(sizeof(struct __AST));
	astree->root = NULL;
	astree->numNodes = 0;

	while(inp!=exitoption)
	{
		printMenu();
		scanf("%d", &inp);

		if(inp==1)
		{
			FILE * f = fopen(testCaseFileName, "r");
			rewind(f);
			line_number = 1;

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
				while(buf!='\0' && shouldRun)
				{
					tk = (dt_token) getNextToken(f, &buf, &begin, BUFFER_SIZE, BUFFERBASE);
					if(tk!=NULL)
					{
						// printf("notnull");
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

			printf("\n~~~~~~~~~~~~~~~~~~\n\n");

			fclose(f);
			free(BUFFERBASE);
		}

		else if(inp==2)
		{
			line_number = 1;
			FILE * grammarFile = fopen("grammarFile.txt", "r");
			grammar g = loadGrammar(grammarFile);

			FILE * firstNT = fopen("firstNT.txt", "r");
			FILE * firstRules = fopen("firstRules.txt", "r");
			FILE * followNT = fopen("followNT.txt", "r");
			
			firstAndFollow ffSets = computeFirstAndFollowSets(firstNT, firstRules, followNT);

			parseTable T = ptInitEmptyTable(g->numNonTerminals, g->numTerminals);
			createParseTable(ffSets, g, T);

			ptree = parseInputSourceCode(testCaseFileName, T, g);
			freeParseTable(T);

			fclose(grammarFile);
			fclose(firstNT);
			fclose(firstRules);
			fclose(followNT);

			ptrPrintParseTree(ptree);
		}

		else if(inp==3)
		{
			if(ptree==NULL)
			{
				printf("Please generate parse tree (step 2) before AST!\n");
			}
			else
			{
				astree->root = buildAstNode(ptree->root, NULL);
				astPrintTree( astree );
			}
		}

		else if(inp==4)
		{
			if(astree == NULL)
			{
				printf("Please generate abstract syntax tree (step 3) before displaying memory!\n");
			}
			else
			{
				int ptn = ptree->numNodes;
				int atn = astree->numNodes;

				long int pts = ptn * sizeof(parseTreeNode);
				long int ats = atn * sizeof(astNode);

				printf("\nParse Tree number of Nodes: %d\tAllocated Memory: %ld\n", ptn, pts);
				printf("\nAbstract Syntax Tree number of Nodes: %d\tAllocated Memory: %ld\n", atn, ats);
				printf("\nCompression percentage: %f\n", ((float)(pts - ats)) / (pts));
			}
		}

		else if(inp==exitoption)
		{
			return 0;
		}
	}
	
	return 0;
}
