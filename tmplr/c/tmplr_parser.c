#include <string.h>
#include "tmplr_parser.h"

const int INIT_LIST_CAP = 100;
const int INIT_TOK_CAP = 100;


/**
 * Pass in a string array and the current capacity to double the capacity
 */
int doubleTokenListCapacity(char*** tokenList, int currSize) {
  int newCap = currSize * 2;
  **tokenList = realloc(**tokenList, (sizeof(char*) * newCap));
  return newCap;
}

/**
 * Pass in a string and the capacity to double its capacity
 */
int doubleTokenSize(char** currToken, int currSize) {
  int newCap = currSize * 2;
  *currToken = realloc(*currToken, (sizeof(char) * newCap));
  return newCap;
}

/**
 * separates into tokens without much interpretation, returns total amount
 */
int naiveParse(FILE* file, char** naiveTokens, int* capacity) {

   int tokenCap = INIT_TOK_CAP;

   char* token = malloc(sizeof(char) * tokenCap);
   int tokenCount = 0;
   int token_building_flag = 0;
   int lexCount = 0;
   char lex = getc(file);

   while (lex != EOF) {
     switch (lex) {
       case ' ':
       case '\n':
       case '\t':
         token[lexCount] = '\0';
         if (token_building_flag == 1) {
           if ((tokenCount + 1) == *capacity) {
             *capacity = doubleTokenListCapacity(&naiveTokens, *capacity);
           }
           naiveTokens[tokenCount] = malloc(sizeof(char) * (lexCount + 1));
           memcpy(naiveTokens[tokenCount], token, (lexCount + 1));
           tokenCount++;
           token_building_flag = 0;

           //refresh token
           lexCount = 0;
           int tokenCap = INIT_TOK_CAP;
           free(token);
           token = malloc(sizeof(char) * tokenCap);

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
         if (token_building_flag == 1) {
           if ((tokenCount + 1) == *capacity) {
             *capacity = doubleTokenListCapacity(&naiveTokens, *capacity);
           }
           token[lexCount] = '\0';
           naiveTokens[tokenCount] = malloc(sizeof(char) * (lexCount + 1));
           memcpy(naiveTokens[tokenCount], token, (lexCount + 1));
           tokenCount++;
           token_building_flag = 0;

           //refresh token
           lexCount = 0;
           int tokenCap = INIT_TOK_CAP;
           free(token);
           token = malloc(sizeof(char) * tokenCap);
         }
         naiveTokens[tokenCount] = malloc(sizeof(char) * (1 + 1));
         char singleLexTok[2] = {lex, '\0'};
         memcpy(naiveTokens[tokenCount], singleLexTok, 2);
         tokenCount++;
         break;
       default:
         token_building_flag = 1;
         if ((lexCount + 1) == tokenCap) {
           tokenCap = doubleTokenSize(&token, tokenCap);
         }
         token[lexCount] = lex;
         lexCount++;
         break;
     }

     lex = getc(file);
   }
   free(token);
   return tokenCount;
}



/**
 * separates into tokens with interpretation, returns total amount
 */
int parse(FILE* file) {

   int* capacity = &INIT_LIST_CAP;
   char** naiveTokens = malloc(sizeof(char*) * (*capacity));
   int naiveCount = naiveParse(file, naiveTokens, capacity);

   //print naiveTokens to the console
   for (int i = 0; i < naiveCount; i++) {
     if (naiveTokens[i]) {
        printf("%s\n", naiveTokens[i]);
     }
   }
   for (int i = 0; i < naiveCount; i++) {
        free(naiveTokens[i]);
   }
   printf("147?\n");
   printf("%d\n", *capacity);
   if (naiveTokens) {
     printf("Free it!\n");
     free(naiveTokens);
   }
   printf("151?\n");


   return naiveCount;
}
