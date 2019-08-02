#ifndef TMPLR_PARSER_H
#define TMPLR_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Token {
  char* leximes;
  int size;
  int capacity;
  int building_flag;
} Token;

typedef struct _TokenList {
  Token** list;
  int size;
  int capacity;
} TokenList;

Token* generateToken();

TokenList* generateTokenList();

void deleteToken(Token*);

void deleteTokenList(TokenList*);

void doubleTokenListCapacity(TokenList*);

void doubleTokenCapacity(Token*);

TokenList* parse(FILE*);

#endif
