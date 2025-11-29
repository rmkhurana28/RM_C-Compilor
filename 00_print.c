/**
 * 00_print.c - Console Output Functions (Disabled)
 * 
 * This module contains console printing functions that are currently disabled.
 * All detailed output has been redirected to files for cleaner terminal output.
 * These functions are preserved for reference and potential debugging use.
 * 
 * Note: All printf statements are commented out. See 00_01_printToFile.c for
 * active file output functions.
 * 
 * Author: Ridham Khurana
 */

// suppress format truncation warnings - buffers are sized correctly
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// =====================================================
//                TOKEN PRINTING SECTION 
// =====================================================

/**
 * Print a single token to console (currently disabled)
 * 
 * @param t - Pointer to the token to print
 * @param index - Index position of the token in the token array
 * 
 * Displays token type and value in a formatted box.
 * All output is commented out - kept for reference.
 */
void printToken(Token* t, int index) {
    (void)index; // unused parameter
    if (!t) return;

    // printf("┌────────────────────────────────────┐\n");
    // printf("│ Token #%2d                         │\n", index + 1);
    // printf("├────────────────────────────────────┤\n");
    // printf("│ Type  : %-18s │\n", tokenTypeNames[t->token_type]);

    switch (t->token_type) {
        case VAL_INT:
            // printf("│ Value : %-18d │\n", t->int_value);
            break;
        case VAL_DOUBLE:
            // printf("│ Value : %-18.3lf │\n", t->double_value);
            break;
        case VAL_BOOL:
            // printf("│ Value : %-18s │\n", t->bool_value ? "true" : "false");
            break;
        case VAL_CHAR:
            // printf("│ Value : '%-16s' │\n", t->var_name);
            break;
        default:
            if (strlen(t->var_name) > 0) {
                // printf("│ Value : %-18s │\n", t->var_name);
            } else {
                // printf("│ Value : (none)          │\n");
            }
            break;
    }

    // printf("└────────────────────────────────────┘\n\n");
}

/**
 * Print all tokens from the lexical analyzer (currently disabled)
 * 
 * Iterates through the global tokens array and prints each token
 * with a summary header and footer. All output is commented out.
 */
void printAllTokens() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                    LEXICAL ANALYSIS - TOKENS                                     ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");

    // Loop through all tokens and display them
    for (int i = 0; i < token_count; i++)
        printToken(tokens[i], i);

    // printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    // printf("                                    END OF TOKEN LIST (Total: %d)\n", token_count);
    // printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    // Success message for token generation
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                              ✓ LEXICAL ANALYSIS SUCCESSFUL                                      ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  All tokens have been successfully generated and validated.                                     ║\n");
    // printf("║  Total Tokens Generated: %-3d                                                                   ║\n", token_count);
    // printf("║  Status: Ready for Parsing                                                                      ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}

// =====================================================
//                AST PRINTING SECTION
// =====================================================

/**
 * Print indentation for AST tree visualization (currently disabled)
 * 
 * @param indent - Number of indentation levels (each level = 2 spaces)
 * 
 * Used to create hierarchical tree structure when printing AST nodes.
 */
static void printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        // printf("  ");  // 2 spaces per indent
    }
}

// =====================================================
//                PRINT ALL AST NODES
// =====================================================

/**
 * Print all AST nodes from the parser (currently disabled)
 * 
 * Iterates through the global all_ast array and prints each AST node
 * with hierarchical structure. All output is commented out.
 */
void printAllASTNodes() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                 ABSTRACT SYNTAX TREE (AST)                                       ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    // Loop through all AST nodes
    for (int i = 0; i < ast_count-1; i++) {
        // printf("\n");
        // printf("─────────────────────────────────────────────────────────────────────────────────────────────────\n");
        // printf("  AST Node #%d\n", i + 1);
        // printf("─────────────────────────────────────────────────────────────────────────────────────────────────\n");

        printASTNode(all_ast[i], 0);
    }

    // printf("\n");
    // printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    // printf("                                  END OF AST (Total Nodes: %d)\n", ast_count);
    // printf("═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    // Success message for AST generation
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                              ✓ AST GENERATION SUCCESSFUL                                        ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Abstract Syntax Tree has been successfully generated and validated.                            ║\n");
    // printf("║  Total AST Nodes Created: %-3d                                                                  ║\n", ast_count);
    // printf("║  Status: Ready for Semantic Analysis                                                            ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}


/**
 * Convert token type operator to string representation
 * 
 * @param op - Token type representing an operator
 * @return String representation of the operator (e.g., "+", "==", "!")
 * 
 * Maps operator token types to their symbolic string forms.
 * Currently unused but kept for potential future use.
 */
__attribute__((unused))
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

/**
 * Convert token type (data type keywords) to string
 * 
 * @param type - Token type representing a data type keyword
 * @return String name of the data type ("int", "double", "char", "bool")
 * 
 * Maps data type token types to their string names.
 * Currently unused but kept for potential future use.
 */
__attribute__((unused))
static const char* tokenTypeToString(tokenType type) {
    switch(type) {
        case KEYWORD_INT:    return "int";
        case KEYWORD_DOUBLE: return "double";
        case KEYWORD_CHAR:   return "char";
        case KEYWORD_BOOL:   return "bool";
        default:             return "unknown_type";
    }
}

/**
 * Print a single AST node recursively (currently disabled)
 * 
 * @param node - Pointer to the AST node to print
 * @param indent - Current indentation level for tree visualization
 * 
 * Recursively traverses and prints AST structure showing node types,
 * values, and hierarchical relationships. Handles all AST node types
 * including literals, variables, operators, control flow, etc.
 */
void printASTNode(ASTNode* node, int indent) {
    if (!node) return;

    printIndent(indent);

    switch(node->type) {
        case AST_NUM:
            // printf("Num: %d\n", node->int_value);
            break;
        case AST_DOUBLE:
            // printf("Double: %f\n", node->double_value);
            break;
        case AST_BOOL:
            // printf("Bool: %s\n", node->bool_value ? "true" : "false");
            break;
        case AST_CHAR:
            // printf("Char: '%c'\n", node->char_value);
            break;
        case AST_STRING:
            // printf("String: \"%s\"\n", node->string_value);
            break;
        case AST_VAR:
            // printf("Var: %s\n", node->var.var_name);
            break;
        case AST_ARRAY_ACCESS:
            // printf("Array Access: %s[\n", node->array_access.array_name);
            printASTNode(node->array_access.sizeExpr, indent + 1);
            printIndent(indent);
            // printf("]\n");
            break;
        case AST_UNOP:
            // Modified to show prefix/postfix information
            if (node->unop.op == OP_PLUS_PLUS || node->unop.op == OP_MINUS_MINUS) {
                // printf("UnaryOp: %s (%s)\n", 
                //        opToString(node->unop.op),
                //        node->unop.isPrefix ? "prefix" : "postfix");
            } else {
                // printf("UnaryOp: %s\n", opToString(node->unop.op));
            }
            printASTNode(node->unop.expr, indent + 1);
            break;
        case AST_BINOP:
            // printf("BinaryOp: %s\n", opToString(node->binop.op));
            printASTNode(node->binop.left, indent + 1);
            printASTNode(node->binop.right, indent + 1);
            break;
        case AST_ASSIGN:
            // printf("Assign:\n");
            printIndent(indent + 1); // printf("Var:\n");
            printASTNode(node->assign.var, indent + 2);
            printIndent(indent + 1); // printf("Expr:\n");
            printASTNode(node->assign.expr, indent + 2);
            break;
        case AST_IF:
            // printf("If Statement:\n");
            printIndent(indent + 1); // printf("Condition:\n");
            printASTNode(node->if_stmt.condition, indent + 2);
            printIndent(indent + 1); // printf("Then:\n");
            printASTNode(node->if_stmt.then_branch, indent + 2);
            break;
        case AST_IF_ELSE:
            // printf("If-Else Statement:\n");
            printIndent(indent + 1); // printf("Condition:\n");
            printASTNode(node->if_else_stmt.condition, indent + 2);
            printIndent(indent + 1); // printf("Then:\n");
            printASTNode(node->if_else_stmt.then_branch, indent + 2);
            printIndent(indent + 1); // printf("Else:\n");
            printASTNode(node->if_else_stmt.else_branch, indent + 2);
            break;
        case AST_WHILE:
            // printf("While Loop:\n");
            printIndent(indent + 1); // printf("Condition:\n");
            printASTNode(node->while_stmt.condition, indent + 2);
            printIndent(indent + 1); // printf("Body:\n");
            printASTNode(node->while_stmt.body, indent + 2);
            break;
        case AST_FOR:
            // printf("For Loop:\n");
            printIndent(indent + 1); // printf("Init:\n");
            printASTNode(node->for_stmt.init, indent + 2);
            printIndent(indent + 1); // printf("Condition:\n");
            printASTNode(node->for_stmt.condition, indent + 2);
            printIndent(indent + 1); // printf("Update:\n");
            printASTNode(node->for_stmt.update, indent + 2);
            printIndent(indent + 1); // printf("Body:\n");
            printASTNode(node->for_stmt.body, indent + 2);
            break;
        case AST_BLOCK:
            // printf("Block (%d statements):\n", node->block.statement_count);
            for (int i = 0; i < node->block.statement_count; i++)
                printASTNode(node->block.statements[i], indent + 1);
            break;
        case AST_PROGRAM:
            // printf("Program (%d top-level statements):\n", node->program.statement_count);
            for (int i = 0; i < node->program.statement_count; i++)
                printASTNode(node->program.statements[i], indent + 1);
            break;
        case AST_DECL:
            if (node->decl.is_array) {
                // printf("Declaration: %s[%s] (%s)\n",
                //        node->decl.var_name,
                //        node->decl.array_size,
                //        tokenTypeToString(node->decl.type));
            } else {
                // printf("Declaration: %s (%s)\n",
                //        node->decl.var_name,
                //        tokenTypeToString(node->decl.type));
            }

            if (node->decl.init_expr) {
                printIndent(indent + 1);
                // printf("Initializer:\n");
                printASTNode(node->decl.init_expr, indent + 2);
            }
            break;
        default:
            // printf("Unknown AST Node\n");
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
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                           ✓ SEMANTIC ANALYSIS COMPLETED                                         ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Type checking and semantic validation completed successfully.                                  ║\n");
    // printf("║  Total Symbols in Table: %-3d                                                                   ║\n", symbol_count);
    // printf("║  Total Block Scopes: %-3d                                                                       ║\n", maxBlockId + 1);
    // printf("║  Total Warnings: %-3d                                                                           ║\n", warning_count);
    
    // Display all warnings if any exist
    if (warning_count > 0) {
        // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
        // printf("║  Warnings:                                                                                      ║\n");
        
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
    
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Status: Ready for Code Generation                                                              ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
    
    // Symbol table
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                      SYMBOL TABLE CONTENTS                                       ║\n");
    // printf("╠═══════╤══════════════════╤════════════════╤════════╤══════════╤══════════╤═══════╤══════════════╣\n");
    // printf("║  #    │ Name             │ Type           │ Array  │ Size     │ Init     │ Scope │ Block ID     ║\n");
    // printf("╠═══════╪══════════════════╪════════════════╪════════╪══════════╪══════════╪═══════╪══════════════╣\n");

    if (symbol_count == 0) {
        // printf("║                                         (Symbol table is empty)                                 ║\n");
    } else {
        for (int i = 0; i < symbol_count; i++) {
            // printf("║ %3d   │ %-16s │ %-14s │ %-6s │ %-8s │ %-8s │ %5d │ %10d   ║\n",
            //        i + 1,
            //        symbolTable[i]->var_name,
            //        tokenTypeNames[symbolTable[i]->type],
            //        symbolTable[i]->isArray ? "Yes" : "No",
            //        symbolTable[i]->isArray ? symbolTable[i]->arraySize : "-",
            //        symbolTable[i]->isInitialized ? "Yes" : "No",
            //        symbolTable[i]->scope,
            //        symbolTable[i]->blockId);
        }
    }

    // printf("╠═══════╧══════════════════╧════════════════╧════════╧══════════╧══════════╧═══════╧══════════════╣\n");
    // printf("║                                        Total Symbols: %-3d                                       ║\n", symbol_count);
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

// =====================================================
//          3-ADDRESS CODE PRINTING SECTION
// =====================================================

void print3AddressCode() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                   THREE-ADDRESS CODE (TAC)                                       ║\n");
    // printf("╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  #    │ Instruction                                                                              ║\n");
    // printf("╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (addr_count == 0) {
        // printf("║                                    (No 3-address code generated)                                 ║\n");
    } else {
        for (int i = 0; i < addr_count; i++) {
            address* addr = allAddress[i];
            char instruction[256] = "";
            
            switch(addr->type) {
                case ADDR_ASSIGN:
                    snprintf(instruction, 256, "%s = %s", 
                            addr->assign.result, 
                            addr->assign.arg1);
                    break;
                    
                case ADDR_BINOP:
                    snprintf(instruction, 256, "%s = %s %s %s", 
                            addr->binop.result, 
                            addr->binop.arg1, 
                            addr->binop.op, 
                            addr->binop.arg2);
                    break;
                    
                case ADDR_UNOP:
                    snprintf(instruction, 256, "%s = %s %s", 
                            addr->unop.result, 
                            addr->unop.op, 
                            addr->unop.arg1);
                    break;
                    
                case ADDR_GOTO:
                    snprintf(instruction, 256, "goto %s", 
                            addr->goto_stmt.target);
                    break;
                    
                case ADDR_IF_F_GOTO:
                    snprintf(instruction, 256, "ifFalse %s goto %s", 
                            addr->if_false.condition, 
                            addr->if_false.target);
                    break;
                    
                case ADDR_IF_T_GOTO:
                    snprintf(instruction, 256, "ifTrue %s goto %s", 
                            addr->if_true.condition, 
                            addr->if_true.target);
                    break;
                    
                case ADDR_LABEL:
                    snprintf(instruction, 256, "%s:", 
                            addr->label.labelNumber);
                    break;
                    
                case ADDR_ARRAY_READ:
                    snprintf(instruction, 256, "%s = %s[%s]", 
                            addr->array_read.result, 
                            addr->array_read.array, 
                            addr->array_read.index);
                    break;
                    
                case ADDR_ARRAY_WRITE:
                    snprintf(instruction, 256, "%s[%s] = %s", 
                            addr->array_write.array, 
                            addr->array_write.index, 
                            addr->array_write.value);
                    break;
                    
                default:
                    snprintf(instruction, 256, "(Unknown instruction type)");
                    break;
            }
            
            // printf("║ %5d │ %-84s ║\n", i + 1, instruction);
        }
    }

    // printf("╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║                                  Total Instructions: %-3d                                        ║\n", addr_count);
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    // Success message for 3-address code generation
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                        ✓ THREE-ADDRESS CODE GENERATION SUCCESSFUL                               ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Intermediate code has been successfully generated from the AST.                                ║\n");
    // printf("║  Total Instructions Generated: %-3d                                                             ║\n", addr_count);
    // printf("║  Status: Ready for Optimization/Target Code Generation                                         ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}

void printBlocks() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                      BASIC BLOCKS                                                ║\n");
    // printf("╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║ Block │ Instructions                                                                             ║\n");
    // printf("╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (block_count == 0) {
        // printf("║                                    (No blocks generated)                                         ║\n");
    } else {
        for (int i = 0; i < block_count; i++) {
            block* blk = allBlocks[i];
            if (!blk) continue;

            // Block header
            // printf("║ B%-4d │                                                                                      ║\n", blk->blockID);
            // printf("║       │ Instructions: %-3d                                                                   ║\n", blk->numberOfAddressesInBlock);

            // In edges
            if (blk->numCFGIn > 0) {
                // printf("║       │ In: ");
                for (int j = 0; j < blk->numCFGIn; j++) {
                    if (blk->cfg_in[j]) printf("B%d", blk->cfg_in[j]->blockID);
                    if (j < blk->numCFGIn - 1) printf(", ");
                }
                // printf("\n");
            } else {
                // printf("║       │ In: (none)                                                                           ║\n");
            }

            // Out edges
            if (blk->numCFGOut > 0) {
                // printf("║       │ Out: ");
                for (int j = 0; j < blk->numCFGOut; j++) {
                    if (blk->cfg_out[j]) printf("B%d", blk->cfg_out[j]->blockID);
                    if (j < blk->numCFGOut - 1) printf(", ");
                }
                // printf("\n");
            } else {
                // printf("║       │ Out: (none)                                                                          ║\n");
            }

            // printf("║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");

            // Print each instruction in the block
            for (int j = 0; j < blk->numberOfAddressesInBlock; j++) {
                address* addr = blk->list[j];
                char instruction[256] = "";
                if (!addr) continue;

                // Find the global line number (index in allAddress)
                int globalLineNum = -1;
                for (int k = 0; k < addr_count; k++) {
                    if (allAddress[k] == addr) {
                        globalLineNum = k + 1; // +1 for 1-based indexing
                        break;
                    }
                }

                switch(addr->type) {
                    case ADDR_ASSIGN:
                        snprintf(instruction, 256, "%s = %s", addr->assign.result, addr->assign.arg1);
                        break;
                    case ADDR_BINOP:
                        snprintf(instruction, 256, "%s = %s %s %s", addr->binop.result, addr->binop.arg1, addr->binop.op, addr->binop.arg2);
                        break;
                    case ADDR_UNOP:
                        snprintf(instruction, 256, "%s = %s %s", addr->unop.result, addr->unop.op, addr->unop.arg1);
                        break;
                    case ADDR_GOTO:
                        snprintf(instruction, 256, "goto %s", addr->goto_stmt.target);
                        break;
                    case ADDR_IF_F_GOTO:
                        snprintf(instruction, 256, "ifFalse %s goto %s", addr->if_false.condition, addr->if_false.target);
                        break;
                    case ADDR_IF_T_GOTO:
                        snprintf(instruction, 256, "ifTrue %s goto %s", addr->if_true.condition, addr->if_true.target);
                        break;
                    case ADDR_LABEL:
                        snprintf(instruction, 256, "%s:", addr->label.labelNumber);
                        break;
                    case ADDR_ARRAY_READ:
                        snprintf(instruction, 256, "%s = %s[%s]", addr->array_read.result, addr->array_read.array, addr->array_read.index);
                        break;
                    case ADDR_ARRAY_WRITE:
                        snprintf(instruction, 256, "%s[%s] = %s", addr->array_write.array, addr->array_write.index, addr->array_write.value);
                        break;
                    default:
                        snprintf(instruction, 256, "(Unknown instruction type)");
                        break;
                }

                if (globalLineNum != -1) {
                    printf("║       │  [%2d] ◦ [%2d] %-68s ║\n", globalLineNum, j + 1, instruction);
                } else {
                    printf("║       │  [??] ◦ [%2d] %-68s ║\n", j + 1, instruction);
                }
            }

            // Print GEN/KILL/IN/OUT if they exist
            if (i < block_prop_count && allBlockProps[i]) {
                blockProp* prop = allBlockProps[i];
                
                // printf("║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");
                // printf("║       │ A) Reaching Definitions                                                              ║\n");
                
                // Print GEN
                char genLine[256] = "➢ GEN: ";
                if (prop->numGen > 0) {
                    strcat(genLine, "{ ");
                    for (int j = 0; j < prop->numGen; j++) {
                        if (prop->gen[j]) {
                            char genStr[64] = "";
                            address* addr = prop->gen[j];
                            // Find the line number (index in allAddress)
                            int lineNum = -1;
                            for (int k = 0; k < addr_count; k++) {
                                if (allAddress[k] == addr) {
                                    lineNum = k + 1; // +1 for 1-based indexing
                                    break;
                                }
                            }
                            if (lineNum != -1) {
                                snprintf(genStr, 64, "%d", lineNum);
                            } else {
                                snprintf(genStr, 64, "?");
                            }
                            strcat(genLine, genStr);
                            if (j < prop->numGen - 1) strcat(genLine, ", ");
                        }
                    }
                    strcat(genLine, " }");
                } else {
                    strcat(genLine, "∅");
                }
                // printf("║       │     %-81s ║\n", genLine);
                
                // Print KILL
                char killLine[256] = "➢ KILL: ";
                if (prop->numKill > 0) {
                    strcat(killLine, "{ ");
                    for (int j = 0; j < prop->numKill; j++) {
                        if (prop->kill[j]) {
                            char killStr[64] = "";
                            address* addr = prop->kill[j];
                            // Find the line number (index in allAddress)
                            int lineNum = -1;
                            for (int k = 0; k < addr_count; k++) {
                                if (allAddress[k] == addr) {
                                    lineNum = k + 1; // +1 for 1-based indexing
                                    break;
                                }
                            }
                            if (lineNum != -1) {
                                snprintf(killStr, 64, "%d", lineNum);
                            } else {
                                snprintf(killStr, 64, "?");
                            }
                            strcat(killLine, killStr);
                            if (j < prop->numKill - 1) strcat(killLine, ", ");
                        }
                    }
                    strcat(killLine, " }");
                } else {
                    strcat(killLine, "∅");
                }
                // printf("║       │     %-81s ║\n", killLine);
                
                // Print IN
                char inLine[256] = "➢ IN: ";
                if (prop->numIn > 0) {
                    strcat(inLine, "{ ");
                    for (int j = 0; j < prop->numIn; j++) {
                        if (prop->in[j]) {
                            char inStr[64] = "";
                            address* addr = prop->in[j];
                            // Find the line number (index in allAddress)
                            int lineNum = -1;
                            for (int k = 0; k < addr_count; k++) {
                                if (allAddress[k] == addr) {
                                    lineNum = k + 1; // +1 for 1-based indexing
                                    break;
                                }
                            }
                            if (lineNum != -1) {
                                snprintf(inStr, 64, "%d", lineNum);
                            } else {
                                snprintf(inStr, 64, "?");
                            }
                            strcat(inLine, inStr);
                            if (j < prop->numIn - 1) strcat(inLine, ", ");
                        }
                    }
                    strcat(inLine, " }");
                } else {
                    strcat(inLine, "∅");
                }
                // printf("║       │     %-81s ║\n", inLine);
                
                // Print OUT
                char outLine[256] = "➢ OUT: ";
                if (prop->numOut > 0) {
                    strcat(outLine, "{ ");
                    for (int j = 0; j < prop->numOut; j++) {
                        if (prop->out[j]) {
                            char outStr[64] = "";
                            address* addr = prop->out[j];
                            // Find the line number (index in allAddress)
                            int lineNum = -1;
                            for (int k = 0; k < addr_count; k++) {
                                if (allAddress[k] == addr) {
                                    lineNum = k + 1; // +1 for 1-based indexing
                                    break;
                                }
                            }
                            if (lineNum != -1) {
                                snprintf(outStr, 64, "%d", lineNum);
                            } else {
                                snprintf(outStr, 64, "?");
                            }
                            strcat(outLine, outStr);
                            if (j < prop->numOut - 1) strcat(outLine, ", ");
                        }
                    }
                    strcat(outLine, " }");
                } else {
                    strcat(outLine, "∅");
                }
                // printf("║       │     %-81s ║\n", outLine);
            }

            if (i < block_count - 1) {
                // printf("╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");
            }
        }
    }

    // printf("╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║                                    Total Blocks: %-3d                                            ║\n", block_count);
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    // Success message
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                           ✓ BASIC BLOCKS GENERATION SUCCESSFUL                                  ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Basic blocks have been successfully generated from 3-address code.                             ║\n");
    // printf("║  Total Blocks Generated: %-3d                                                                   ║\n", block_count);
    // printf("║  Status: Ready for Optimization Analysis                                                        ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}

// Print x86-64 assembly code (Target Code)
void printTargetCode() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                                    TARGET CODE (x86-64 Assembly)                                 ║\n");
    // printf("╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  #    │ Assembly Instruction                                                                     ║\n");
    // printf("╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    // External declarations for assembly code
    extern AsmInstruction* asmCode[];
    extern int asm_count;

    if (asm_count == 0) {
        // printf("║                                    (No target code generated)                                   ║\n");
    } else {
        for (int i = 0; i < asm_count; i++) {
            // Handle empty lines and labels (no line numbers for formatting)
            if (strlen(asmCode[i]->instruction) == 0) {
                // printf("║       │                                                                                      ║\n");
            } else if (asmCode[i]->instruction[0] != ' ' && 
                       asmCode[i]->instruction[0] != '\t' &&
                       strchr(asmCode[i]->instruction, ':') != NULL) {
                // It's a label or section directive
                // printf("║       │ %-84s ║\n", asmCode[i]->instruction);
            } else {
                // Regular instruction with line number
                // printf("║ %5d │ %-84s ║\n", i + 1, asmCode[i]->instruction);
            }
        }
    }

    // printf("╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║                                  Total Instructions: %-3d                                        ║\n", asm_count);
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    // Success message for target code generation       
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                         ✓ TARGET CODE GENERATION SUCCESSFUL                                     ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  x86-64 assembly code has been successfully generated from 3-address code.                      ║\n");
    // printf("║  Total Assembly Instructions: %-3d                                                              ║\n", asm_count);
    // printf("║  Target Architecture: x86-64                                                                    ║\n");
    // printf("║  Status: Ready for Assembly/Execution                                                           ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}

// Print real x86-64 assembly code (Fully Assemblable)
void printRealTargetCode() {
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                          REAL TARGET CODE (Assemblable x86-64)                                   ║\n");
    // printf("╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  #    │ Assembly Instruction                                                                     ║\n");
    // printf("╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    // External declarations for real assembly code
    extern AsmInstruction* realAsmCode[];
    extern int real_asm_count;

    if (real_asm_count == 0) {
        // printf("║                                  (No real target code generated)                                 ║\n");
    } else {
        for (int i = 0; i < real_asm_count; i++) {
            // Handle empty lines and labels (no line numbers for formatting)
            if (strlen(realAsmCode[i]->instruction) == 0) {
                // printf("║       │                                                                                      ║\n");
            } else if (realAsmCode[i]->instruction[0] != ' ' && 
                       realAsmCode[i]->instruction[0] != '\t' &&
                       (strchr(realAsmCode[i]->instruction, ':') != NULL || 
                        realAsmCode[i]->instruction[0] == '.')) {
                // It's a label or section directive
                // printf("║       │ %-84s ║\n", realAsmCode[i]->instruction);
            } else {
                // Regular instruction with line number
                // printf("║ %5d │ %-84s ║\n", i + 1, realAsmCode[i]->instruction);
            }
        }
    }

    // printf("╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║                                  Total Instructions: %-3d                                        ║\n", real_asm_count);
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    // Success message for real target code generation
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                      ✓ REAL TARGET CODE GENERATION SUCCESSFUL                                   ║\n");
    // printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    // printf("║  Fully assemblable x86-64 code has been generated with proper stack allocation.                ║\n");
    // printf("║  Total Assembly Instructions: %-3d                                                              ║\n", real_asm_count);
    // printf("║  Target Architecture: x86-64 (AT&T syntax)                                                     ║\n");
    // printf("║  Stack Allocated: Variables mapped to stack offsets                                            ║\n");
    // printf("║  Status: Ready for GCC Assembly (save as .s file and assemble)                                 ║\n");
    // printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    // printf("\n");
}
