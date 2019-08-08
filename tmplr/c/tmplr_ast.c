#include "tmplr_ast.h"

const int INIT_ELM_STR_CAP = 10;

Element* generateElement() {
  Element* el = malloc(sizeof(Element));
  if (el == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  el->eType = NO_TYPE;
  el->token = malloc(sizeof(Token));
  if (el->token == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  return el;
}

Special* generateSpecial() {
  Special* sp = malloc(sizeof(Special));
  if (sp == NULL) {
    printf("Error creating special");
    exit(EXIT_FAILURE);
  }
  sp->sType = UNDEF_SPEC;
  sp->token = malloc(sizeof(Token));
  if (sp->token == NULL) {
    printf("Error creating special");
    exit(EXIT_FAILURE);
  }
  return sp;
}

Operator* generateOperator() {
  Operator* op = malloc(sizeof(Operator));
  if (op == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  op->oType = NOOP;
  op->token = malloc(sizeof(Token));
  if (op->token == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  return op;
}

Keyword* generateKeyword() {
  Keyword* key = malloc(sizeof(Keyword));
  if (key == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  key->kType = UNDEF_KEY;
  key->token = malloc(sizeof(Token));
  if (key->token == NULL) {
    printf("Error creating element");
    exit(EXIT_FAILURE);
  }
  return key;
}

Expression* generateExpression() {
  Expression* ex = malloc(sizeof(Expression));
  if (ex == NULL) {
    printf("Error creating expression");
    exit(EXIT_FAILURE);
  }
  ex->left = NULL;
  ex->op = NULL;
  ex->right = NULL;
  ex->l_exp = NULL;
  ex->r_exp = NULL;
  ex->exType = UNDEF_EX;
  return ex;
}

Statement* generateStatement() {
  Statement* st = malloc(sizeof(Statement));
  if (st == NULL) {
    printf("Error creating statement");
    exit(EXIT_FAILURE);
  }
  st->keyword = NULL;
  st->type = UNDEF_STATE;
  st->express = NULL;
  st->l_routine = NULL;
  st->r_routine = NULL;
  st->special = NULL;
  return st;
}

Routine* generateRoutine() {
  Routine* routine = malloc(sizeof(Routine));
  if (routine == NULL) {
    printf("Error creating ASTNode");
    exit(EXIT_FAILURE);
  }
  routine->curr = NULL;
  routine->next = NULL;
  return routine;
}

Ast* generateAst() {
  Ast* ast = malloc(sizeof(Ast));
  if (ast == NULL) {
    printf("Error creating AST");
    exit(EXIT_FAILURE);
  }
  ast->root = NULL;
  ast->next = NULL;
  return ast;
}

SmartToken* generateSmartToken(tokenCat type, Token* token) {
  SmartToken* sTok = malloc(sizeof(SmartToken));
  sTok->tType = type;
  switch (sTok->tType) {
    case ELEMENT:
      sTok->el = generateElement();
      deepCopyTokenToElement(sTok->el, token);
      break;
    case OPERATOR:
      sTok->op = generateOperator();
      deepCopyTokenToOperator(sTok->op, token);
      break;
    case SPECIAL:
      sTok->sp = generateSpecial();
      deepCopyTokenToSpecial(sTok->sp, token);
      break;
    case KEYWORD:
      sTok->key = generateKeyword();
      deepCopyTokenToKeyword(sTok->key, token);
      break;
    default:
      return NULL;
  }
  return sTok;
}

TokenBuffer* generateTokenBuffer(int capacity) {
  TokenBuffer* buffer = malloc(sizeof(TokenBuffer));
  buffer->size = capacity;
  buffer->lType = UNDEF_LINE;
  buffer->smartTokens = malloc(buffer->size * sizeof(SmartToken));
  return buffer;
}

void deleteElement(Element* el) {
  el->eType = NO_TYPE;
  deleteToken(el->token);
  free(el);
}

void deleteOperator(Operator* op) {
  op->oType = NOOP;
  deleteToken(op->token);
  free(op);
}

void deleteSpecial(Special* sp) {
  sp->sType = UNDEF_SPEC;
  deleteToken(sp->token);
  free(sp);
}

void deleteKeyword(Keyword* key) {
  key->kType = UNDEF_KEY;
  deleteToken(key->token);
  free(key);
}

void deleteExpression(Expression* ex) {
  switch (ex->exType) {
    case EL_O_EL:
      deleteElement(ex->left);
      deleteOperator(ex->op);
      deleteElement(ex->right);
      break;
    case EX_O_EL:
      deleteExpression(ex->l_exp);
      deleteOperator(ex->op);
      deleteElement(ex->right);
      break;
    case EL_O_EX:
      deleteElement(ex->left);
      deleteOperator(ex->op);
      deleteExpression(ex->r_exp);
      break;
    case EX_O_EX:
      deleteExpression(ex->l_exp);
      deleteOperator(ex->op);
      deleteExpression(ex->r_exp);
      break;
    default:
      break;
  }
  ex->exType = UNDEF_EX;
  free(ex);
}

void deleteStatement(Statement* st) {
  switch (st->type) {
    case K_E_SP:
      deleteKeyword(st->keyword);
      deleteExpression(st->express);
      deleteSpecial(st->special);
      break;
    case K_E_R:
      deleteKeyword(st->keyword);
      deleteExpression(st->express);
      deleteRoutine(st->l_routine);
      break;
    case E_SP:
      deleteExpression(st->express);
      deleteSpecial(st->special);
      break;
    case K_R_R:
      deleteKeyword(st->keyword);
      deleteRoutine(st->l_routine);
      deleteRoutine(st->r_routine);
      break;
    default:
      break;
  }
  st->type = UNDEF_STATE;
  free(st);
}

void deleteRoutine(Routine* routine) {
  if (routine->next != NULL) {
    deleteRoutine(routine->next);
  }
  deleteStatement(routine->curr);
  free(routine);
}

void deleteAst(Ast* ast) {
  if (ast->next != NULL) {
    deleteAst(ast->next);
  }
  deleteRoutine(ast->root);
  free(ast);
}

void deleteSmartToken(SmartToken* sTok) {
  switch (sTok->tType) {
    case ELEMENT:
      deleteElement(sTok->el);
      break;
    case OPERATOR:
      deleteOperator(sTok->op);
      break;
    case SPECIAL:
      deleteSpecial(sTok->sp);
      break;
    case KEYWORD:
      deleteKeyword(sTok->key);
      break;
    default:
      break;
  }
  sTok->tType = UNDEF_TOKEN;
  free(sTok);
}

void deleteTokenBuffer(TokenBuffer* buffer) {
  buffer->lType = UNDEF_LINE;
  for (int i = 0; i < buffer->size; i++) {
    deleteSmartToken(buffer->smartTokens[i]);
  }
  free(buffer->smartTokens);
  buffer->size = 0;
  free(buffer);
}

tokenCat getTokenType(Token* token) {
  tokenCat tokenType;
  if ((strcmp(token->leximes, "==") == 0) || (strcmp(token->leximes, "!=") == 0) ||
      (strcmp(token->leximes, ">=") == 0) || (strcmp(token->leximes, "<=") == 0) ||
      (strcmp(token->leximes, "*=") == 0) || (strcmp(token->leximes, "+=") == 0) ||
      (strcmp(token->leximes, "/=") == 0) || (strcmp(token->leximes, "-=") == 0) ||
      (strcmp(token->leximes, "&=") == 0) || (strcmp(token->leximes, "|=") == 0) ||
      (strcmp(token->leximes, "^=") == 0) || (strcmp(token->leximes, "~=") == 0) ||
      (strcmp(token->leximes, "%=") == 0) || (strcmp(token->leximes, "<<=") == 0) ||
      (strcmp(token->leximes, ">>=") == 0) || (strcmp(token->leximes, "=") == 0) ||
      (strcmp(token->leximes, "<<") == 0) || (strcmp(token->leximes, ">>") == 0) ||
      (strcmp(token->leximes, "**") == 0) || (strcmp(token->leximes, "?") == 0) ||
      (strcmp(token->leximes, "++") == 0) || (strcmp(token->leximes, "--") == 0) ||
      (strcmp(token->leximes, "&&") == 0) || (strcmp(token->leximes, "||") == 0) ||
      (strcmp(token->leximes, "<") == 0) || (strcmp(token->leximes, ">") == 0) ||
      (strcmp(token->leximes, "+") == 0) || (strcmp(token->leximes, "-") == 0) ||
      (strcmp(token->leximes, "*") == 0) || (strcmp(token->leximes, "/") == 0) ||
      (strcmp(token->leximes, "%") == 0) || (strcmp(token->leximes, "&") == 0) ||
      (strcmp(token->leximes, "|") == 0) || (strcmp(token->leximes, "^") == 0) ||
      (strcmp(token->leximes, "!") == 0) || (strcmp(token->leximes, "~") == 0))  {
    tokenType = OPERATOR;
  }
  else if ((strcmp(token->leximes, "if") == 0) || (strcmp(token->leximes, "elif") == 0) ||
           (strcmp(token->leximes, "else") == 0) || (strcmp(token->leximes, "for") == 0) ||
           (strcmp(token->leximes, "while") == 0) || (strcmp(token->leximes, "do") == 0) ||
           (strcmp(token->leximes, "jump") == 0) || (strcmp(token->leximes, "label") == 0) ||
           (strcmp(token->leximes, "break") == 0) || (strcmp(token->leximes, "break") == 0) ||
           (strcmp(token->leximes, "make") == 0) || (strcmp(token->leximes, "return") == 0) ||
           (strcmp(token->leximes, "switch") == 0) || (strcmp(token->leximes, "case") == 0) ||
           (strcmp(token->leximes, "default") == 0) || (strcmp(token->leximes, "include") == 0) ||
           (strcmp(token->leximes, "try") == 0) || (strcmp(token->leximes, "catch") == 0) ||
           (strcmp(token->leximes, "throw") == 0) || (strcmp(token->leximes, "create") == 0) ||
           (strcmp(token->leximes, "destroy") == 0)) {
    tokenType = KEYWORD;
  }
  else if ((strcmp(token->leximes, "(") == 0) || (strcmp(token->leximes, ")") == 0) ||
           (strcmp(token->leximes, "{") == 0) || (strcmp(token->leximes, "}") == 0) ||
           (strcmp(token->leximes, "[") == 0) || (strcmp(token->leximes, "]") == 0) ||
           (strcmp(token->leximes, ";") == 0) || (strcmp(token->leximes, ":") == 0) ||
           (strcmp(token->leximes, "\n") == 0)) {
    tokenType = SPECIAL;
  }
  else {
    tokenType = ELEMENT;
  }
  return tokenType;
}

void deepCopyTokenToElement(Element* el, Token* tok) {
  el->token = generateToken();
  while (el->token->capacity <= tok->size) {
    doubleTokenCapacity(el->token);
  }
  el->token->size = tok->size;
  memcpy(el->token->leximes, tok->leximes, tok->size);
}

void deepCopyTokenToOperator(Operator* op, Token* tok) {
  op->token = generateToken();
  while (op->token->capacity <= tok->size) {
    doubleTokenCapacity(op->token);
  }
  op->token->size = tok->size;
  memcpy(op->token->leximes, tok->leximes, tok->size);
}

void deepCopyTokenToSpecial(Special* sp, Token* tok) {
  sp->token = generateToken();
  while (sp->token->capacity <= tok->size) {
    doubleTokenCapacity(sp->token);
  }
  sp->token->size = tok->size;
  memcpy(sp->token->leximes, tok->leximes, tok->size);
}

void deepCopyTokenToKeyword(Keyword* key, Token* tok) {
  key->token = generateToken();
  while (key->token->capacity <= tok->size) {
    doubleTokenCapacity(key->token);
  }
  key->token->size = tok->size;
  memcpy(key->token->leximes, tok->leximes, tok->size);
}

/*
switch (tokType) {
  case ELEMENT:
    lineBuffer->smartTokens[i] = generateSmartToken(ELEMENT, tokens->list[i]);
    break;
  case OPERATOR:
    op = generateOperator();
    deepCopyTokenToOperator(op, tokens->list[i]);
    break;
  case SPECIAL:
    sp = generateSpecial();
    deepCopyTokenToSpecial(sp, tokens->list[i]);
    break;
  case KEYWORD:
    key = generateKeyword();
    deepCopyTokenToKeyword(key, tokens->list[i]);
    break;
  default:
    printf("Error parsing token");
    exit(EXIT_FAILURE);
    break;
}
*/

Ast* createAst(TokenList* tokens) {
  Ast* head = generateAst();
  head->root = generateRoutine();
  head->root->curr = generateStatement();

  Ast* currAst = head;
  Routine* currRoutine = currAst->root;
  TokenBuffer* lineBuffer;
  for (int i = 0; i < tokens->size; i++) {
    if (strcmp(tokens->list[i]->leximes, "\n") == 0) {
      int j = 0;
      while (((i + j + 1) < tokens->size) && (strcmp(tokens->list[i + j + 1]->leximes, "\n") != 0)) {
        j++;
      }
      if (j != 0) {
        lineBuffer = generateTokenBuffer(j);
      }
    }
    else {
      for (int k = 0; k < lineBuffer->size; k++) {
        tokenCat tokType = getTokenType(tokens->list[k]);
        lineBuffer->smartTokens[k] = generateSmartToken(tokType, tokens->list[k]);
        if (tokType == OPERATOR) {
          switch (tokens->list[k]->leximes[0]) {
            case '*':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = MULT;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '*':
                    lineBuffer->smartTokens[k]->op->oType = STARSTAR;
                    break;
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = MULT_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '+':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = ADD;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '+':
                    lineBuffer->smartTokens[k]->op->oType = PLUSPLUS;
                    break;
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = ADD_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '-':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = SUB;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '-':
                    lineBuffer->smartTokens[k]->op->oType = MINUSMINUS;
                    break;
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = SUB_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '/':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = DIV;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = SUB_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '&':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = AND;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = AND_E;
                    break;
                  case '&':
                    lineBuffer->smartTokens[k]->op->oType = ANDAND;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                  }
              }
              break;
            case '|':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = OR;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = OR_E;
                    break;
                  case '|':
                    lineBuffer->smartTokens[k]->op->oType = OROR;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                  }
              }
              break;
            case '^':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = XOR;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = XOR_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '<':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = LESS;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = LESS_E;
                    break;
                  case '<':
                    if (tokens->list[k]->leximes[2] == '\0') {
                      lineBuffer->smartTokens[k]->op->oType = LSHIFT;
                    }
                    else {
                      switch (tokens->list[k]->leximes[2]) {
                        case '=':
                          lineBuffer->smartTokens[k]->op->oType = LSHIFT_E;
                          break;
                        default:
                          lineBuffer->smartTokens[k]->op->oType = NOOP;
                          break;
                      }
                    }
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '>':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = GREATER;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = GREATER_E;
                    break;
                  case '<':
                    if (tokens->list[k]->leximes[2] == '\0') {
                      lineBuffer->smartTokens[k]->op->oType = RSHIFT;
                    }
                    else {
                      switch (tokens->list[k]->leximes[2]) {
                        case '=':
                          lineBuffer->smartTokens[k]->op->oType = RSHIFT_E;
                          break;
                        default:
                          lineBuffer->smartTokens[k]->op->oType = NOOP;
                          break;
                      }
                    }
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '!':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = NOT;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = NOT_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '~':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = TWOCOMP;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = TWOCOMP_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '%':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = MOD;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = MOD_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '=':
              if (tokens->list[k]->leximes[1] == '\0') {
                lineBuffer->smartTokens[k]->op->oType = EQUAL;
              }
              else {
                switch (tokens->list[k]->leximes[1]) {
                  case '=':
                    lineBuffer->smartTokens[k]->op->oType = EQUAL_E;
                    break;
                  default:
                    lineBuffer->smartTokens[k]->op->oType = NOOP;
                    break;
                }
              }
              break;
            case '?':
              lineBuffer->smartTokens[k]->op->oType = WILDCARD;
              break;
            default:
              lineBuffer->smartTokens[k]->op->oType = NOOP;
              break;
          }
        }
        else if (tokType == SPECIAL) {
          switch (tokens->list[k]->leximes[0]) {
            case '{':
              lineBuffer->smartTokens[k]->sp->sType = L_CURLY;
              break;
            case '}':
              lineBuffer->smartTokens[k]->sp->sType = R_CURLY;
              break;
            case '(':
              lineBuffer->smartTokens[k]->sp->sType = L_PAREN;
              break;
            case ')':
              lineBuffer->smartTokens[k]->sp->sType = R_PAREN;
              break;
            case '[':
              lineBuffer->smartTokens[k]->sp->sType = L_BRACE;
              break;
            case ']':
              lineBuffer->smartTokens[k]->sp->sType = R_BRACE;
              break;
            case ';':
              lineBuffer->smartTokens[k]->sp->sType = SEMI;
              break;
            case ':':
              lineBuffer->smartTokens[k]->sp->sType = COLON;
              break;
            case '\n':
              lineBuffer->smartTokens[k]->sp->sType = NEWLINE;
              break;
            default:
              lineBuffer->smartTokens[k]->sp->sType = UNDEF_SPEC;
              break;
          }
        }
        else if (tokType == ELEMENT) {
          //TODO handle element type specifying
        }
        else if (tokType == KEYWORD) {
          if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "if") == 0) {
            lineBuffer->smartTokens[k]->key->kType = IF;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "else") == 0) {
            lineBuffer->smartTokens[k]->key->kType = ELSE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "elif") == 0) {
            lineBuffer->smartTokens[k]->key->kType = ELIF;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "while") == 0) {
            lineBuffer->smartTokens[k]->key->kType = WHILE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "for") == 0) {
            lineBuffer->smartTokens[k]->key->kType = FOR;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "do") == 0) {
            lineBuffer->smartTokens[k]->key->kType = DO;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "jump") == 0) {
            lineBuffer->smartTokens[k]->key->kType = JUMP;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "label") == 0) {
            lineBuffer->smartTokens[k]->key->kType = LABEL;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "break") == 0) {
            lineBuffer->smartTokens[k]->key->kType = BREAK;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "make") == 0) {
            lineBuffer->smartTokens[k]->key->kType = MAKE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "return") == 0) {
            lineBuffer->smartTokens[k]->key->kType = RETURN;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "switch") == 0) {
            lineBuffer->smartTokens[k]->key->kType = SWITCH;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "case") == 0) {
            lineBuffer->smartTokens[k]->key->kType = CASE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "try") == 0) {
            lineBuffer->smartTokens[k]->key->kType = TRY;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "catch") == 0) {
            lineBuffer->smartTokens[k]->key->kType = CATCH;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "throw") == 0) {
            lineBuffer->smartTokens[k]->key->kType = THROW;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "include") == 0) {
            lineBuffer->smartTokens[k]->key->kType = INCLUDE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "create") == 0) {
            lineBuffer->smartTokens[k]->key->kType = CREATE;
          }
          else if (strcmp(lineBuffer->smartTokens[k]->key->token->leximes, "destroy") == 0) {
            lineBuffer->smartTokens[k]->key->kType = DESTROY;
          }
          else {
            lineBuffer->smartTokens[k]->key->kType = UNDEF_KEY;
          }
        }
      }
      //linebuffer full of one line, now to make AST bits
      SmartToken** lineStack = malloc(lineBuffer->size * sizeof(SmartToken*));
      int marker = 0;
      for (int l = 0; l < lineBuffer->size; l++) {
        if ((lineBuffer->smartTokens[l]->tType == SPECIAL) && (
            (lineBuffer->smartTokens[l]->sp->sType == R_PAREN) ||
            (lineBuffer->smartTokens[l]->sp->sType == R_BRACE) ||
            (lineBuffer->smartTokens[l]->sp->sType == R_CURLY))) {
          //pop
          marker--;
        }
        else {
          //push
          marker++;
        }
      }

      i += lineBuffer->size;
      deleteTokenBuffer(lineBuffer);
    }
  }

  deleteTokenList(tokens);
  return head;
}
