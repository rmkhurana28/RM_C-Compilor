#include "database.h"

// Define globals
Token* tokens[MAX];
int token_count = 0;

ASTNode* all_ast[MAX];
int ast_count = 0;

int ast_current_index = 0;

// symbol table 
symbol* symbolTable[MAX];

// number of symbols in symbol table
int symbol_count = 0;

char* all_warnings[MAX];
int warning_count = 0;

const char* tokenTypeNames[] = {
    "KEYWORD_INT", "KEYWORD_CHAR", "KEYWORD_DOUBLE", "KEYWORD_BOOL",
    "KEYWORD_IF", "KEYWORD_ELSE", "KEYWORD_WHILE", "KEYWORD_FOR",
    "OP_PLUS", "OP_MINUS", "OP_MUL", "OP_DIV", "OP_EQUAL",
    "OP_IS_EQUAL", "OP_NOT_EQUAL", "OP_LESS_THAN", "OP_GREAT_THAN",
    "OP_LESS_EQUAL", "OP_GREAT_EQUAL", "OP_AND_AND", "OP_OR_OR", "OP_NOT","OP_PLUS_PLUS","OP_MINUS_MINUS","FULL",
    "SEMI", "COMMA", "L_PARAN", "R_PARAN", "L_BRACES", "R_BRACES",
    "L_BRACK", "R_BRACK",
    "ID",
    "VAL_INT", "VAL_DOUBLE", "VAL_CHAR", "VAL_STRING",  "VAL_BOOL",
    "MISC_EOF", "MISC_UNKNOWN"
};

// define functions

bool isStringSame(char *str1 , char *str2){
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2) return false;
        
        str1++;
        str2++;
    }

    if(*str1 != '\0' || *str2 != '\0') return false;

    return true;
}


