#ifndef TMPLR_AST_H
#define TMPLR_AST_H

#include "tmplr_parser.h"

typedef enum { ELEMENT, SPECIAL, OPERATOR, KEYWORD, UNDEF_TOKEN } tokenCat;

typedef enum { INTEGER, DOUBLE, STRING, NO_TYPE } elemType;
typedef enum { ADD, SUB, MULT, DIV, AND, OR, XOR, LSHIFT, RSHIFT, NOT, TWOCOMP,
               ADD_E, SUB_E, MULT_E, DIV_E, AND_E, OR_E, XOR_E, LSHIFT_E, RSHIFT_E,
               NOT_E, TWOCOMP_E, EQUAL, GREATER, LESS, EQUAL_E, GREATER_E, LESS_E,
               MOD, MOD_E, PLUSPLUS, MINUSMINUS, STARSTAR, ANDAND, OROR, WILDCARD,
               NOOP } opType;
typedef enum { K_E_SP, K_E_R, E_SP, K_R_R, UNDEF_STATE } stateType;
typedef enum { L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_CURLY, R_CURLY, NEWLINE, SEMI,
               COLON, UNDEF_SPEC } specType;
typedef enum { IF, ELSE, ELIF, WHILE, FOR, DO, JUMP, LABEL, BREAK, MAKE, RETURN,
               SWITCH, CASE, TRY, CATCH, THROW, INCLUDE, CREATE, DESTROY, UNDEF_KEY} keyType;
typedef enum { EL_O_EL, EX_O_EL, EL_O_EX, EX_O_EX, UNDEF_EX } exType;

typedef struct _ELEMENT {
  elemType eType;
  Token* token;
} Element;

typedef struct _SPECIAL {
  specType sType;
  Token* token;
} Special;

typedef struct _OPERATOR {
  opType oType;
  Token* token;
} Operator;

typedef struct _KEYWORD {
  keyType kType;
  Token* token;
} Keyword;

typedef struct _EXPRESSION {
  exType exType;
  Element* left;
  Operator* op;
  Element* right;
  struct _EXPRESSION* l_exp;
  struct _EXPRESSION* r_exp;
} Expression;

struct _ROUTINE;

typedef struct _STATEMENT {
  stateType type;
  Keyword* keyword;
  struct _ROUTINE* l_routine;
  struct _ROUTINE* r_routine;
  Expression* express;
  Special* special;
} Statement;

typedef struct _ROUTINE {
  Statement* curr;
  struct _ROUTINE* next;
} Routine;

typedef struct _AST {
  Routine* root;
  struct _AST* next;
} Ast;

Element* generateElement();
Special* generateSpecial();
Operator* generateOperator();
Keyword* generateKeyword();
Expression* generateExpression();
Statement* generateStatement();
Routine* generateRoutine();
Ast* generateAst();

void deleteElement(Element*);
void deleteSpecial(Special*);
void deleteOperator(Operator*);
void deleteKeyword(Keyword*);
void deleteExpression(Expression*);
void deleteStatement(Statement*);
void deleteRoutine(Routine*);
void deleteAst(Ast*);

Ast* createAst(TokenList*);

#endif
