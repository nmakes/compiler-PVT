#include <stdio.h>
#include <string.h>

long long int findHash(char * lexeme, int x, int m)
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

int main()
{

	char text[][40] = {"end", "int", "real", "string", "matrix", "if", "else", "endif", "read", "print"};

	int i=0;

	for(i=0; i<10; i++)
	{
		printf("%s = %lld\n", text[i], findHash(text[i], 3, 30));
	}

	return 0;
}