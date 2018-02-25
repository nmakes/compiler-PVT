#include "lexerDef.h"

// token functions
dt_token makeToken(dt_str lexeme, dt_id tokenID, int lineNo);
void printToken(dt_token token);
dt_NUM getNUM(dt_str lexeme);
dt_NUM getRNUM(dt_str lexeme);
dt_id getTokID(dt_str lexeme);

// string functions declarations
dt_str strmake(const char * src);
dt_str strslice(dt_str s, int beg, int end); // s[beg:end] inclusive

// linked list functions
dt_linkedListNode llMakeNode(void * data, int size);
void llFreeNode(dt_linkedListNode node);
void llPushBack(dt_linkedList ll, void * data, int size);
void llPushFront(dt_linkedList ll, void * data, int size);
void llPopFront(dt_linkedList ll);
void llPopBack(dt_linkedList ll);

// Lexer Functions
void removeComments(dt_str testCaseFile);
dt_token getNextToken(FILE * inputFile, char ** buffer, int * begin, int bufSize); // will be used by parser