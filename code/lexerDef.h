#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096
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
			TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, 

			// control symbols
			TK_EXIT, TK_ABRUPTEND,
			
			// non terminals
			TK_mainFunction, TK_stmtsAndFuntionDefs, TK_stmtOrFunctionDef, 
			TK_stmt, TK_functionDef, TK_parameter_list, TK_type, TK_remainingList, 
			TK_declarationStmt, TK_var_list, TK_more_ids, TK_assignmentStmt_type1, 
			TK_assignmentStmt_type2, TK_leftHandSide_singleVar, TK_leftHandSide_listVar, 
			TK_rightHandSide_type1, TK_rightHandSide_type2, TK_sizeExpression, TK_ifStmt, 
			TK_otherStmts, TK_ioStmt, TK_funCallStmt, TK_inputParameterList, TK_listVar, 
			TK_arithmeticExpression, TK_arithmeticTerm, TK_factor, TK_operator_lowPrecedence, 
			TK_operator_highPrecedence, TK_constrainedVars, TK_var, TK_matrix, TK_rows, 
			TK_row, TK_remainingColElements, TK_matrixElement, TK_logicalOp, 
			TK_booleanExpression, TK_relationalOp
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

