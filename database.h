/**
 * database.h - Core Data Structures and Declarations
 * 
 * Central header file containing all data structures, type definitions,
 * and function declarations used across the compiler.
 * 
 * Includes:
 * - Token definitions and structures
 * - AST node types and structures
 * - Symbol table structures
 * - Three-address code (TAC) structures
 * - Basic block and CFG structures
 * - Global constants and limits
 * 
 * Author: Ridham Khurana
 */

#ifndef DATABASE_H
#define DATABASE_H

// Standard library includes
#include <stdio.h>      // File I/O operations
#include <stdlib.h>     // Memory allocation, conversions
#include <string.h>     // String manipulation
#include <math.h>       // Math operations for floating-point
#include <stdbool.h>    // Boolean type support
#include <ctype.h>      // Character classification

// ============================================================================
// CONFIGURATION CONSTANTS
// ============================================================================

#define MAX_NAME 128           // Maximum length for identifiers and names
#define MINI_MAX 128           // Auxiliary buffer size for small strings
#define MAX_STATEMENTS 256     // Maximum statements in a basic block
#define MAX 8192               // Maximum global array sizes (tokens, AST nodes, etc.)

// ============================================================================
// TOKEN DEFINITIONS
// ============================================================================

/**
 * tokenType - Enumeration of all token types recognized by the lexer
 * 
 * Categories:
 * - Keywords: Data types and control flow
 * - Operators: Arithmetic, relational, logical, unary
 * - Delimiters: Punctuation and brackets
 * - Literals: Numeric, boolean, character, string
 * - Identifiers: Variable names
 */
typedef enum{
    // Data type keywords
    KEYWORD_INT,         // int - integer type
    KEYWORD_CHAR,        // char - character type
    KEYWORD_DOUBLE,      // double - floating-point type
    KEYWORD_BOOL,        // bool - boolean type
    
    // Control flow keywords
    KEYWORD_IF,          // if - conditional statement
    KEYWORD_ELSE,        // else - alternative branch
    KEYWORD_WHILE,       // while - loop with precondition
    KEYWORD_FOR,         // for - loop with init/condition/update

    // Binary arithmetic and relational operators
    OP_PLUS,             // + addition
    OP_MINUS,            // - subtraction
    OP_MUL,              // * multiplication
    OP_DIV,              // / division
    OP_EQUAL,            // = assignment
    OP_IS_EQUAL,         // == equality comparison
    OP_NOT_EQUAL,        // != inequality comparison
    OP_LESS_THAN,        // < less than
    OP_GREAT_THAN,       // > greater than
    OP_LESS_EQUAL,       // <= less than or equal
    OP_GREAT_EQUAL,      // >= greater than or equal
    OP_AND_AND,          // && logical AND
    OP_OR_OR,            // || logical OR

    // Unary operators
    OP_NOT,              // ! logical NOT
    OP_PLUS_PLUS,        // ++ increment (prefix/postfix)
    OP_MINUS_MINUS,      // -- decrement (prefix/postfix)
    
    // Delimiters and punctuation
    FULL,                // . period (reserved for future use)
    SEMI,                // ; semicolon (statement terminator)
    COMMA,               // , comma (separator)
    L_PARAN,             // ( left parenthesis
    R_PARAN,             // ) right parenthesis
    L_BRACES,            // { left brace (block start)
    R_BRACES,            // } right brace (block end)
    L_BRACK,             // [ left bracket (array subscript)
    R_BRACK,             // ] right bracket

    // Identifiers
    ID,                  // Variable or function identifier

    // Literal values
    VAL_INT,             // Integer literal (e.g., 42)
    VAL_DOUBLE,          // Floating-point literal (e.g., 3.14)
    VAL_CHAR,            // Character literal (e.g., 'a')
    VAL_STRING,          // String literal (e.g., "hello")
    VAL_BOOL,            // Boolean literal (true/false)

    // Special tokens
    MISC_EOF,            // End of file marker
    MISC_UNKNOWN,        // Unknown/unrecognized token
    
}tokenType;

/**
 * Token - Structure representing a single lexical token
 * 
 * Fields:
 * - token_type: Category of the token
 * - Union containing the token's value (only one field is active):
 *   - int_value: For integer literals
 *   - double_value: For floating-point literals  
 *   - bool_value: For boolean literals
 * - var_name: For identifiers, keywords, operators, strings
 * 
 * Note: The union saves memory by overlapping storage - only the field
 * corresponding to token_type should be accessed.
 */
typedef struct{
    tokenType token_type;    // Category of this token
    union{
        int int_value;              // Used when token_type == VAL_INT
        double double_value;        // Used when token_type == VAL_DOUBLE
        bool bool_value;            // Used when token_type == VAL_BOOL
        char var_name[32];          // Used for IDs, keywords, operators, strings
    };
    
} Token;

// ============================================================================
// ABSTRACT SYNTAX TREE (AST) DEFINITIONS
// ============================================================================

/**
 * ASTNodeType - Enumeration of all AST node types
 * 
 * Categories:
 * - Program structure: Root, blocks
 * - Statements: Assignments, control flow (if, while, for)
 * - Expressions: Variables, arrays, operators
 * - Literals: Numbers, booleans, characters, strings
 * - Declarations: Variable and array declarations
 */
typedef enum {
    // Program structure
    AST_PROGRAM,         // Root node containing all top-level statements

    // Statement types
    AST_BLOCK,           // Block of statements: { ... }
    AST_ASSIGN,          // Assignment statement: var = expr
    AST_IF,              // if(condition) { ... }
    AST_IF_ELSE,         // if(condition) { ... } else { ... }
    AST_WHILE,           // while(condition) { ... }
    AST_FOR,             // for(init; condition; update) { ... }

    // Expression types
    AST_VAR,             // Variable reference
    AST_ARRAY_ACCESS,    // Array element access: arr[index]
    AST_BINOP,           // Binary operation: +, -, *, /, &&, ||, <, >, ==, etc.
    AST_UNOP,            // Unary operation: -, !, ++, --

    // Literal types
    AST_NUM,             // Integer literal
    AST_DOUBLE,          // Double literal
    AST_BOOL,            // Boolean literal (true/false)
    AST_CHAR,            // Character literal
    AST_STRING,          // String literal

    // Declaration
    AST_DECL,            // Variable or array declaration
} ASTNodeType;

/**
 * ASTNode - Structure representing a node in the Abstract Syntax Tree
 * 
 * Each node has a type field and a union containing type-specific data.
 * The active union member depends on the node's type field.
 * 
 * Node types and their corresponding union members:
 * - Literals (AST_NUM, AST_DOUBLE, etc.): Direct value fields
 * - Variables (AST_VAR): var.var_name
 * - Operators (AST_BINOP, AST_UNOP): binop/unop structs
 * - Statements (AST_IF, AST_WHILE, etc.): Statement-specific structs
 * - Blocks (AST_BLOCK, AST_PROGRAM): Array of child statements
 * - Declarations (AST_DECL): decl struct with type and initialization info
 */
typedef struct ASTNode {
    ASTNodeType type;   // Determines which union member is active

    union {
        // Literal values (AST_NUM, AST_DOUBLE, AST_BOOL, AST_CHAR, AST_STRING)
        int int_value;                   // Integer literal value
        double double_value;             // Floating-point literal value
        int bool_value;                  // Boolean value (0 or 1)
        char char_value;                 // Character literal
        char string_value[MAX_NAME];     // String literal

        // Variable reference (AST_VAR)
        struct {
            char var_name[MAX_NAME];     // Name of the variable
        } var;

        // Array element access (AST_ARRAY_ACCESS)
        struct {
            char array_name[MAX_NAME];   // Name of the array
            struct ASTNode* sizeExpr;    // Index expression (can be constant or expression)
        } array_access;

        // Unary operation (AST_UNOP)
        struct {
            tokenType op;                // Operator: OP_MINUS, OP_NOT, OP_PLUS_PLUS, OP_MINUS_MINUS
            struct ASTNode* expr;        // Operand expression
            bool isPrefix;               // true for prefix (++x), false for postfix (x++)
        } unop;

        // Binary operation (AST_BINOP)
        struct {
            tokenType op;                // Operator: OP_PLUS, OP_MUL, OP_LESS_THAN, etc.
            struct ASTNode* left;        // Left operand
            struct ASTNode* right;       // Right operand
        } binop;

        // Assignment statement (AST_ASSIGN)
        struct {
            struct ASTNode* var;         // Target: AST_VAR or AST_ARRAY_ACCESS
            struct ASTNode* expr;        // Value expression
        } assign;

        // If statement without else (AST_IF)
        struct {
            struct ASTNode* condition;   // Boolean condition
            struct ASTNode* then_branch; // Body to execute if true
        } if_stmt;

        // If-Else statement (AST_IF_ELSE)
        struct {
            struct ASTNode* condition;   // Boolean condition
            struct ASTNode* then_branch; // Body to execute if true
            struct ASTNode* else_branch; // Body to execute if false
        } if_else_stmt;

        // While loop (AST_WHILE)
        struct {
            struct ASTNode* condition;   // Loop condition
            struct ASTNode* body;        // Loop body
        } while_stmt;

        // For loop (AST_FOR)
        struct {
            struct ASTNode* init;        // Initialization (usually AST_ASSIGN)
            struct ASTNode* condition;   // Loop condition
            struct ASTNode* update;      // Update expression (usually AST_ASSIGN or AST_UNOP)
            struct ASTNode* body;        // Loop body
        } for_stmt;

        // Block of statements (AST_BLOCK)
        struct {
            struct ASTNode** statements; // Array of pointers to statement nodes
            int statement_count;         // Number of statements in this block
        } block;

        // Program root (AST_PROGRAM)
        struct {
            struct ASTNode** statements; // Array of top-level statements
            int statement_count;         // Number of top-level statements
        } program;

        // Variable or array declaration (AST_DECL)
        struct {
            char var_name[MAX_NAME];     // Name of variable/array
            tokenType type;              // Data type: KEYWORD_INT, KEYWORD_DOUBLE, etc.
            struct ASTNode* init_expr;   // Optional initializer expression (NULL if none)
            bool is_array;               // true if declaring an array
            char array_size[MAX_NAME];   // Array size (valid only if is_array == true)
        } decl;

    };
} ASTNode;

// ============================================================================
// SYMBOL TABLE DEFINITIONS
// ============================================================================

/**
 * symbol - Structure representing a symbol table entry
 * 
 * Tracks all variables and arrays declared in the program with their
 * types, scopes, and initialization status.
 */
typedef struct symbol{ 
    char var_name[MAX_NAME];     // Symbol name (variable or array)
    tokenType type;              // Data type: KEYWORD_INT, KEYWORD_DOUBLE, etc.
    bool isArray;                // true if this is an array
    char arraySize[MAX_NAME];    // Size of array (valid only if isArray == true)
    bool isInitialized;          // true if variable/array was initialized
    int scope;                   // Scope level (0 = global, increases with nesting)
    int blockId;                 // Block ID where this symbol is declared
} symbol;

// ============================================================================
// THREE-ADDRESS CODE (INTERMEDIATE REPRESENTATION) DEFINITIONS
// ============================================================================

/**
 * addrType - Enumeration of three-address code instruction types
 * 
 * Categories:
 * - Basic operations: Assignment, binary ops, unary ops
 * - Control flow: Conditional/unconditional jumps, labels
 * - Array operations: Array element read/write
 */
typedef enum{
    // Basic operations
    ADDR_ASSIGN,         // Simple assignment: x = y
    ADDR_BINOP,          // Binary operation: x = y op z
    ADDR_UNOP,           // Unary operation: x = op y

    // Control flow
    ADDR_GOTO,           // Unconditional jump: goto L
    ADDR_IF_F_GOTO,      // Conditional jump if false: if_false x goto L
    ADDR_IF_T_GOTO,      // Conditional jump if true: if_true x goto L
    ADDR_LABEL,          // Label: L:

    // Array operations
    ADDR_ARRAY_READ,     // Array element read: x = arr[i]
    ADDR_ARRAY_WRITE,    // Array element write: arr[i] = x
    
} addrType;

/**
 * address - Structure representing a three-address code instruction
 * 
 * Three-address code is a linearized intermediate representation where
 * each instruction has at most one operator and three operands.
 * 
 * The type field determines which union member is active.
 * Each instruction type has specific fields for its operands.
 */
typedef struct address{
    addrType type;       // Instruction type (determines active union member)

    union{
        // ADDR_ASSIGN: x = y (copy/move)
        struct{
            char result[MAX_NAME];      // Destination variable
            char arg1[MAX_NAME];        // Source variable or constant
        } assign;

        // ADDR_BINOP: x = y op z (binary operation)
        struct{
            char result[MAX_NAME];      // Destination variable
            char arg1[MAX_NAME];        // Left operand
            char arg2[MAX_NAME];        // Right operand
            char op[MAX_NAME];          // Operator: +, -, *, /, <, >, ==, etc.
        } binop;

        // ADDR_UNOP: x = op y (unary operation)
        struct{
            char result[MAX_NAME];      // Destination variable
            char arg1[MAX_NAME];        // Operand
            char op[MAX_NAME];          // Operator: !, -, +, ++, --
        } unop;

        // ADDR_GOTO: goto L (unconditional jump)
        struct{
            char target[MAX_NAME];      // Target label name
        } goto_stmt;

        // ADDR_IF_F_GOTO: if_false x goto L (conditional jump on false)
        struct{
            char condition[MAX_NAME];   // Condition variable to test
            char target[MAX_NAME];      // Label to jump to if false
        } if_false;

        // ADDR_IF_T_GOTO: if_true x goto L (conditional jump on true)
        struct{
            char condition[MAX_NAME];   // Condition variable to test
            char target[MAX_NAME];      // Label to jump to if true
        } if_true;

        // ADDR_ARRAY_READ: x = arr[i] (read from array)
        struct{
            char result[MAX_NAME];      // Destination variable
            char array[MAX_NAME];       // Array name
            char index[MAX_NAME];       // Index variable or constant
        } array_read;

        // ADDR_ARRAY_WRITE: arr[i] = x (write to array)
        struct{
            char array[MAX_NAME];       // Array name
            char index[MAX_NAME];       // Index variable or constant
            char value[MAX_NAME];       // Value to store
        } array_write;

        // ADDR_LABEL: L: (label definition)
        struct{
            char labelNumber[MAX_NAME]; // Label name/number
        } label;
    };    
} address;

// ============================================================================
// CODE OPTIMIZATION DATA STRUCTURES
// ============================================================================

/**
 * block - Structure representing a basic block in the Control Flow Graph (CFG)
 * 
 * A basic block is a sequence of instructions with:
 * - One entry point (first instruction)
 * - One exit point (last instruction)
 * - No branches except at the end
 * 
 * Basic blocks are connected via cfg_in and cfg_out to form the CFG.
 * The CFG represents all possible execution paths through the program.
 */
typedef struct block{
    int blockID;                         // Unique identifier for this block

    address* list[MAX];                  // Array of instructions in this block
    int numberOfAddressesInBlock;        // Count of instructions

    struct block* cfg_out[MAX];          // Successor blocks (blocks that follow this one)
    int numCFGOut;                       // Number of successors

    struct block* cfg_in[MAX];           // Predecessor blocks (blocks that come before this one)
    int numCFGIn;                        // Number of predecessors
} block;

// Global storage for all basic blocks
extern block* allBlocks[MAX];
extern int block_count;

/**
 * blockProp - Data flow analysis properties for reaching definitions
 * 
 * Used in data flow analysis to track which definitions reach each block.
 * Implements the reaching definitions algorithm:
 * - GEN: Definitions generated in this block
 * - KILL: Definitions killed (overwritten) in this block
 * - IN: Definitions reaching the start of this block
 * - OUT: Definitions reaching the end of this block
 * 
 * The algorithm iteratively computes IN and OUT until convergence:
 * OUT[B] = GEN[B] ∪ (IN[B] - KILL[B])
 * IN[B] = ∪ OUT[P] for all predecessors P of B
 */
typedef struct blockProp{
    address* gen[MAX_STATEMENTS];        // Definitions generated in this block
    int numGen;

    address* kill[MAX_STATEMENTS];       // Definitions killed in this block
    int numKill;

    address* in[MAX_STATEMENTS];         // Definitions reaching block entry
    int numIn;

    address* out[MAX_STATEMENTS];        // Definitions reaching block exit
    int numOut;
} blockProp;

// Global storage for reaching definitions analysis
extern blockProp* allBlockProps[MAX];
extern int block_prop_count;

/**
 * blockPropLive - Data flow analysis properties for live variable analysis
 * 
 * Used to determine which variables are "live" (will be used) at each
 * program point. This information is used for dead code elimination.
 * - GEN: Variables used before being defined in this block
 * - KILL: Variables defined in this block
 * - IN: Variables live at block entry
 * - OUT: Variables live at block exit
 * 
 * Unlike reaching definitions, live variable analysis is a backward
 * data flow problem - we compute from OUT to IN:
 * IN[B] = GEN[B] ∪ (OUT[B] - KILL[B])
 * OUT[B] = ∪ IN[S] for all successors S of B
 */
typedef struct blockPropLive{
    char genLive[MAX_STATEMENTS][MAX_STATEMENTS];  // Variables used in this block
    int numGenLive;

    char killLive[MAX_STATEMENTS][MAX_STATEMENTS]; // Variables defined in this block
    int numKillLive;

    char inLive[MAX_STATEMENTS][MAX_STATEMENTS];   // Variables live at block entry
    int numInLive;

    char outLive[MAX_STATEMENTS][MAX_STATEMENTS];  // Variables live at block exit
    int numOutLive;
} blockPropLive;

// Global storage for live variable analysis
extern blockPropLive* allBlockPropsLive[MAX];
extern int block_prop_live_count;

// ============================================================================
// TARGET CODE GENERATION DATA STRUCTURES
// ============================================================================

/**
 * AsmInstruction - Structure for assembly code instructions
 * 
 * Represents a single line of assembly code (either pseudo or real x86-64).
 */
typedef struct {
    char instruction[256];               // Assembly instruction text
} AsmInstruction;

// ============================================================================
// GLOBAL DATA STORAGE
// ============================================================================

// Three-address code storage
extern address* allAddress[MAX];         // Array of all TAC instructions
extern int addr_count;                   // Number of TAC instructions

// Assembly code storage (pseudo-assembly for educational purposes)
extern AsmInstruction* asmCode[MAX];     // Simplified assembly output
extern int asm_count;                    // Number of pseudo-assembly instructions

// Real assembly code storage (fully assemblable x86-64)
extern AsmInstruction* realAsmCode[MAX]; // Actual x86-64 assembly output
extern int real_asm_count;               // Number of real assembly instructions

// Symbol table storage
extern symbol* symbolTable[MAX];         // Array of all symbols
extern int symbol_count;                 // Number of symbols in table

// Token storage
extern Token* tokens[MAX];               // Array of all tokens from lexer
extern int token_count;                  // Number of tokens

// AST node storage
extern ASTNode* all_ast[MAX];            // Array of all AST nodes
extern int ast_count;                    // Number of AST nodes
extern int ast_current_index;            // Current index during AST construction

// Token type names for display
extern const char* tokenTypeNames[];     // String names for each tokenType enum value

// Warning messages
extern char* all_warnings[MAX];          // Array of warning message strings
extern int warning_count;                // Number of warnings generated

// Output file configuration
extern char output_filename[MAX_NAME];   // Name of the compiler output file

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// --- Utility Functions ---
bool isStringSame(char *str1, char *str2);  // String comparison helper

// --- Phase 1: Lexical Analysis ---
void generateTokens(char* file_name);       // Tokenize source file

// --- Phase 2: Syntax Analysis ---
void generateAllASTNodes();                 // Parse tokens and build AST

// --- Phase 3: Semantic Analysis ---
void doSemanticCheck();                     // Type checking and symbol table construction

// --- Console Output Functions (mostly disabled, see 00_print.c) ---
void printAllTokens(void);                  // Display all tokens
void printASTNode(ASTNode* node, int indent); // Display single AST node
void printAllASTNodes();                    // Display entire AST
void printSymbol(symbol* s, int index);     // Display single symbol
void printSymbolTable();                    // Display entire symbol table

// --- Phase 4: Intermediate Code Generation ---
void startICG();                            // Generate three-address code from AST
void print3AddressCode();                   // Display TAC instructions

// --- Phase 5: Code Optimization ---
void startOptimization();                   // Run optimization passes on TAC
void printBlocks();                         // Display basic blocks and CFG

// --- Phase 6: Target Code Generation ---
void generateTargetCode();                  // Generate pseudo-assembly code
void printTargetCode();                     // Display pseudo-assembly
void generateRealTargetCode();              // Generate real x86-64 assembly
void printRealTargetCode();                 // Display real x86-64 assembly

// --- File Output Functions (00_01_printToFile.c) ---
void initializeOutputFile();                // Open output file for writing
void closeOutputFile();                     // Close output file
void printAllTokensToFile();                // Write token list to file
void printASTNodeToFile(ASTNode* node, int indent); // Write AST node to file
void printAllASTNodesToFile();              // Write complete AST to file
void printSymbolTableToFile();              // Write symbol table to file
void print3AddressCodeToFile();             // Write TAC (before optimization) to file
void printBlocksBeforeOptimizationToFile(); // Write basic blocks (before optimization)
void printCFGOnlyToFile();                  // Write control flow graph only
void printBlocksToFile();                   // Write basic blocks (after optimization)
void print3AddressCodeAfterOptimizationToFile(int before_count, int after_count); // Write optimized TAC with statistics
void printRealTargetCodeToFile();           // Write real x86-64 assembly to file

#endif // DATABASE_H

