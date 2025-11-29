/**
 * database.c - Utility Functions and Data Structure Operations
 * 
 * This module provides utility functions for managing and manipulating
 * the core data structures used throughout the compiler.
 * 
 * Author: Ridham Khurana
 */

#include "database.h"

// ============================================================================
// GLOBAL VARIABLE DEFINITIONS
// ============================================================================

// Lexical Analysis
Token* tokens[MAX];              // Array of all tokens from the lexer
int token_count = 0;             // Current number of tokens

// Syntax Analysis  
ASTNode* all_ast[MAX];           // Array of all AST nodes
int ast_count = 0;               // Current number of AST nodes
int ast_current_index = 0;       // Index for AST traversal/construction

// Semantic Analysis
symbol* symbolTable[MAX];        // Symbol table for variables and arrays
int symbol_count = 0;            // Number of symbols in the table

// Warnings
char* all_warnings[MAX];         // Array of warning messages
int warning_count = 0;           // Number of warnings generated

// Intermediate Code (Three-Address Code)
address* allAddress[MAX];        // Array of all TAC instructions
int addr_count = 0;              // Number of TAC instructions

// Optimization Data Structures
block* allBlocks[MAX];           // Array of basic blocks for CFG
int block_count = 0;             // Number of basic blocks

blockProp* allBlockProps[MAX];   // Reaching definitions analysis data
int block_prop_count = 0;        // Number of block properties

blockPropLive* allBlockPropsLive[MAX]; // Live variable analysis data
int block_prop_live_count = 0;   // Number of live variable properties

// Token type name lookup table for display purposes
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

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * isStringSame - Compare two null-terminated strings for equality
 * 
 * @param str1: First string to compare
 * @param str2: Second string to compare
 * @return: true if strings are identical, false otherwise
 */
bool isStringSame(char *str1, char *str2){
    // Compare characters until end of either string
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2) return false;  // Mismatch found
        
        str1++;
        str2++;
    }

    // Check if both strings ended at the same position
    if(*str1 != '\0' || *str2 != '\0') return false;  // Length mismatch

    return true;  // Strings are identical
}


