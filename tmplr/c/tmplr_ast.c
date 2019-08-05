#include "tmplr_ast.h"

const int INIT_AST_CAP = 1000;
const int INIT_ELM_STR_CAP = 10;

Element* generateElement() {
  Element* el = malloc(sizeof(Element));

  return el;
}
Expression* generateExpression() {
  Expression* ex = malloc(sizeof(Expression));

  return ex;
}
Statement* generateStatement() {
  Statement* st = malloc(sizeof(Statement));

  return st;
}
Ast* generateAst() {
  Ast* ast = malloc(sizeof(Ast));

  return ast;
}

void deleteElement(Element*) {

}
void deleteExpression(Expression*) {

}
void deleteStatement(Statement*) {

}
void deleteAst(Ast*) {

}


Ast* createAst(TokenList* parsedTokens) {
  Ast* tree = generateAst();

  deleteTokenList(parsedTokens);
  return tree;
}
