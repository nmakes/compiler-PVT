#ifndef __LEXERDEF_H
#define __LEXERDEF_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "linkedListDef.h"

#define BUFFER_SIZE 4092
#define MAX_LEXEME_SIZE 128
#define HASH_X 3
#define HASH_M 30
#define EOFchar '$'

enum __DT_FLAG {False, True};
typedef enum __DT_FLAG dt_flag;

enum __DT_STATETYPE {FINAL, NONFINAL, ERROR};
typedef enum __DT_STATETYPE dt_stateType;

typedef long long int dt_NUM;
typedef double dt_RNUM;

typedef char * dt_str;

enum __DT_ID {
			// terminals
			TK_ASSIGNOP, TK_COMMENT, TK_FUNID, TK_ID, TK_NUM, TK_RNUM, 
			TK_STR, TK_END, TK_INT, TK_REAL, TK_STRING, TK_MATRIX, TK_MAIN, 
			TK_SQO, TK_SQC, TK_OP, TK_CL, TK_SEMICOLON, TK_COMMA, TK_IF, 
			TK_ELSE, TK_ENDIF, TK_READ, TK_PRINT, TK_FUNCTION, TK_PLUS, 
			TK_MINUS, TK_MUL, TK_DIV, TK_SIZE, TK_AND, TK_OR, TK_NOT, TK_LT, 
			TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, TK_epsilon, ENUM_SEP_TERMINALS, // 0-40

			// control symbols
			TK_EXIT, TK_ABRUPTEND, ENUM_SEP_CONTROL, // 41 - 43
			
			// non terminals
			TK_mainFunction, TK_stmtsAndFunctionDefs, TK_other1, TK_stmtOrFunctionDef, 
			TK_stmt, TK_functionDef, TK_parameter_list, TK_remainingList, TK_type, 
			TK_declarationStmt, TK_var_list, TK_more_ids, TK_assignmentStmt_type1, 
			TK_assignmentStmt_type2, TK_leftHandSide_singleVar, TK_leftHandSide_listVar, 
			TK_rightHandSide_type1, TK_rightHandSide_type2, TK_sizeExpression, TK_ifStmt, 
			TK_other2, TK_otherStmts, TK_ioStmt, TK_funCallStmt, TK_inputParameterList, 
			TK_listVar, TK_arithmeticExpression, TK_other3, TK_arithmeticTerm, TK_other4, 
			TK_factor, TK_operator_lowPrecedence, TK_operator_highPrecedence, TK_booleanExpression, 
			TK_constrainedVars, TK_var, TK_matrix, TK_rows, TK_other5, TK_row, 
			TK_remainingColElements, TK_matrixElement, TK_logicalOp, TK_relationalOp, ENUM_SEP_NONTERMINALS // 44 - 88

			// terminals range : 0 - 39	
			// non terminals range : 44 - 87
		};
typedef enum __DT_ID dt_id;

struct __TOKEN
{
	dt_id tokenID;
	dt_str lexeme;
	void * value;
	int lineNo;
};
typedef struct __TOKEN * dt_token;

#define countNonTerminals (ENUM_SEP_NONTERMINALS - TK_mainFunction)
#define countTerminals (ENUM_SEP_TERMINALS)
#define countLexicalUnits (ENUM_SEP_NONTERMINALS)
#define ntBase (TK_mainFunction)
#define tBase (TK_ASSIGNOP)

#endif