#include "tmplr_parser.h"

const int INIT_LIST_CAP = 100;
const int INIT_TOK_CAP = 10;

Token* generateToken() {
  Token* token = malloc(sizeof(Token));
  token->capacity = INIT_TOK_CAP;
  token->leximes = malloc(token->capacity * sizeof(char));
  token->size = 0;
  token->building_flag = 0;
  return token;
}

TokenList* generateTokenList() {
  TokenList* tokens = malloc(sizeof(TokenList));
  tokens->capacity = INIT_LIST_CAP;
  tokens->list = malloc(tokens->capacity * sizeof(Token));
  tokens->size = 0;
  return tokens;
}

void deleteToken(Token* token) {
  free(token->leximes);
  token->capacity = 0;
  token->size = 0;
  free(token);
}

void deleteTokenList(TokenList* tokens) {
  for (int i = 0; i < tokens->size; i++) {
    deleteToken(tokens->list[i]);
  }
  free(tokens->list);
  tokens->capacity = 0;
  tokens->size = 0;
  free(tokens);
}

/**
 * Pass in a string array and the current capacity to double the capacity
 */
void doubleTokenListCapacity(TokenList* tokens) {
  tokens->capacity = tokens->capacity * 2;
  tokens->list = realloc(tokens->list, (sizeof(Token) * tokens->capacity));
}

/**
 * Pass in a string and the capacity to double its capacity
 */
void doubleTokenCapacity(Token* token) {
  token->capacity = token->capacity * 2;
  token->leximes = realloc(token->leximes, (sizeof(char) * token->capacity));
 }
/**
 * separates into tokens without much interpretation, returns total amount
 */
void naiveParse(FILE* file, TokenList* naiveTokens) {

   Token* token = generateToken();
   char lex = getc(file);

   while (lex != EOF) {
     switch (lex) {
       case ' ':
       case '\n':
       case '\t':
         //Finish current token
         if (token->building_flag == 1) {
           token->leximes[token->size] = '\0';
           if ((naiveTokens->size + 1) == naiveTokens->capacity) {
             doubleTokenListCapacity(naiveTokens);
           }
           naiveTokens->list[naiveTokens->size] = generateToken();
           naiveTokens->list[naiveTokens->size]->size = token->size + 1;
           memcpy(naiveTokens->list[naiveTokens->size]->leximes, token->leximes, (token->size + 1));
           naiveTokens->size++;
           token->building_flag = 0;

           //refresh token
           deleteToken(token);
           token = generateToken();
         }
         //Ignore extra space
         if (lex == '\n') {
           naiveTokens->list[naiveTokens->size] = generateToken();
           naiveTokens->list[naiveTokens->size]->size = 2;
           char singleLexTok[2] = {lex, '\0'};
           memcpy(naiveTokens->list[naiveTokens->size]->leximes, singleLexTok, 2);
           naiveTokens->size++;
         }
         break;
       case '-':
       case '/':
       case '+':
       case '*':
       case '(':
       case ')':
       case '{':
       case '}':
       case '[':
       case ']':
       case '<':
       case '>':
       case '=':
       case '&':
       case '!':
       case '?':
       case '%':
       case '^':
       case ';':
       case '\"':
       case '\'':
       case '\\':
       case '.':
       case ':':
       case ',':
         //Finish current token
         if (token->building_flag == 1) {
           token->leximes[token->size] = '\0';
           if ((naiveTokens->size + 1) == naiveTokens->capacity) {
             doubleTokenListCapacity(naiveTokens);
           }
           naiveTokens->list[naiveTokens->size] = generateToken();
           naiveTokens->list[naiveTokens->size]->size = token->size + 1;
           memcpy(naiveTokens->list[naiveTokens->size]->leximes, token->leximes, (token->size + 1));
           naiveTokens->size++;
           token->building_flag = 0;

           //refresh token
           deleteToken(token);
           token = generateToken();
         }
         naiveTokens->list[naiveTokens->size] = generateToken();
         naiveTokens->list[naiveTokens->size]->size = 2;
         char singleLexTok[2] = {lex, '\0'};
         memcpy(naiveTokens->list[naiveTokens->size]->leximes, singleLexTok, 2);
         naiveTokens->size++;
         break;
       default:
         token->building_flag = 1;
         if ((token->size + 2) == token->capacity) {
           doubleTokenCapacity(token);
         }
         token->leximes[token->size] = lex;
         token->size++;
         break;
     }

     lex = getc(file);
   }
   free(token);
}

TokenList* removeEmptyLines(TokenList* naiveTokens) {
  TokenList* noEmptyLines = generateTokenList();
  int newLineFlag = 0;
  for (int i = 0; i < naiveTokens->size; i++) {
    if ((newLineFlag == 0) && strcmp(naiveTokens->list[i]->leximes, "\n") == 0) {
      newLineFlag = 1;
      if ((noEmptyLines->size + 1) == noEmptyLines->capacity) {
        doubleTokenListCapacity(noEmptyLines);
      }
      //no need to check token size/capacity
      noEmptyLines->list[noEmptyLines->size] = generateToken();
      noEmptyLines->list[noEmptyLines->size]->size = 2;
      memcpy(noEmptyLines->list[noEmptyLines->size]->leximes, naiveTokens->list[i]->leximes, naiveTokens->list[i]->size);
      noEmptyLines->size++;
    }
    else if (naiveTokens->list[i]->leximes[0] >= 33 && naiveTokens->list[i]->leximes[0] <= 126) {
      newLineFlag = 0;
      if ((noEmptyLines->size + 1) == noEmptyLines->capacity) {
        doubleTokenListCapacity(noEmptyLines);
      }
      noEmptyLines->list[noEmptyLines->size] = generateToken();
      while (noEmptyLines->list[noEmptyLines->size]->capacity <= naiveTokens->list[i]->size) {
        doubleTokenCapacity(noEmptyLines->list[noEmptyLines->size]);
      }
      noEmptyLines->list[noEmptyLines->size]->size = naiveTokens->list[i]->size;
      memcpy(noEmptyLines->list[noEmptyLines->size]->leximes, naiveTokens->list[i]->leximes, naiveTokens->list[i]->size);
      noEmptyLines->size++;
    }
  }
  deleteTokenList(naiveTokens);
  return noEmptyLines;
}



/**
 * separates into tokens with interpretation, returns total amount
 */
int parse(FILE* file) {
   TokenList* tokens = generateTokenList();
   naiveParse(file, tokens);

   //print naiveTokens to the console
   for (int i = 0; i < tokens->size; i++) {
      printf("%s\n", tokens->list[i]->leximes);
   }
   printf("Naive count: %d\n", tokens->size);

   tokens = removeEmptyLines(tokens);
   printf("New count: %d\n", tokens->size);
   for (int i = 0; i < tokens->size; i++) {
      printf("%s\n", tokens->list[i]->leximes);
   }

   int count = tokens->size;
   //free them
   deleteTokenList(tokens);


   return count;
}
