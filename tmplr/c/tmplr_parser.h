#ifndef TMPLR_PARSER_H
#define TMPLR_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _TOKEN {
  char* leximes;
  int size;
  int capacity;
  int building_flag;
} Token;

typedef struct _TOKEN_LIST {
  Token** list;
  int size;
  int capacity;
} TokenList;

typedef struct _TOKEN_LINE {
  TokenList* tokens;
  struct _TOKEN_LINE* next;
} TokenLine;

Token* generateToken();

TokenList* generateTokenList();

TokenLine* generateTokenLine();

void deleteToken(Token*);

void deleteTokenList(TokenList*);

void deleteTokenLine(TokenLine*);

void doubleTokenListCapacity(TokenList*);

void doubleTokenCapacity(Token*);

TokenLine* parse(FILE*);

#endif
