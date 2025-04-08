/* front.c - a lexical analyzer system for simple arithmetic expressions */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
int charClass;     // Stores the character type
char lexeme[100];  // Holds the current lexeme
char nextChar;     // Holds the next character read from the file
int lexLen;        // Length of the current lexeme
int token;         // Stores the current token type
int nextToken;     // Stores the token to be processed next
FILE* in_fp;       // Pointer to the input file

/* Function declarations */
void addChar();    // Adds the current character to the lexeme
void getChar();    // Reads the next character and determines its type
void getNonBlank(); // Skips over whitespace in the input
int lex();         // The main lexical analyzer function
int lookup(char ch); // Recognizes symbols and returns their token type

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10      // integers
#define IDENT 11        // identifier 
#define ASSIGN_OP 20    // =
#define ADD_OP 21       // +
#define SUB_OP 22       // -
#define MULT_OP 23      // *
#define DIV_OP 24       // /
#define LEFT_PAREN 25   // (
#define RIGHT_PAREN 26  // )
#define GT_COMPARISON 12 // >
#define EOF_TOKEN -1    // end of file

/* main driver */
int main() {
    char filename[100];

    /* Get filename from user */
    printf("Enter the input file name: ");
    scanf("%99s", filename);

    /* Open the input data file */
    in_fp = fopen(filename, "r");
    if (in_fp == NULL) {
        printf("ERROR - cannot open %s\n", filename);
        return 1;
    }

    /* Process the file contents */
    getChar(); // Read the first character
    do {
        lex();  // Call the lexical analyzer until end of file
    } while (nextToken != EOF_TOKEN);

    fclose(in_fp);
    return 0;
}

/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case '>':
        addChar();
        nextToken = GT_COMPARISON;
        break;
    default:
        addChar();
        nextToken = EOF_TOKEN;
        break;
    }
    return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {  // Prevents buffer overflow
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0'; // Null-terminate the string
    }
    else {
        printf("Error - lexeme is too long\n");
    }
}

/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;  // Symbols or other characters
    }
    else {
        charClass = EOF_TOKEN; // End of file
    }
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();  // Keep reading until a non-space character is found
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    memset(lexeme, 0, sizeof(lexeme)); // Clear lexeme before building a new one

    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;

        /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

        /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

        /* End of file */
    case EOF_TOKEN:
        nextToken = EOF_TOKEN;
        strcpy(lexeme, "EOF");
        break;
    }

    /* Print the token and its corresponding lexeme */
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}