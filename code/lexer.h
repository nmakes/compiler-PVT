#include "lexerDef.h"

dt_token makeToken(dt_str lexeme, dt_id tokenID, int lineNo);
void printToken(dt_token token);
dt_NUM getNUM(dt_str lexeme);
dt_NUM getRNUM(dt_str lexeme);
dt_id getTokID(dt_str lexeme);

// string functions declarations
dt_str strmake(dt_str src);
dt_str strslice(dt_str s, int beg, int end); // s[beg:end] inclusive

// Lexer Functions
void removeComments(FILE *testCaseFile, FILE *cleanFile);
dt_token getNextToken(FILE * inputFile, char ** buffer, int * begin, int bufSize); // will be used by parser