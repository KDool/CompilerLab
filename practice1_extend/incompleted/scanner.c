  /* Scanner
  * @copyright (c) 2008, Hedspi, Hanoi University of Technology
  * @author Huu-Duc Nguyen
  * @version 1.0
  */

  #include <stdio.h>
  #include <stdlib.h>

  #include "reader.h"
  #include "charcode.h"
  #include "token.h"
  #include "error.h"


  extern int lineNo;
  extern int colNo;
  extern int currentChar;

  extern CharCode charCodes[];

  /***************************************************************/

  void skipBlank() {
    readChar();
    // TODO
  }

  void skipComment() {
    // TODO
    while (1)
    {
      /* code */
      if (currentChar == EOF) { // End of File
        error(ERR_ENDOFCOMMENT, lineNo, colNo); // Comment end with EOF
      }
      readChar();
      if (currentChar != EOF && charCodes[currentChar] == CHAR_TIMES){
        readChar();         // get next character
        if (currentChar == EOF && charCodes[currentChar] != CHAR_SLASH) { // End of File
          error(ERR_ENDOFCOMMENT, lineNo, colNo); // Comment end with EOF
          }
        else if(charCodes[currentChar] == CHAR_SLASH) break;
          
      }
    }
    readChar();
  }

Token* readIdentKeyword(void) {
  // TODO
  int length = 0;
  Token *token = makeToken(TK_IDENT, lineNo, colNo);
  while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_UNDERSCORE) {
    token->string[length++] = currentChar;
    // if (length > 15)  // MAX_IDENT_LEN = 15 
    //   error(ERR_IDENTTOOLONG, token->lineNo, token->colNo);
    readChar();
  }
  token->string[length] = '\0';
  
  TokenType tokenType = checkKeyword(token->string);
  if (tokenType != TK_NONE)  // is a keyword  or a Identifier
    token->tokenType = tokenType;
  return token;
}


  Token* readNumber(void) {
    // TODO
    Token *token;
    token = makeToken(TK_NUMBER,lineNo,colNo);
    int count = 0;
    while(charCodes[currentChar]== CHAR_DIGIT){
      token->string[count++] = (char)currentChar;
      readChar();
    }
    token->string[count] = '\0';
    token->value = atoi(token->string);  // String to Integer
    long check_int =  atol(token->string); // String to Long 
    // printf("COUNT: %d\n",count);
    // printf("TOKEN VALUE: %d\n",token->value);
    if (count>10){
      error(ERR_NUMBERTOOLONG,lineNo,colNo);
      return NULL;
    }
    if (count == 10){
      // printf("CHECK INT: %d\n",check_int);
      if (check_int >2147483647 || check_int < 0){
        // long cc= check_int-2147483647;
        // printf("CC is: %ld\n",cc);
        error(ERR_NUMBERTOOLONG,lineNo,colNo);
        return NULL;
      }
      else return token;
    }
    return token;

  }

  Token* readConstChar(void) {
    // TODO
    Token *token;
    int i = 0, ln, cn;
    ln = lineNo; cn = colNo;
    token = makeToken(TK_CHAR, lineNo, colNo);
    readChar();

    token->string[i++] = (char)currentChar;
    readChar();
    if(charCodes[currentChar] != CHAR_SINGLEQUOTE){
      error(ERR_INVALIDCHARCONSTANT, ln, cn);
    }
    readChar();
    return token;
  }

  Token* getToken(void) {
    Token *token;
    int ln, cn;

    if (currentChar == EOF) 
      return makeToken(TK_EOF, lineNo, colNo);

    switch (charCodes[currentChar]) {
    case CHAR_SPACE: skipBlank(); return getToken();
    case CHAR_LETTER: return readIdentKeyword();
    case CHAR_UNDERSCORE: return readIdentKeyword();
    case CHAR_DIGIT: return readNumber();
    case CHAR_PLUS: 
      ln = lineNo;
      cn = colNo;
      token = makeToken(SB_PLUS, ln, cn);
      readChar(); 
      return token;
      case CHAR_MINUS:
      ln = lineNo;
      cn = colNo;
      token = makeToken(SB_MINUS, ln, cn);
      readChar();
      return token;
    case CHAR_TIMES:
      ln = lineNo;
      cn = colNo;
      token = makeToken(SB_TIMES, ln, cn);
      readChar();
      return token;
    case CHAR_SLASH:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_TIMES){
        skipComment();
        return getToken();
      }
      else{
        return makeToken(SB_LPAR, ln, cn);
        token = makeToken(SB_SLASH, ln, cn);        
      }
      return token;
    case CHAR_LT:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        readChar();
        return makeToken(SB_LE, ln, cn);
      }
      return makeToken(SB_LT, ln, cn);
    case CHAR_GT:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        readChar();
        return makeToken(SB_GE, ln, cn);
      }
      return makeToken(SB_GT, ln, cn);
    case CHAR_EXCLAIMATION:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        readChar();
        return makeToken(SB_NEQ, ln, cn);
      }
      error(ERR_INVALIDSYMBOL, ln, cn);
      return makeToken(TK_NONE, ln, cn);
    case CHAR_EQ:
      token = makeToken(SB_EQ, lineNo, colNo);
      readChar();
      return token;
    case CHAR_COMMA:
      token = makeToken(SB_COMMA, lineNo, colNo);
      readChar();
      return token;
    case CHAR_PERIOD:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_RPAR)
      {
        return makeToken(SB_RSEL, ln, cn);
      }
      return makeToken(SB_PERIOD, ln, cn);  
    case CHAR_COLON:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        readChar();
        return makeToken(SB_ASSIGN, ln, cn);
      }else
        return makeToken(SB_COLON, ln, cn);
    case CHAR_SEMICOLON:
      token = makeToken(SB_SEMICOLON, lineNo, colNo);
      readChar();
      return token;
    case CHAR_SINGLEQUOTE: 
      return readConstChar();
    case CHAR_LPAR:
      ln = lineNo;
      cn = colNo;
      readChar();
    
      return makeToken(SB_LPAR, ln, cn);
    case CHAR_RPAR:
      token = makeToken(SB_RPAR, lineNo, colNo);
      readChar();
      return token;
    case CHAR_LSB:
      ln = lineNo;
      cn = colNo;
      readChar();
      if(currentChar != EOF && charCodes[currentChar] == CHAR_PERIOD)
      {
        return makeToken(SB_LSEL, ln, cn);
      }
      return makeToken(SB_LSB, ln, cn);
    case CHAR_RSB:
      ln = lineNo;
      cn = colNo;
      readChar();
      return makeToken(SB_RSB, ln, cn);

      // ....
      // TODO
      // ....
    default:
      token = makeToken(TK_NONE, lineNo, colNo);
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
      readChar(); 
      return token;
    }
  }


  /******************************************************************/

  void printToken(Token *token) {

    printf("%d-%d:", token->lineNo, token->colNo);

    switch (token->tokenType) {
    case TK_NONE: printf("TK_NONE\n"); break;
    case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
    case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
    case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
    case TK_EOF: printf("TK_EOF\n"); break;

    case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
    case KW_CONST: printf("KW_CONST\n"); break;
    case KW_TYPE: printf("KW_TYPE\n"); break;
    case KW_VAR: printf("KW_VAR\n"); break;
    case KW_INTEGER: printf("KW_INTEGER\n"); break;
    case KW_CHAR: printf("KW_CHAR\n"); break;
    case KW_ARRAY: printf("KW_ARRAY\n"); break;
    case KW_OF: printf("KW_OF\n"); break;
    case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
    case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
    case KW_BEGIN: printf("KW_BEGIN\n"); break;
    case KW_END: printf("KW_END\n"); break;
    case KW_CALL: printf("KW_CALL\n"); break;
    case KW_IF: printf("KW_IF\n"); break;
    case KW_THEN: printf("KW_THEN\n"); break;
    case KW_ELSE: printf("KW_ELSE\n"); break;
    case KW_WHILE: printf("KW_WHILE\n"); break;
    case KW_DO: printf("KW_DO\n"); break;
    case KW_FOR: printf("KW_FOR\n"); break;
    case KW_TO: printf("KW_TO\n"); break;

    case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
    case SB_COLON: printf("SB_COLON\n"); break;
    case SB_PERIOD: printf("SB_PERIOD\n"); break;
    case SB_COMMA: printf("SB_COMMA\n"); break;
    case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
    case SB_EQ: printf("SB_EQ\n"); break;
    case SB_NEQ: printf("SB_NEQ\n"); break;
    case SB_LT: printf("SB_LT\n"); break;
    case SB_LE: printf("SB_LE\n"); break;
    case SB_GT: printf("SB_GT\n"); break;
    case SB_GE: printf("SB_GE\n"); break;
    case SB_PLUS: printf("SB_PLUS\n"); break;
    case SB_MINUS: printf("SB_MINUS\n"); break;
    case SB_TIMES: printf("SB_TIMES\n"); break;
    case SB_SLASH: printf("SB_SLASH\n"); break;
    case SB_LPAR: printf("SB_LPAR\n"); break;
    case SB_RPAR: printf("SB_RPAR\n"); break;
    case SB_LSEL: printf("SB_LSEL\n"); break;
    case SB_RSEL: printf("SB_RSEL\n"); break;
    }
  }

  int scan(char *fileName) {
    Token *token;

    if (openInputStream(fileName) == IO_ERROR)
      return IO_ERROR;

    token = getToken();
    while (token->tokenType != TK_EOF) {
      printToken(token);
      free(token);
      token = getToken();
    }

    free(token);
    closeInputStream();
    return IO_SUCCESS;
  }

  /******************************************************************/

  int main(int argc, char *argv[]) {
    if (argc <= 1) {
      printf("scanner: no input file.\n");
      return -1;
    }

    if (scan(argv[1]) == IO_ERROR) {
      printf("Can\'t read input file!\n");
      return -1;
    }
      
    return 0;
  }



