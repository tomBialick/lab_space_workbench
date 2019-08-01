#include <string.h>
#include "tmplr_parser.h"

const int INIT_LIST_CAP = 100;
const int INIT_TOK_CAP = 100;

/**
 * Pass in a string array and the current capacity to double the capacity
 */
int doubleTokenListCapacity(char** tokenList, int currSize) {
  int newCap = currSize * 2;
  *tokenList = realloc(*tokenList, (sizeof(char*) * newCap));
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
int naiveParse(FILE* file, char** naiveTokens, int cap) {

   int tokenCap = INIT_TOK_CAP;
   int capacity = cap;

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
         //Finish current token
         if (token_building_flag == 1) {
           token[lexCount] = '\0';
           if ((tokenCount + 1) == capacity) {
             capacity = doubleTokenListCapacity(naiveTokens, capacity);
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
         //Ignore extra space
         if (lex == '\n') {
           naiveTokens[tokenCount] = malloc(sizeof(char) * (1 + 1));
           char singleLexTok[2] = {lex, '\0'};
           memcpy(naiveTokens[tokenCount], singleLexTok, 2);
           tokenCount++;
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
         if (token_building_flag == 1) {
           token[lexCount] = '\0';
           if ((tokenCount + 1) == capacity) {
             capacity = doubleTokenListCapacity(naiveTokens, capacity);
           }
           naiveTokens[tokenCount] = malloc(sizeof(char) * (lexCount + 1));
           memcpy(naiveTokens[tokenCount], token, (lexCount + 1));
           tokenCount++;
           token_building_flag = 0;

           //refresh token
           free(token);
           lexCount = 0;
           int tokenCap = INIT_TOK_CAP;
           token = malloc(sizeof(char) * tokenCap);
         }
         naiveTokens[tokenCount] = malloc(sizeof(char) * (1 + 1));
         char singleLexTok[2] = {lex, '\0'};
         memcpy(naiveTokens[tokenCount], singleLexTok, 2);
         tokenCount++;
         break;
       default:
         token_building_flag = 1;
         if ((lexCount + 2) == tokenCap) {
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

// char** removeExtraLines(char** naiveTokens, int naiveCount) {
//
// }



/**
 * separates into tokens with interpretation, returns total amount
 */
int parse(FILE* file) {
   int capacity = INIT_LIST_CAP;
   char** naiveTokens = malloc(sizeof(char*) * (capacity));
   int naiveCount = naiveParse(file, naiveTokens, capacity);

   //print naiveTokens to the console
   for (int i = 0; i < naiveCount; i++) {
      printf("%s\n", naiveTokens[i]);
   }

   for (int i = 0; i < naiveCount; i++) {
        free(naiveTokens[i]);
   }

   // For some reason, this causes a seg fault. Might be already freed?
   // if (naiveTokens) {
   //   free(naiveTokens);
   // }


   return naiveCount;
}
