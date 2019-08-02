#include "tmplr_parser.h"
// #include <string.h>

const int ARG_COUNT = 3;

/**
 * For now, call on the command line with -f <file name>
 */
int main(int argc, char* argv[]) {
  //Verify expected number of args
  if (argc != ARG_COUNT) {
    printf("Unexpected amount of arguments\n");
    exit(EXIT_FAILURE);
  }

  char* filename;
  FILE* file;

  //Cycle through args and store them in the proper variables
  for (int i = 1; i < argc; i++) {
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

   TokenList* tokens = parse(file);
   printf("Actual token count: %d\n", tokens->size);

   // for (int i = 0; i < tokens->size; i++) {
   //    printf("%s\n", tokens->list[i]->leximes);
   // }

   //free them
   deleteTokenList(tokens);
   fclose(file);

   return 0;
}
