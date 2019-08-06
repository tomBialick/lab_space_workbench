#ifndef TMPLR_AST_H
#define TMPLR_AST_H

#include "tmplr_parser.h"

typedef enum { INTEGER, DOUBLE, STRING, NONE } elemType;
typedef enum { ADD, SUB, MULT, DIV, AND, OR, XOR, LSHIFT, RSHIFT, NOT, TWOCOMP,
               ADD_E, SUB_E, MULT_E, DIV_E, AND_E, OR_E, XOR_E, LSHIFT_E, RSHIFT_E,
               NOT_E, TWOCOMP_E, EQUAL, GREATER, LESS, EQUAL_E, GREATER_E, LESS_E,
               MOD, MOD_E, PLUSPLUS, MINUSMINUS, STARSTAR, ANDAND, OROR, WILDCARD, NOOP
              } opType;
typedef enum { ARITHMETIC, ASSIGN, DECLARE, CONDITIONAL, LOOP, BRACE_PAREN, EMPTY } stateType;

typedef struct _ELEMENT {
  elemType eType;
  int intVal;
  char* strVal;
  int strLength;
  int strCapacity;
  double dubVal;
} Element;

typedef struct _EXPRESSION {
  Element* left;
  Element* right;
  struct _EXPRESSION* l_exp;
  struct _EXPRESSION* r_exp;
  opType op;
} Expression;

typedef struct _STATEMENT {
  stateType type;
  struct _STATEMENT* statement;
  Expression* express;
} Statement;

typedef struct _AST_NODE {
  Statement* curr;
  struct _AST_NODE* next;
} AstNode;

typedef struct _AST {
  AstNode* root;
} Ast;

Element* generateElement();
Expression* generateExpression();
Statement* generateStatement();
Ast* generateAst();

void deleteElement(Element*);
void deleteExpression(Expression*);
void deleteStatement(Statement*);
void deleteAstNode(AstNode*);
void deleteAst(Ast*);

Ast* createAst(TokenLine*);

#endif
