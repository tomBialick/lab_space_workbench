#ifndef TMPLR_AST_H
#define TMPLR_AST_H

#include "tmplr_parser.h"

enum elemType = {INTEGER, DOUBLE, STRING};
enum opType = {ADD, SUB, MULT, DIV, AND, OR, XOR, LSHIFT, RSHIFT, NOT, TWOCOMP,
               ADD_E, SUB_E, MULT_E, DIV_E, AND_E, OR_E, XOR_E, LSHIFT_E, RSHIFT_E,
               NOT_E, TWOCOMP_E, EQUAL, GREATER, LESS, EQUAL_E, GREATER_E, LESS_E,
               MOD, MOD_E, PLUSPLUS, MINUSMINUS, STARSTAR, ANDAND, OROR, WILDCARD, NONE
              };
enum stateType = {ARITHMETIC, ASSIGN, DECLARE, CONDITIONAL};

typedef struct _ELEMENT {
  elemType eType;
  int intVal;
  char* strVal;
  int strLength;
  int strCapacity
  double dubVal;
} Element;

typedef struct _EXPRESSION {
  Element left;
  Element right;
  opType op;
} Expression;

typedef struct _STATEMENT {
  stateType type;
  Expression leftExpress;
  opType op;
  Expression rightExpress;
} Statement;

typedef struct _AST {
  Statement* statements;
  int capacity;
  int size;
} Ast;

Element* generateElement();
Expression* generateExpression();
Statement* generateStatement();
Ast* generateAst();

void deleteElement(Element*);
void deleteExpression(Expression*);
void deleteStatement(Statement*);
void deleteAst(Ast*);

Ast* createAst(TokenList*);

#endif
