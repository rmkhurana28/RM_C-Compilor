// #include "database.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>


// // =====================================================
// //                TOKEN PRINTING SECTION 
// // =====================================================

// void printToken(Token* t, int index) {
//     if (!t) return;

//     printf("┌────────────────────────────────────┐\n");
//     printf("│ Token #%2d                         │\n", index + 1);
//     printf("├────────────────────────────────────┤\n");
//     printf("│ Type  : %-18s │\n", tokenTypeNames[t->token_type]);

//     switch (t->token_type) {
//         case VAL_INT:
//             printf("│ Value : %-18d │\n", t->int_value);
//             break;
//         case VAL_DOUBLE:
//             printf("│ Value : %-18.3lf │\n", t->double_value);
//             break;
//         case VAL_BOOL:
//             printf("│ Value : %-18s │\n", t->bool_value ? "true" : "false");
//             break;
//         case VAL_CHAR:
//             printf("│ Value : '%-16s' │\n", t->var_name);
//             break;
//         default:
//             if (strlen(t->var_name) > 0)
//                 printf("│ Value : %-18s │\n", t->var_name);
//             else
//                 printf("│ Value : (none)          │\n");
//             break;
//     }

//     printf("└────────────────────────────────────┘\n\n");
// }

// void printAllTokens() {
//     printf("\n=========================================\n");
//     printf("           PRINTING ALL TOKENS\n");
//     printf("=========================================\n\n");

//     for (int i = 0; i < token_count; i++)
//         printToken(tokens[i], i);

//     printf("=========================================\n");
//     printf("        END OF TOKEN PRINTING\n");
//     printf("=========================================\n\n");
// }





// // =====================================================
// //                AST PRINTING SECTION
// // =====================================================

// static void printIndent(int indent) {
//     for (int i = 0; i < indent; i++)
//         printf("  ");  // 2 spaces per indent
// }

// // =====================================================
// //                PRINT ALL AST NODES
// // =====================================================

// void printAllASTNodes() {
//     printf("=========================================\n");
//     printf("        PRINTING ALL AST NODES\n");
//     printf("=========================================\n");

//     for (int i = 0; i < ast_count-1; i++) {
//         printf("\n-----------------------------------------\n");
//         printf("AST #%d:\n", i + 1);
//         printf("-----------------------------------------\n");

//         printASTNode(all_ast[i], 0);
//     }

//     printf("\n=========================================\n");
//     printf("       END OF ALL AST NODES\n");
//     printf("=========================================\n");
// }


// // Map tokenType operators to string
// static const char* opToString(tokenType op) {
//     switch(op) {
//         case OP_PLUS: return "+";
//         case OP_MINUS: return "-";
//         case OP_MUL: return "*";
//         case OP_DIV: return "/";
//         case OP_EQUAL: return "=";
//         case OP_IS_EQUAL: return "==";
//         case OP_NOT_EQUAL: return "!=";
//         case OP_LESS_THAN: return "<";
//         case OP_GREAT_THAN: return ">";
//         case OP_LESS_EQUAL: return "<=";
//         case OP_GREAT_EQUAL: return ">=";
//         case OP_AND_AND: return "&&";
//         case OP_OR_OR: return "||";
//         case OP_NOT: return "!";
//         case OP_PLUS_PLUS: return "++";
//         case OP_MINUS_MINUS: return "--";
//         default: return "?";
//     }
// }

// // Convert token type (data types) to string
// static const char* tokenTypeToString(tokenType type) {
//     switch(type) {
//         case KEYWORD_INT:    return "int";
//         case KEYWORD_DOUBLE: return "double";
//         case KEYWORD_CHAR:   return "char";
//         case KEYWORD_BOOL:   return "bool";
//         default:             return "unknown_type";
//     }
// }

// void printASTNode(ASTNode* node, int indent) {
//     if (!node) return;

//     printIndent(indent);

//     switch(node->type) {
//         case AST_NUM:
//             printf("Num: %d\n", node->int_value);
//             break;
//         case AST_DOUBLE:
//             printf("Double: %f\n", node->double_value);
//             break;
//         case AST_BOOL:
//             printf("Bool: %s\n", node->bool_value ? "true" : "false");
//             break;
//         case AST_CHAR:
//             printf("Char: '%c'\n", node->char_value);
//             break;
//         case AST_STRING:
//             printf("String: \"%s\"\n", node->string_value);
//             break;
//         case AST_VAR:
//             printf("Var: %s\n", node->var.var_name);
//             break;
//         case AST_ARRAY_ACCESS:
//             printf("Array Access: %s[\n", node->array_access.array_name);
//             printASTNode(node->array_access.sizeExpr, indent + 1);
//             printIndent(indent);
//             printf("]\n");
//             break;
//         case AST_UNOP:
//             printf("UnaryOp: %s\n", opToString(node->unop.op));
//             printASTNode(node->unop.expr, indent + 1);
//             break;
//         case AST_BINOP:
//             printf("BinaryOp: %s\n", opToString(node->binop.op));
//             printASTNode(node->binop.left, indent + 1);
//             printASTNode(node->binop.right, indent + 1);
//             break;
//         case AST_ASSIGN:
//             printf("Assign:\n");
//             printIndent(indent + 1); printf("Var:\n");
//             printASTNode(node->assign.var, indent + 2);
//             printIndent(indent + 1); printf("Expr:\n");
//             printASTNode(node->assign.expr, indent + 2);
//             break;
//         case AST_IF:
//             printf("If Statement:\n");
//             printIndent(indent + 1); printf("Condition:\n");
//             printASTNode(node->if_stmt.condition, indent + 2);
//             printIndent(indent + 1); printf("Then:\n");
//             printASTNode(node->if_stmt.then_branch, indent + 2);
//             break;
//         case AST_IF_ELSE:
//             printf("If-Else Statement:\n");
//             printIndent(indent + 1); printf("Condition:\n");
//             printASTNode(node->if_else_stmt.condition, indent + 2);
//             printIndent(indent + 1); printf("Then:\n");
//             printASTNode(node->if_else_stmt.then_branch, indent + 2);
//             printIndent(indent + 1); printf("Else:\n");
//             printASTNode(node->if_else_stmt.else_branch, indent + 2);
//             break;
//         case AST_WHILE:
//             printf("While Loop:\n");
//             printIndent(indent + 1); printf("Condition:\n");
//             printASTNode(node->while_stmt.condition, indent + 2);
//             printIndent(indent + 1); printf("Body:\n");
//             printASTNode(node->while_stmt.body, indent + 2);
//             break;
//         case AST_FOR:
//             printf("For Loop:\n");
//             printIndent(indent + 1); printf("Init:\n");
//             printASTNode(node->for_stmt.init, indent + 2);
//             printIndent(indent + 1); printf("Condition:\n");
//             printASTNode(node->for_stmt.condition, indent + 2);
//             printIndent(indent + 1); printf("Update:\n");
//             printASTNode(node->for_stmt.update, indent + 2);
//             printIndent(indent + 1); printf("Body:\n");
//             printASTNode(node->for_stmt.body, indent + 2);
//             break;
//         case AST_BLOCK:
//             printf("Block (%d statements):\n", node->block.statement_count);
//             for (int i = 0; i < node->block.statement_count; i++)
//                 printASTNode(node->block.statements[i], indent + 1);
//             break;
//         case AST_PROGRAM:
//             printf("Program (%d top-level statements):\n", node->program.statement_count);
//             for (int i = 0; i < node->program.statement_count; i++)
//                 printASTNode(node->program.statements[i], indent + 1);
//             break;
//         case AST_DECL:
//             if (node->decl.is_array)
//                 printf("Declaration: %s[%s] (%s)\n",
//                        node->decl.var_name,
//                        node->decl.array_size,
//                        tokenTypeToString(node->decl.type));
//             else
//                 printf("Declaration: %s (%s)\n",
//                        node->decl.var_name,
//                        tokenTypeToString(node->decl.type));

//             if (node->decl.init_expr) {
//                 printIndent(indent + 1);
//                 printf("Initializer:\n");
//                 printASTNode(node->decl.init_expr, indent + 2);
//             }
//             break;
//         default:
//             printf("Unknown AST Node\n");
//     }
// }


// void printSymbolTable() {
//     printf("\n╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
//     printf("║                                      SYMBOL TABLE CONTENTS                                       ║\n");
//     printf("╠═══════╤══════════════════╤════════════════╤════════╤══════════╤══════════╤═══════╤══════════════╣\n");
//     printf("║  #    │ Name             │ Type           │ Array  │ Size     │ Init     │ Scope │ Block ID     ║\n");
//     printf("╠═══════╪══════════════════╪════════════════╪════════╪══════════╪══════════╪═══════╪══════════════╣\n");

//     if (symbol_count == 0) {
//         printf("║                                         (Symbol table is empty)                                 ║\n");
//     } else {
//         for (int i = 0; i < symbol_count; i++) {
//             printf("║ %3d   │ %-16s │ %-14s │ %-6s │ %-8s │ %-8s │ %5d │ %10d   ║\n",
//                    i + 1,
//                    symbolTable[i]->var_name,
//                    tokenTypeNames[symbolTable[i]->type],
//                    symbolTable[i]->isArray ? "Yes" : "No",
//                    symbolTable[i]->isArray ? symbolTable[i]->arraySize : "-",
//                    symbolTable[i]->isInitialized ? "Yes" : "No",
//                    symbolTable[i]->scope,
//                    symbolTable[i]->blockId);
//         }
//     }

//     printf("╠═══════╧══════════════════╧════════════════╧════════╧══════════╧══════════╧═══════╧══════════════╣\n");
//     printf("║                                        Total Symbols: %-3d                                       ║\n", symbol_count);
//     printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
// }


#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// =====================================================
//                TOKEN PRINTING SECTION 
// =====================================================

void printToken(Token* t, int index) {
    if (!t) return;

    printf("┌────────────────────────────────────┐\n");
    printf("│ Token #%2d                         │\n", index + 1);
    printf("├────────────────────────────────────┤\n");
    printf("│ Type  : %-18s │\n", tokenTypeNames[t->token_type]);

    switch (t->token_type) {
        case VAL_INT:
            printf("│ Value : %-18d │\n", t->int_value);
            break;
        case VAL_DOUBLE:
            printf("│ Value : %-18.3lf │\n", t->double_value);
            break;
        case VAL_BOOL:
            printf("│ Value : %-18s │\n", t->bool_value ? "true" : "false");
            break;
        case VAL_CHAR:
            printf("│ Value : '%-16s' │\n", t->var_name);
            break;
        default:
            if (strlen(t->var_name) > 0)
                printf("│ Value : %-18s │\n", t->var_name);
            else
                printf("│ Value : (none)          │\n");
            break;
    }

    printf("└────────────────────────────────────┘\n\n");
}

void printAllTokens() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    LEXICAL ANALYSIS - TOKENS                                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    for (int i = 0; i < token_count; i++)
        printToken(tokens[i], i);

    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                    END OF TOKEN LIST (Total: %d)\n", token_count);
    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    // Success message for token generation
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                              ✓ LEXICAL ANALYSIS SUCCESSFUL                                      ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  All tokens have been successfully generated and validated.                                     ║\n");
    printf("║  Total Tokens Generated: %-3d                                                                   ║\n", token_count);
    printf("║  Status: Ready for Parsing                                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}


// =====================================================
//                AST PRINTING SECTION
// =====================================================

static void printIndent(int indent) {
    for (int i = 0; i < indent; i++)
        printf("  ");  // 2 spaces per indent
}

// =====================================================
//                PRINT ALL AST NODES
// =====================================================

void printAllASTNodes() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                 ABSTRACT SYNTAX TREE (AST)                                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    for (int i = 0; i < ast_count-1; i++) {
        printf("\n");
        printf("─────────────────────────────────────────────────────────────────────────────────────────────────\n");
        printf("  AST Node #%d\n", i + 1);
        printf("─────────────────────────────────────────────────────────────────────────────────────────────────\n");

        printASTNode(all_ast[i], 0);
    }

    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                  END OF AST (Total Nodes: %d)\n", ast_count);
    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    // Success message for AST generation
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                              ✓ AST GENERATION SUCCESSFUL                                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Abstract Syntax Tree has been successfully generated and validated.                            ║\n");
    printf("║  Total AST Nodes Created: %-3d                                                                  ║\n", ast_count);
    printf("║  Status: Ready for Semantic Analysis                                                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}


// Map tokenType operators to string
static const char* opToString(tokenType op) {
    switch(op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_EQUAL: return "=";
        case OP_IS_EQUAL: return "==";
        case OP_NOT_EQUAL: return "!=";
        case OP_LESS_THAN: return "<";
        case OP_GREAT_THAN: return ">";
        case OP_LESS_EQUAL: return "<=";
        case OP_GREAT_EQUAL: return ">=";
        case OP_AND_AND: return "&&";
        case OP_OR_OR: return "||";
        case OP_NOT: return "!";
        case OP_PLUS_PLUS: return "++";
        case OP_MINUS_MINUS: return "--";
        default: return "?";
    }
}

// Convert token type (data types) to string
static const char* tokenTypeToString(tokenType type) {
    switch(type) {
        case KEYWORD_INT:    return "int";
        case KEYWORD_DOUBLE: return "double";
        case KEYWORD_CHAR:   return "char";
        case KEYWORD_BOOL:   return "bool";
        default:             return "unknown_type";
    }
}

void printASTNode(ASTNode* node, int indent) {
    if (!node) return;

    printIndent(indent);

    switch(node->type) {
        case AST_NUM:
            printf("Num: %d\n", node->int_value);
            break;
        case AST_DOUBLE:
            printf("Double: %f\n", node->double_value);
            break;
        case AST_BOOL:
            printf("Bool: %s\n", node->bool_value ? "true" : "false");
            break;
        case AST_CHAR:
            printf("Char: '%c'\n", node->char_value);
            break;
        case AST_STRING:
            printf("String: \"%s\"\n", node->string_value);
            break;
        case AST_VAR:
            printf("Var: %s\n", node->var.var_name);
            break;
        case AST_ARRAY_ACCESS:
            printf("Array Access: %s[\n", node->array_access.array_name);
            printASTNode(node->array_access.sizeExpr, indent + 1);
            printIndent(indent);
            printf("]\n");
            break;
        case AST_UNOP:
            printf("UnaryOp: %s\n", opToString(node->unop.op));
            printASTNode(node->unop.expr, indent + 1);
            break;
        case AST_BINOP:
            printf("BinaryOp: %s\n", opToString(node->binop.op));
            printASTNode(node->binop.left, indent + 1);
            printASTNode(node->binop.right, indent + 1);
            break;
        case AST_ASSIGN:
            printf("Assign:\n");
            printIndent(indent + 1); printf("Var:\n");
            printASTNode(node->assign.var, indent + 2);
            printIndent(indent + 1); printf("Expr:\n");
            printASTNode(node->assign.expr, indent + 2);
            break;
        case AST_IF:
            printf("If Statement:\n");
            printIndent(indent + 1); printf("Condition:\n");
            printASTNode(node->if_stmt.condition, indent + 2);
            printIndent(indent + 1); printf("Then:\n");
            printASTNode(node->if_stmt.then_branch, indent + 2);
            break;
        case AST_IF_ELSE:
            printf("If-Else Statement:\n");
            printIndent(indent + 1); printf("Condition:\n");
            printASTNode(node->if_else_stmt.condition, indent + 2);
            printIndent(indent + 1); printf("Then:\n");
            printASTNode(node->if_else_stmt.then_branch, indent + 2);
            printIndent(indent + 1); printf("Else:\n");
            printASTNode(node->if_else_stmt.else_branch, indent + 2);
            break;
        case AST_WHILE:
            printf("While Loop:\n");
            printIndent(indent + 1); printf("Condition:\n");
            printASTNode(node->while_stmt.condition, indent + 2);
            printIndent(indent + 1); printf("Body:\n");
            printASTNode(node->while_stmt.body, indent + 2);
            break;
        case AST_FOR:
            printf("For Loop:\n");
            printIndent(indent + 1); printf("Init:\n");
            printASTNode(node->for_stmt.init, indent + 2);
            printIndent(indent + 1); printf("Condition:\n");
            printASTNode(node->for_stmt.condition, indent + 2);
            printIndent(indent + 1); printf("Update:\n");
            printASTNode(node->for_stmt.update, indent + 2);
            printIndent(indent + 1); printf("Body:\n");
            printASTNode(node->for_stmt.body, indent + 2);
            break;
        case AST_BLOCK:
            printf("Block (%d statements):\n", node->block.statement_count);
            for (int i = 0; i < node->block.statement_count; i++)
                printASTNode(node->block.statements[i], indent + 1);
            break;
        case AST_PROGRAM:
            printf("Program (%d top-level statements):\n", node->program.statement_count);
            for (int i = 0; i < node->program.statement_count; i++)
                printASTNode(node->program.statements[i], indent + 1);
            break;
        case AST_DECL:
            if (node->decl.is_array)
                printf("Declaration: %s[%s] (%s)\n",
                       node->decl.var_name,
                       node->decl.array_size,
                       tokenTypeToString(node->decl.type));
            else
                printf("Declaration: %s (%s)\n",
                       node->decl.var_name,
                       tokenTypeToString(node->decl.type));

            if (node->decl.init_expr) {
                printIndent(indent + 1);
                printf("Initializer:\n");
                printASTNode(node->decl.init_expr, indent + 2);
            }
            break;
        default:
            printf("Unknown AST Node\n");
    }
}


void printSymbolTable() {
    // Calculate max block ID from symbol table
    int maxBlockId = 0;
    for (int i = 0; i < symbol_count; i++) {
        if (symbolTable[i]->blockId > maxBlockId) {
            maxBlockId = symbolTable[i]->blockId;
        }
    }
    
    // Success message for semantic analysis
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           ✓ SEMANTIC ANALYSIS COMPLETED                                         ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Type checking and semantic validation completed successfully.                                  ║\n");
    printf("║  Total Symbols in Table: %-3d                                                                   ║\n", symbol_count);
    printf("║  Total Block Scopes: %-3d                                                                       ║\n", maxBlockId + 1);
    printf("║  Total Warnings: %-3d                                                                           ║\n", warning_count);
    
    // Display all warnings if any exist
    if (warning_count > 0) {
        printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║  Warnings:                                                                                      ║\n");
        
        for (int i = 0; i < warning_count; i++) {
            // Create a copy of the warning to tokenize
            char warning_copy[256];
            strncpy(warning_copy, all_warnings[i], 255);
            warning_copy[255] = '\0';
            
            // Split by newlines and print each line
            char *line = strtok(warning_copy, "\n");
            int line_num = 0;
            
            while (line != NULL) {
                if (line_num == 0) {
                    // First line with number prefix
                    printf("║  %d. %-91s  ║\n", i + 1, line);
                } else {
                    // Continuation lines
                    printf("║     %-91s  ║\n", line);
                }
                line = strtok(NULL, "\n");
                line_num++;
            }
        }
    }
    
    printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Status: Ready for Code Generation                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Symbol table
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                      SYMBOL TABLE CONTENTS                                       ║\n");
    printf("╠═══════╤══════════════════╤════════════════╤════════╤══════════╤══════════╤═══════╤══════════════╣\n");
    printf("║  #    │ Name             │ Type           │ Array  │ Size     │ Init     │ Scope │ Block ID     ║\n");
    printf("╠═══════╪══════════════════╪════════════════╪════════╪══════════╪══════════╪═══════╪══════════════╣\n");

    if (symbol_count == 0) {
        printf("║                                         (Symbol table is empty)                                 ║\n");
    } else {
        for (int i = 0; i < symbol_count; i++) {
            printf("║ %3d   │ %-16s │ %-14s │ %-6s │ %-8s │ %-8s │ %5d │ %10d   ║\n",
                   i + 1,
                   symbolTable[i]->var_name,
                   tokenTypeNames[symbolTable[i]->type],
                   symbolTable[i]->isArray ? "Yes" : "No",
                   symbolTable[i]->isArray ? symbolTable[i]->arraySize : "-",
                   symbolTable[i]->isInitialized ? "Yes" : "No",
                   symbolTable[i]->scope,
                   symbolTable[i]->blockId);
        }
    }

    printf("╠═══════╧══════════════════╧════════════════╧════════╧══════════╧══════════╧═══════╧══════════════╣\n");
    printf("║                                        Total Symbols: %-3d                                       ║\n", symbol_count);
    printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

