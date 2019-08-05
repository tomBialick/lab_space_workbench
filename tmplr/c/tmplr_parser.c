#include "tmplr_parser.h"

//If segfaults or aborts, try increasing INIT_LIST_CAP.
//I think my doubleTokenListCapacity() function is wrong
const int INIT_LIST_CAP = 1000;
const int INIT_TOK_CAP = 10;

/*
 * Creates a token with INIT_TOK_CAP character slots
 */
Token* generateToken() {
  Token* token = malloc(sizeof(Token));
  if (token == NULL){
    printf("Error creating token");
    exit(EXIT_FAILURE);
   }
  token->capacity = INIT_TOK_CAP;
  token->leximes = malloc(token->capacity * sizeof(char));
  if (token->leximes == NULL){
    printf("Error creating token");
    exit(EXIT_FAILURE);
  }

  token->size = 0;
  token->building_flag = 0;
  return token;
}

/*
 * Creates a tokenlist with INIT_LIST_CAP token slots
 */
TokenList* generateTokenList() {
  TokenList* tokens = malloc(sizeof(TokenList));
  if (tokens == NULL){
    printf("Error creating token list");
    exit(EXIT_FAILURE);
  }
  tokens->capacity = INIT_LIST_CAP;
  tokens->list = malloc(tokens->capacity * sizeof(Token));
  if (tokens->list == NULL){
    printf("Error creating token list");
    exit(EXIT_FAILURE);
  }

  tokens->size = 0;
  return tokens;
}

/*
 * Frees a Token* and its contents
 */
void deleteToken(Token* token) {
  free(token->leximes);
  token->capacity = 0;
  token->size = 0;
  free(token);
}

/*
 * Frees a TokenList* and its contents
 */
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
 * Pass in a TokenList to double the capacity
 */
void doubleTokenListCapacity(TokenList* tokens) {
  tokens->capacity = tokens->capacity * 2;
  tokens->list = realloc(tokens->list, (sizeof(Token) * tokens->capacity));
  if (tokens->list == NULL){
    printf("Error expanding token list");
    exit(EXIT_FAILURE);
  }
}

/**
 * Pass in a Token to double its capacity
 */
void doubleTokenCapacity(Token* token) {
  token->capacity = token->capacity * 2;
  token->leximes = realloc(token->leximes, (sizeof(char) * token->capacity));
  if (token == NULL){
    printf("Error expanding token");
    exit(EXIT_FAILURE);
  }
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

/*
 * removes blank lines as well as lines with weird invisible characters
 */
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

/*
 * Removes from // to when a newline is detected
 */
TokenList* removeInlineComments(TokenList* noEmptyLines) {
  TokenList* noInLineComments = generateTokenList();

  for (int i = 0; i < noEmptyLines->size; i++) {
    if ((i < (noEmptyLines->size - 3)) && (strcmp(noEmptyLines->list[i]->leximes, "/") == 0) && (strcmp(noEmptyLines->list[i + 1]->leximes, "/") == 0)) {
      int j = i + 2;
      while ((j < noEmptyLines->size - 2) && (strcmp(noEmptyLines->list[j]->leximes, "\n") != 0)) {
        j++;
      }
      i = j + 1;
    }
    if ((noInLineComments->size + 1) == noInLineComments->capacity) {
      doubleTokenListCapacity(noInLineComments);
    }
    noInLineComments->list[noInLineComments->size] = generateToken();
    while (noInLineComments->list[noInLineComments->size]->capacity <= noEmptyLines->list[i]->size) {
      doubleTokenCapacity(noInLineComments->list[noInLineComments->size]);
    }
    noInLineComments->list[noInLineComments->size]->size = noEmptyLines->list[i]->size;
    memcpy(noInLineComments->list[noInLineComments->size]->leximes, noEmptyLines->list[i]->leximes, noEmptyLines->list[i]->size);
    noInLineComments->size++;
  }
  deleteTokenList(noEmptyLines);
  return noInLineComments;
}

/*
 * removes any and all block comments
 */
TokenList* removeBlockComments(TokenList* noInLineComments) {
  TokenList* noBlockComments = generateTokenList();
  for (int i = 0; i < noInLineComments->size; i++) {
    if ((i < (noInLineComments->size - 4)) && (strcmp(noInLineComments->list[i]->leximes, "/") == 0) && (strcmp(noInLineComments->list[i + 1]->leximes, "*") == 0)) {
      int j = i + 2;
      while ((j < noInLineComments->size - 3) && (strcmp(noInLineComments->list[j]->leximes, "*") != 0) && (strcmp(noInLineComments->list[j+1]->leximes, "/") != 0)) {
        j++;
      }
      i = j + 2;
    }
    if ((noBlockComments->size + 1) == noBlockComments->capacity) {
      doubleTokenListCapacity(noBlockComments);
    }
    noBlockComments->list[noBlockComments->size] = generateToken();
    while (noBlockComments->list[noBlockComments->size]->capacity <= noInLineComments->list[i]->size) {
      doubleTokenCapacity(noBlockComments->list[noBlockComments->size]);
    }
    noBlockComments->list[noBlockComments->size]->size = noInLineComments->list[i]->size;
    memcpy(noBlockComments->list[noBlockComments->size]->leximes, noInLineComments->list[i]->leximes, noInLineComments->list[i]->size);
    noBlockComments->size++;
  }
  deleteTokenList(noInLineComments);
  return noBlockComments;
}

/**
 * Consolidates tokens together that shouldn't be split up
 */
TokenList* consolidator(TokenList* noComments) {
  TokenList* consolidated = generateTokenList();
  for (int i = 0; i < noComments->size; i++) {
    if ((consolidated->size + 1) == consolidated->capacity) {
      doubleTokenListCapacity(consolidated);
    }

    if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i+1]->leximes, "=") == 0) &&
      ((strcmp(noComments->list[i]->leximes, "=") == 0) || (strcmp(noComments->list[i]->leximes, "!") == 0) ||
       (strcmp(noComments->list[i]->leximes, "*") == 0) || (strcmp(noComments->list[i]->leximes, "+") == 0) ||
       (strcmp(noComments->list[i]->leximes, "-") == 0) || (strcmp(noComments->list[i]->leximes, "/") == 0) ||
       (strcmp(noComments->list[i]->leximes, "<") == 0) || (strcmp(noComments->list[i]->leximes, ">") == 0) ||
       (strcmp(noComments->list[i]->leximes, "%") == 0) || (strcmp(noComments->list[i]->leximes, "^") == 0) ||
       (strcmp(noComments->list[i]->leximes, "&") == 0) || (strcmp(noComments->list[i]->leximes, "|") == 0))) {

      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, ">") == 0) && (strcmp(noComments->list[i + 1]->leximes, ">") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "<") == 0) && (strcmp(noComments->list[i + 1]->leximes, "<") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 3)) && (strcmp(noComments->list[i]->leximes, ">") == 0) && (strcmp(noComments->list[i + 1]->leximes, ">") == 0) && (strcmp(noComments->list[i]->leximes, "=") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 4;
      char dubLexTok[4] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], noComments->list[i+2]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 4);
      consolidated->size++;
      i += 2;
    }
    else if ((i < (noComments->size - 3)) && (strcmp(noComments->list[i]->leximes, "<") == 0) && (strcmp(noComments->list[i + 1]->leximes, "<") == 0) && (strcmp(noComments->list[i]->leximes, "=") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 4;
      char dubLexTok[4] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], noComments->list[i+2]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 4);
      consolidated->size++;
      i += 2;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "!") == 0) &&
      (strcmp(noComments->list[i + 1]->leximes, "=") == 0)) {
        consolidated->list[consolidated->size] = generateToken();
        consolidated->list[consolidated->size]->size = 3;
        char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
        memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
        consolidated->size++;
        i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "&") == 0) && (strcmp(noComments->list[i + 1]->leximes, "&") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "|") == 0) && (strcmp(noComments->list[i + 1]->leximes, "|") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "+") == 0) && (strcmp(noComments->list[i + 1]->leximes, "+") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else if ((i < (noComments->size - 2)) && (strcmp(noComments->list[i]->leximes, "-") == 0) && (strcmp(noComments->list[i + 1]->leximes, "-") == 0)) {
      consolidated->list[consolidated->size] = generateToken();
      consolidated->list[consolidated->size]->size = 3;
      char dubLexTok[3] = {noComments->list[i]->leximes[0], noComments->list[i+1]->leximes[0], '\0'};
      memcpy(consolidated->list[consolidated->size]->leximes, dubLexTok, 3);
      consolidated->size++;
      i++;
    }
    else {
      consolidated->list[consolidated->size] = generateToken();
      while (consolidated->list[consolidated->size]->capacity <= noComments->list[i]->size) {
        doubleTokenCapacity(consolidated->list[consolidated->size]);
      }
      consolidated->list[consolidated->size]->size = noComments->list[i]->size;
      memcpy(consolidated->list[consolidated->size]->leximes, noComments->list[i]->leximes, noComments->list[i]->size);
      consolidated->size++;
    }
  }
  deleteTokenList(noComments);
  return consolidated;
}

/**
 * separates into tokens with interpretation, returns the parsed struct
 */
TokenList* parse(FILE* file) {
   TokenList* tokens = generateTokenList();
   naiveParse(file, tokens);
   tokens = removeEmptyLines(tokens);
   tokens = removeInlineComments(tokens);
   tokens = removeBlockComments(tokens);
   tokens = removeEmptyLines(tokens);
   tokens = consolidator(tokens);

   return tokens;
}
