// Naveen Venkat
// 2015A7PS0078P

#ifndef __LEXER_H
#define __LEXER_H

#include "lexerDef.h"

// token functions
dt_token makeToken(dt_str lexeme, dt_id tokenID, int lineNo);
void printToken(dt_token token);
dt_NUM getNUM(dt_str lexeme);
dt_NUM getRNUM(dt_str lexeme);
dt_id getTokID(dt_str lexeme);

int isTerminal(dt_id x);
int isNonTerminal(dt_id x);

// string functions declarations
dt_str strmake(const char * src);
dt_str strslice(dt_str s, int beg, int end); // s[beg:end] inclusive

// Lexer Functions
void removeComments(dt_str testCaseFile);
dt_token getNextToken(FILE * inputFile, char ** buffer, int * begin, int bufSize, dt_str BUFFERBASE); // will be used by parser

#endif