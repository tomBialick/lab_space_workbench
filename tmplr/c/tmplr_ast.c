#include "tmplr_ast.h"

const int INIT_ELM_STR_CAP = 10;

Element* generateElement() {
  Element* el = malloc(sizeof(Element));
  if (el == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  el->eType = EMPTY;
  el->intVal = 0;
  el->strCapacity = INIT_ELM_STR_CAP;
  el->strVal = malloc(el->strCapacity * sizeof(char));
  if (el->strVal == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  el->strLength = 0;
  el->dubVal = 0;
  return el;
}

Expression* generateExpression() {
  Expression* ex = malloc(sizeof(Expression));
  if (ex == NULL) {
    printf("Error creating expression");
    exit(EXIT_FAILURE);
  }
  ex->left = generateElement();
  ex->right = generateElement();
  ex->l_exp = NULL;
  ex->r_exp = NULL;
  ex->op = NOOP;
  return ex;
}

Statement* generateStatement() {
  Statement* st = malloc(sizeof(Statement));
  if (st == NULL) {
    printf("Error creating statement");
    exit(EXIT_FAILURE);
  }
  st->type = EMPTY;
  st->statement = NULL;
  st->express = generateExpression();
  return st;
}

AstNode* generateAstNode() {
  AstNode* astNode = malloc(sizeof(AstNode));
  if (astNode == NULL) {
    printf("Error creating ASTNode");
    exit(EXIT_FAILURE);
  }
  astNode->curr = generateStatement();
  if (astNode->curr == NULL) {
    printf("Error creating ASTNode");
    exit(EXIT_FAILURE);
  }
  astNode->next = NULL;
  return astNode;
}

Ast* generateAst() {
  Ast* ast = malloc(sizeof(Ast));
  if (ast == NULL) {
    printf("Error creating AST");
    exit(EXIT_FAILURE);
  }
  ast->root = generateAstNode();
  return ast;
}

void deleteElement(Element* el) {
  el->eType = EMPTY;
  el->intVal = 0;
  el->dubVal = 0;
  free(el->strVal);
  el->strCapacity = 0;
  el->strLength = 0;
  free(el);
}

void deleteExpression(Expression* ex) {
  deleteElement(ex->left);
  deleteElement(ex->right);
  if (ex->l_exp != NULL) {
    deleteExpression(ex->l_exp);
  }
  if (ex->r_exp != NULL) {
    deleteExpression(ex->r_exp);
  }
  ex->op = NOOP;
  free(ex);
}

void deleteStatement(Statement* st) {
  st->type = EMPTY;
  if (st->statement != NULL) {
    deleteStatement(st->statement);
  }
  deleteExpression(st->express);
  free(st);
}

void deleteAstNode(AstNode* astNode) {
  if (astNode->next != NULL) {
    deleteAstNode(astNode->next);
  }
  deleteStatement(astNode->curr);
  free(astNode);
}

void deleteAst(Ast* ast) {
  deleteAstNode(ast->root);
  free(ast);
}

void doubleElementStrCap(Element* el) {
  el->strCapacity = el->strCapacity * 2;
  el->strVal = realloc(el->strVal, (sizeof(char) * el->strCapacity));
  if (el == NULL) {
    printf("Error expanding element");
    exit(EXIT_FAILURE);
  }
}


Ast* createAst(TokenLine* lines) {
  Ast* ast = generateAst();
  AstNode* curr = ast->root;
  while (lines->next != NULL) {
    //Token* currToken = lineTokens[i];



    if (lines->next->next != NULL) {
      curr->next = generateAstNode();
      curr = curr->next;
    }
  }

  deleteTokenLine(lines);
  return ast;
}
