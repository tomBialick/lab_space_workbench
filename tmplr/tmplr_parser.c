#include <stdio.h>
#include <stdlib.h>

const int ARG_COUNT = 2;
const int INIT_LIST_CAP = 100;
const int INIT_TOK_CAP = 100;


int doubleTokenListCapacity(char*** tokenList, int currSize) {
  int newCap = currSize * 2;
  char** newList = malloc(sizeof(char*) * newCap);

  for (int i = 0; i < currSize; i++) {
    newList[i] = tokenList[i];
  }

  free(**tokenList);
  **tokenList = newList;
  return newCap;
}

int doubleTokenSize(char** currToken, int currSize) {
  int newCap = currSize * 2;
  char* newToken = malloc(sizeof(char) * newCap);

  for (int i = 0; i < currSize; i++) {
    newToken[i] = currToken[i];
  }

  free(*currToken);
  *currToken = newList;
  return newCap;
}

/**
 *
 */
int main(int argc, char* argv[]) {
  //Verify expected number of args
  if (argc != ARG_COUNT) {
    printf("Unexpected amount of arguments\n", );
    exit(EXIT_FAILURE);
  }

  char* filename;
  FILE *file;

  //Cycle through args and store them in the proper variables
  for (int i = 0; i < argc; i++) {
    if (i + 1 < argc) {
      if (strcmp(argv[i], "-f") == 0) {
        filename = argv[i + 1];
        i++;
      }
    }
  }

  file = fopen(filename,"r");

  if (file == NULL){
       printf("Error opening file");
       // Program exits if the file pointer returns NULL.
       exit(EXIT_FAILURE);
   }

   int capacity = INIT_LIST_CAP;
   int tokenCap = INIT_TOK_CAP;
   char** tokens = malloc(sizeof(char*) * capacity);
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
         if (token_building_flag == 1) {
           if ((tokenCount + 1) == capacity) {
             capacity = doubleTokenListCapacity(&tokens, capacity);
           }
           tokenCount++;
           tokens[tokenCount] = token;

           //refresh token
           lexCount = 0;
           int tokenCap = INIT_TOK_CAP;
           free(token);
           *token = malloc(sizeof(char) * tokenCap);

         }
         break;
       default:
         if ((lexCount + 1) == tokenCap) {
           tokenCap = doubleTokenSize(&token, tokenCap);
         }
         lexCount++;
         token[lexCount] = lex;
         break;
     }

     lex = getc(file);
   }

   free(token);
   free(tokens);
   fclose(file);
   return 0;
}
