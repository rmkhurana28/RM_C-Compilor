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
#define MAX_STATEMENTS 256
#define MAX 8192

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
            bool isPrefix;          // true if prefix operator
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

typedef struct symbol{ 
    char var_name[MAX_NAME]; // symbol name
    tokenType type; // symbol type
    bool isArray; // if it is an array
    char arraySize[MAX_NAME]; // ONLY valid if isArray == true
    bool isInitialized; // if variable if initialized
    int scope; // scope of variable
    int blockId; // block id for variables 
} symbol;

typedef enum{
    // basic operations
    ADDR_ASSIGN,
    ADDR_BINOP,
    ADDR_UNOP,

    // control flow
    ADDR_GOTO,
    ADDR_IF_F_GOTO,
    ADDR_IF_T_GOTO,
    ADDR_LABEL,

    // array evaluation
    ADDR_ARRAY_READ,
    ADDR_ARRAY_WRITE,
    
} addrType;

// structure of 3-address code
typedef struct address{
    addrType type;

    union{
        // For ADDR_ASSIGN: x = y
        struct{
            char result[MAX_NAME];
            char arg1[MAX_NAME];
        } assign;

        // For ADDR_BINOP: x = y op z
        struct{
            char result[MAX_NAME];
            char arg1[MAX_NAME];
            char arg2[MAX_NAME];
            char op[MAX_NAME];
        } binop;

        // For ADDR_UNOP: x = op y
        struct{
            char result[MAX_NAME];
            char arg1[MAX_NAME];
            char op[MAX_NAME];          // Added: !, -, +
        } unop;

        // For ADDR_GOTO: goto L1
        struct{
            char target[MAX_NAME];      // Target label
        } goto_stmt;

        // For ADDR_IF_F_GOTO: ifFalse condition goto L1
        struct{
            char condition[MAX_NAME];   // Condition variable
            char target[MAX_NAME];      // Target label
        } if_false;

        // For ADDR_IF_T_GOTO: ifTrue condition goto L1
        struct{
            char condition[MAX_NAME];   // Condition variable
            char target[MAX_NAME];      // Target label
        } if_true;

        // For ADDR_ARRAY_READ: x = arr[i]
        struct{
            char result[MAX_NAME];      // Destination variable
            char array[MAX_NAME];       // Array name
            char index[MAX_NAME];       // Index variable/constant
        } array_read;

        // For ADDR_ARRAY_WRITE: arr[i] = x
        struct{
            char array[MAX_NAME];       // Array name
            char index[MAX_NAME];       // Index variable/constant
            char value[MAX_NAME];       // Value to store
        } array_write;

        struct{
            char labelNumber[MAX_NAME];
        } label;
    };    
} address;

// optimizations start from here

// structure for blocks
typedef struct block{
    int blockID;

    address* list[MAX];
    int numberOfAddressesInBlock;

    struct block* cfg_out[MAX];
    int numCFGOut;

    struct block* cfg_in[MAX];
    int numCFGIn;
} block;

// block storage
extern block* allBlocks[MAX];
extern int block_count;

// structure for GEN/KILL/IN/OUT
typedef struct blockProp{
    address* gen[MAX_STATEMENTS];
    int numGen;

    address* kill[MAX_STATEMENTS];
    int numKill;

    address* in[MAX_STATEMENTS];
    int numIn;

    address* out[MAX_STATEMENTS];
    int numOut;
} blockProp;

// storage for GEN/KILL/IN/OUT
extern blockProp* allBlockProps[MAX];
extern int block_prop_count;

// Structure for x86-64 assembly instructions
typedef struct {
    char instruction[256];
} AsmInstruction;

extern address* allAddress[MAX];
extern int addr_count; 

// Assembly code storage (pseudo-assembly)
extern AsmInstruction* asmCode[MAX];
extern int asm_count;

// Real assembly code storage (fully assemblable)
extern AsmInstruction* realAsmCode[MAX];
extern int real_asm_count;

// symbol table 
extern symbol* symbolTable[MAX];

// number of symbols in symbol table
extern int symbol_count;

// Global 
extern Token* tokens[MAX];
extern int token_count;

extern ASTNode* all_ast[MAX];
extern int ast_count;

extern int ast_current_index;

extern const char* tokenTypeNames[];

extern char* all_warnings[MAX];
extern int warning_count;

// fucntions declarations

// helper functions
bool isStringSame(char *str1 , char *str2);

// generate tokens
void generateTokens(char* file_name);

// generate AST
void generateAllASTNodes();

// semantic check
void doSemanticCheck();

// print tokens
void printAllTokens(void);

// print AST
void printASTNode(ASTNode* node, int indent);
void printAllASTNodes();

void printSymbol(symbol* s, int index);
void printSymbolTable();

// intermediate code generation
void startICG();
void print3AddressCode();

void startOptimization();

// print basic blocks (optimization)
void printBlocks();

// target code generation (x86-64)
void generateTargetCode();
void printTargetCode();

// real target code generation (fully assemblable x86-64)
void generateRealTargetCode();
void printRealTargetCode();


#endif

