// rm 129
// Date: 2025-10-07 20:00

#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NAME 32
#define MINI_MAX 128
#define MAX 1024

// tokenType enum
typedef enum{
    // int, char, double , bool
    KEYWORD_INT, // int
    KEYWORD_CHAR, // char
    KEYWORD_DOUBLE, // double
    KEYWORD_BOOL, // bool
    
    // if, else, while, for
    KEYWORD_IF, // if
    KEYWORD_ELSE, // else
    KEYWORD_WHILE, // while
    KEYWORD_FOR, // for

    // arithematic operator
    OP_PLUS, // +
    OP_MINUS, // -
    OP_MUL, // *
    OP_DIV, // /
    OP_EQUAL, // =
    OP_IS_EQUAL, // ==
    OP_NOT_EQUAL, // !=
    OP_LESS_THAN, // <
    OP_GREAT_THAN, // >
    OP_LESS_EQUAL, // <=
    OP_GREAT_EQUAL, // >=
    OP_AND_AND, // &&
    OP_OR_OR, // ||

    // unary operators
    OP_NOT, // !
    OP_PLUS_PLUS, // ++
    OP_MINUS_MINUS, // --
    
    // punctuations / delimeter
    FULL, // .
    SEMI, // ;
    COMMA, // ,
    L_PARAN, // (
    R_PARAN, // )
    L_BRACES, // {
    R_BRACES, // }
    L_BRACK, // [
    R_BRACK, // ]

    // variables
    ID,

    // absolute values
    VAL_INT, // 5 , 0
    VAL_DOUBLE, // 3.4 , 0.0
    VAL_CHAR, // 't'
    VAL_STRING, // "word"
    VAL_BOOL, // true/false

    // misc
    MISC_EOF, // EOF
    MISC_UNKNOWN, // unknown
    
}tokenType;

// token structure
typedef struct{
    tokenType token_type;
    union{
        int int_value; // used if token_type == VAL_INT
        double double_value; // used if token_type == VAL_DOUBLE
        bool bool_value; // used if token_type == VAL_BOOL
        char var_name[32]; // used if token_type == anything else
    };
    
} Token;

// ASTNodeType enum
typedef enum {
    // Program / Root
    AST_PROGRAM,        // Root node containing all top-level statements or declarations

    // Blocks / Statements
    AST_BLOCK,          // Block of statements: { ... }
    AST_ASSIGN,         // Assignment statement: var = expr
    AST_IF,             // if(condition) { ... }
    AST_IF_ELSE,        // if(condition) { ... } else { ... }
    AST_WHILE,          // while(condition) { ... }
    AST_FOR,            // for(init; condition; update) { ... }

    // Expressions / Variables
    AST_VAR,            // Variable reference
    AST_ARRAY_ACCESS,   // Array element access: arr[index]
    AST_BINOP,          // Binary operation: +, -, *, /, &&, ||, <, >, ==
    AST_UNOP,           // Unary operation: -, !

    // Literals / Constants
    AST_NUM,            // Integer literal
    AST_DOUBLE,         // Double literal
    AST_BOOL,           // Boolean literal (true/false)
    AST_CHAR,           // Character literal
    AST_STRING,         // String

    AST_DECL,           // declarations
} ASTNodeType;




// ASTNode structure
typedef struct ASTNode {
    ASTNodeType type;   // Type of this node

    union {
        // Literal values
        int int_value;           // AST_NUM
        double double_value;     // AST_DOUBLE
        int bool_value;          // AST_BOOL
        char char_value;         // AST_CHAR
        char string_value[MAX_NAME]; // AST_STRING

        // Variable
        struct {
            char var_name[MAX_NAME]; // Variable name
        } var;                   // AST_VAR

        // Array access
        struct {
            char array_name[MAX_NAME];  // Array name
            struct ASTNode* sizeExpr;      // Index expression
        } array_access;                 // AST_ARRAY_ACCESS

        // Unary operation
        struct {
            tokenType op;            // Operator: TOKEN_MINUS, TOKEN_NOT
            struct ASTNode* expr;    // Operand
        } unop;                       // AST_UNOP

        // Binary operation
        struct {
            tokenType op;            // Operator: TOKEN_PLUS, TOKEN_MUL, etc.
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;                      // AST_BINOP

        // Assignment
        struct {
            struct ASTNode* var;     // AST_VAR or AST_ARRAY_ACCESS
            struct ASTNode* expr;    // Expression
        } assign;                     // AST_ASSIGN

        // If statement
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
        } if_stmt;                    // AST_IF

        // If-Else statement
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_else_stmt;               // AST_IF_ELSE

        // While loop
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;                 // AST_WHILE

        // For loop
        struct {
            struct ASTNode* init;       // Usually AST_ASSIGN
            struct ASTNode* condition;  // Expression
            struct ASTNode* update;     // Usually AST_ASSIGN or AST_UNOP
            struct ASTNode* body;       // Body block
        } for_stmt;                     // AST_FOR

        // Block of statements
        struct {
            struct ASTNode** statements; // Array of pointers to ASTNodes
            int statement_count;              // Number of statements
        } block;                          // AST_BLOCK

        // Program root
        struct {
            struct ASTNode** statements; // Array of top-level statements
            int statement_count;              // Number of statements
        } program;                        // AST_PROGRAM

        // declarations
        struct {
            char var_name[MAX_NAME]; // variable name
            tokenType type;          // base type
            struct ASTNode* init_expr;      // optional initializer
            bool is_array;           // true if array
            char array_size[MAX_NAME];          // valid only if is_array == true
        } decl;

    };
} ASTNode;

// Global 
extern Token* tokens[MAX];
extern int token_count;

extern ASTNode* all_ast[MAX];
extern int ast_count;

extern int ast_current_index;

extern const char* tokenTypeNames[];

// fucntions declarations

// helper functions
bool isStringSame(char *str1 , char *str2);

// generate tokens
void generateTokens(char* file_name);

void generateAllASTNodes();


#endif

