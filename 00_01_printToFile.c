/**
 * 00_01_printToFile.c - File Output Functions
 * 
 * This module handles all detailed compiler output to the file
 * output/compiler_output.txt. This keeps the terminal clean while
 * providing comprehensive compilation details in a separate file.
 * 
 * Output includes:
 * - Token analysis (lexical phase)
 * - AST visualization (syntax phase)
 * - Symbol table (semantic phase)
 * - Three-address code (intermediate representation)
 * - Basic blocks and CFG (optimization phase)
 * - x86-64 assembly code (target code generation)
 * 
 * Author: Ridham Khurana
 */

// suppress format truncation warnings - buffers are sized correctly
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
// suppress address warnings - array checks are for element validity
#pragma GCC diagnostic ignored "-Waddress"

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global file pointer for output
static FILE* output_file = NULL;

// =====================================================
//          FILE OUTPUT INITIALIZATION
// =====================================================

void initializeOutputFile() {
    // use the global output_filename variable set from command line
    output_file = fopen(output_filename, "w");
    if (!output_file) {
        fprintf(stderr, "Error: Could not create output file %s\n", output_filename);
        exit(1);
    }
}

void closeOutputFile() {
    if (output_file) {
        fclose(output_file);
        output_file = NULL;
    }
}

// =====================================================
//          TOKEN PRINTING TO FILE SECTION 
// =====================================================

void printTokenToFile(Token* t, int index) {
    if (!t || !output_file) return;

    fprintf(output_file, "┌────────────────────────────────────┐\n");
    fprintf(output_file, "│ Token #%2d                         │\n", index + 1);
    fprintf(output_file, "├────────────────────────────────────┤\n");
    fprintf(output_file, "│ Type  : %-18s │\n", tokenTypeNames[t->token_type]);

    switch (t->token_type) {
        case VAL_INT:
            fprintf(output_file, "│ Value : %-18d │\n", t->int_value);
            break;
        case VAL_DOUBLE:
            fprintf(output_file, "│ Value : %-18.3lf │\n", t->double_value);
            break;
        case VAL_BOOL:
            fprintf(output_file, "│ Value : %-18s │\n", t->bool_value ? "true" : "false");
            break;
        case VAL_CHAR:
            fprintf(output_file, "│ Value : '%-16s' │\n", t->var_name);
            break;
        default:
            if (strlen(t->var_name) > 0)
                fprintf(output_file, "│ Value : %-18s │\n", t->var_name);
            else
                fprintf(output_file, "│ Value : (none)          │\n");
            break;
    }

    fprintf(output_file, "└────────────────────────────────────┘\n\n");
}

void printAllTokensToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                    LEXICAL ANALYSIS - TOKENS                                     ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");

    for (int i = 0; i < token_count; i++)
        printTokenToFile(tokens[i], i);

    fprintf(output_file, "═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    fprintf(output_file, "                                    END OF TOKEN LIST (Total: %d)\n", token_count);
    fprintf(output_file, "═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                              ✓ LEXICAL ANALYSIS SUCCESSFUL                                      ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  All tokens have been successfully generated and validated.                                     ║\n");
    fprintf(output_file, "║  Total Tokens Generated: %-3d                                                                   ║\n", token_count);
    fprintf(output_file, "║  Status: Ready for Parsing                                                                      ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// =====================================================
//          AST PRINTING TO FILE SECTION
// =====================================================

static void printIndentToFile(int indent) {
    if (!output_file) return;
    for (int i = 0; i < indent; i++)
        fprintf(output_file, "  ");
}

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

static const char* tokenTypeToString(tokenType type) {
    switch(type) {
        case KEYWORD_INT:    return "int";
        case KEYWORD_DOUBLE: return "double";
        case KEYWORD_CHAR:   return "char";
        case KEYWORD_BOOL:   return "bool";
        default:             return "unknown_type";
    }
}

void printASTNodeToFile(ASTNode* node, int indent) {
    if (!node || !output_file) return;

    printIndentToFile(indent);

    switch(node->type) {
        case AST_NUM:
            fprintf(output_file, "Num: %d\n", node->int_value);
            break;
        case AST_DOUBLE:
            fprintf(output_file, "Double: %f\n", node->double_value);
            break;
        case AST_BOOL:
            fprintf(output_file, "Bool: %s\n", node->bool_value ? "true" : "false");
            break;
        case AST_CHAR:
            fprintf(output_file, "Char: '%c'\n", node->char_value);
            break;
        case AST_STRING:
            fprintf(output_file, "String: \"%s\"\n", node->string_value);
            break;
        case AST_VAR:
            fprintf(output_file, "Var: %s\n", node->var.var_name);
            break;
        case AST_ARRAY_ACCESS:
            fprintf(output_file, "Array Access: %s[\n", node->array_access.array_name);
            printASTNodeToFile(node->array_access.sizeExpr, indent + 1);
            printIndentToFile(indent);
            fprintf(output_file, "]\n");
            break;
        case AST_UNOP:
            if (node->unop.op == OP_PLUS_PLUS || node->unop.op == OP_MINUS_MINUS) {
                fprintf(output_file, "UnaryOp: %s (%s)\n", 
                       opToString(node->unop.op),
                       node->unop.isPrefix ? "prefix" : "postfix");
            } else {
                fprintf(output_file, "UnaryOp: %s\n", opToString(node->unop.op));
            }
            printASTNodeToFile(node->unop.expr, indent + 1);
            break;
        case AST_BINOP:
            fprintf(output_file, "BinaryOp: %s\n", opToString(node->binop.op));
            printASTNodeToFile(node->binop.left, indent + 1);
            printASTNodeToFile(node->binop.right, indent + 1);
            break;
        case AST_ASSIGN:
            fprintf(output_file, "Assign:\n");
            printIndentToFile(indent + 1); fprintf(output_file, "Var:\n");
            printASTNodeToFile(node->assign.var, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Expr:\n");
            printASTNodeToFile(node->assign.expr, indent + 2);
            break;
        case AST_IF:
            fprintf(output_file, "If Statement:\n");
            printIndentToFile(indent + 1); fprintf(output_file, "Condition:\n");
            printASTNodeToFile(node->if_stmt.condition, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Then:\n");
            printASTNodeToFile(node->if_stmt.then_branch, indent + 2);
            break;
        case AST_IF_ELSE:
            fprintf(output_file, "If-Else Statement:\n");
            printIndentToFile(indent + 1); fprintf(output_file, "Condition:\n");
            printASTNodeToFile(node->if_else_stmt.condition, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Then:\n");
            printASTNodeToFile(node->if_else_stmt.then_branch, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Else:\n");
            printASTNodeToFile(node->if_else_stmt.else_branch, indent + 2);
            break;
        case AST_WHILE:
            fprintf(output_file, "While Loop:\n");
            printIndentToFile(indent + 1); fprintf(output_file, "Condition:\n");
            printASTNodeToFile(node->while_stmt.condition, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Body:\n");
            printASTNodeToFile(node->while_stmt.body, indent + 2);
            break;
        case AST_FOR:
            fprintf(output_file, "For Loop:\n");
            printIndentToFile(indent + 1); fprintf(output_file, "Init:\n");
            printASTNodeToFile(node->for_stmt.init, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Condition:\n");
            printASTNodeToFile(node->for_stmt.condition, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Update:\n");
            printASTNodeToFile(node->for_stmt.update, indent + 2);
            printIndentToFile(indent + 1); fprintf(output_file, "Body:\n");
            printASTNodeToFile(node->for_stmt.body, indent + 2);
            break;
        case AST_BLOCK:
            fprintf(output_file, "Block (%d statements):\n", node->block.statement_count);
            for (int i = 0; i < node->block.statement_count; i++)
                printASTNodeToFile(node->block.statements[i], indent + 1);
            break;
        case AST_PROGRAM:
            fprintf(output_file, "Program (%d top-level statements):\n", node->program.statement_count);
            for (int i = 0; i < node->program.statement_count; i++)
                printASTNodeToFile(node->program.statements[i], indent + 1);
            break;
        case AST_DECL:
            if (node->decl.is_array)
                fprintf(output_file, "Declaration: %s[%s] (%s)\n",
                       node->decl.var_name,
                       node->decl.array_size,
                       tokenTypeToString(node->decl.type));
            else
                fprintf(output_file, "Declaration: %s (%s)\n",
                       node->decl.var_name,
                       tokenTypeToString(node->decl.type));

            if (node->decl.init_expr) {
                printIndentToFile(indent + 1);
                fprintf(output_file, "Initializer:\n");
                printASTNodeToFile(node->decl.init_expr, indent + 2);
            }
            break;
        default:
            fprintf(output_file, "Unknown AST Node\n");
    }
}

void printAllASTNodesToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                 ABSTRACT SYNTAX TREE (AST)                                       ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    for (int i = 0; i < ast_count-1; i++) {
        fprintf(output_file, "\n");
        fprintf(output_file, "─────────────────────────────────────────────────────────────────────────────────────────────────\n");
        fprintf(output_file, "  AST Node #%d\n", i + 1);
        fprintf(output_file, "─────────────────────────────────────────────────────────────────────────────────────────────────\n");

        printASTNodeToFile(all_ast[i], 0);
    }

    fprintf(output_file, "\n");
    fprintf(output_file, "═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    fprintf(output_file, "                                  END OF AST (Total Nodes: %d)\n", ast_count);
    fprintf(output_file, "═══════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                              ✓ AST GENERATION SUCCESSFUL                                        ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Abstract Syntax Tree has been successfully generated and validated.                            ║\n");
    fprintf(output_file, "║  Total AST Nodes Created: %-3d                                                                  ║\n", ast_count);
    fprintf(output_file, "║  Status: Ready for Semantic Analysis                                                            ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// =====================================================
//          SYMBOL TABLE PRINTING TO FILE
// =====================================================

void printSymbolTableToFile() {
    if (!output_file) return;
    
    int maxBlockId = 0;
    for (int i = 0; i < symbol_count; i++) {
        if (symbolTable[i]->blockId > maxBlockId) {
            maxBlockId = symbolTable[i]->blockId;
        }
    }
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                           ✓ SEMANTIC ANALYSIS COMPLETED                                         ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Type checking and semantic validation completed successfully.                                  ║\n");
    fprintf(output_file, "║  Total Symbols in Table: %-3d                                                                   ║\n", symbol_count);
    fprintf(output_file, "║  Total Block Scopes: %-3d                                                                       ║\n", maxBlockId + 1);
    fprintf(output_file, "║  Total Warnings: %-3d                                                                           ║\n", warning_count);
    
    if (warning_count > 0) {
        fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
        fprintf(output_file, "║  Warnings:                                                                                      ║\n");
        
        for (int i = 0; i < warning_count; i++) {
            char warning_copy[256];
            strncpy(warning_copy, all_warnings[i], 255);
            warning_copy[255] = '\0';
            
            char *line = strtok(warning_copy, "\n");
            int line_num = 0;
            
            while (line != NULL) {
                if (line_num == 0) {
                    fprintf(output_file, "║  %d. %-91s  ║\n", i + 1, line);
                } else {
                    fprintf(output_file, "║     %-91s  ║\n", line);
                }
                line = strtok(NULL, "\n");
                line_num++;
            }
        }
    }
    
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Status: Ready for Code Generation                                                              ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
    
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                      SYMBOL TABLE CONTENTS                                       ║\n");
    fprintf(output_file, "╠═══════╤══════════════════╤════════════════╤════════╤══════════╤══════════╤═══════╤══════════════╣\n");
    fprintf(output_file, "║  #    │ Name             │ Type           │ Array  │ Size     │ Init     │ Scope │ Block ID     ║\n");
    fprintf(output_file, "╠═══════╪══════════════════╪════════════════╪════════╪══════════╪══════════╪═══════╪══════════════╣\n");

    if (symbol_count == 0) {
        fprintf(output_file, "║                                         (Symbol table is empty)                                 ║\n");
    } else {
        for (int i = 0; i < symbol_count; i++) {
            fprintf(output_file, "║ %3d   │ %-16s │ %-14s │ %-6s │ %-8s │ %-8s │ %5d │ %10d   ║\n",
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

    fprintf(output_file, "╠═══════╧══════════════════╧════════════════╧════════╧══════════╧══════════╧═══════╧══════════════╣\n");
    fprintf(output_file, "║                                        Total Symbols: %-3d                                       ║\n", symbol_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

// =====================================================
//          3-ADDRESS CODE PRINTING TO FILE
// =====================================================

void print3AddressCodeToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                   THREE-ADDRESS CODE (TAC)                                       ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  #    │ Instruction                                                                              ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (addr_count == 0) {
        fprintf(output_file, "║                                    (No 3-address code generated)                                 ║\n");
    } else {
        for (int i = 0; i < addr_count; i++) {
            address* addr = allAddress[i];
            char instruction[256] = "";
            
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
            
            fprintf(output_file, "║ %5d │ %-84s ║\n", i + 1, instruction);
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                  Total Instructions: %-3d                                        ║\n", addr_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                        ✓ THREE-ADDRESS CODE GENERATION SUCCESSFUL                               ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Intermediate code has been successfully generated from the AST.                                ║\n");
    fprintf(output_file, "║  Total Instructions Generated: %-3d                                                             ║\n", addr_count);
    fprintf(output_file, "║  Status: Ready for Optimization/Target Code Generation                                         ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// new function: print optimized 3-address code with instruction count comparison
void print3AddressCodeAfterOptimizationToFile(int before_count, int after_count) {
    if (!output_file) return;
    
    // calculate reduction statistics
    int instructions_eliminated = before_count - after_count;
    double reduction_percentage = 0.0;
    if (before_count > 0) {
        reduction_percentage = ((double)instructions_eliminated / (double)before_count) * 100.0;
    }
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                            THREE-ADDRESS CODE (After Optimization)                               ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  #    │ Instruction                                                                              ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (addr_count == 0) {
        fprintf(output_file, "║                                    (No 3-address code remaining)                                 ║\n");
    } else {
        for (int i = 0; i < addr_count; i++) {
            address* addr = allAddress[i];
            char instruction[256] = "";
            
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
            
            fprintf(output_file, "║ %5d │ %-84s ║\n", i + 1, instruction);
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                  Total Instructions: %-3d                                        ║\n", addr_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                             ✓ CODE OPTIMIZATION COMPLETED                                       ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                                                                                  ║\n");
    fprintf(output_file, "║  Optimization Statistics:                                                                       ║\n");
    fprintf(output_file, "║  ├─ Instructions Before Optimization:  %-3d                                                     ║\n", before_count);
    fprintf(output_file, "║  ├─ Instructions After Optimization:   %-3d                                                     ║\n", after_count);
    fprintf(output_file, "║  ├─ Instructions Eliminated:           %-3d                                                     ║\n", instructions_eliminated);
    fprintf(output_file, "║  └─ Code Size Reduction:               %.2f%%                                                  ║\n", reduction_percentage);
    fprintf(output_file, "║                                                                                                  ║\n");
    fprintf(output_file, "║  Optimizations Applied:                                                                         ║\n");
    fprintf(output_file, "║  • Constant Folding                                                                             ║\n");
    fprintf(output_file, "║  • Constant Propagation                                                                         ║\n");
    fprintf(output_file, "║  • Copy Propagation                                                                             ║\n");
    fprintf(output_file, "║  • Dead Code Elimination (DCE)                                                                  ║\n");
    fprintf(output_file, "║  • Live Variable Analysis                                                                       ║\n");
    fprintf(output_file, "║                                                                                                  ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// =====================================================
//          BASIC BLOCKS PRINTING TO FILE
// =====================================================

// Helper function to print address sets (GEN/KILL/IN/OUT) that may contain deleted instructions
static void printAddressSet(const char* label, address** set, int count) {
    if (!output_file) return;
    
    fprintf(output_file, "║       │     ➢ %s: ", label);
    if (count > 0) {
        fprintf(output_file, "{ ");
        int charCount = 0;
        int printedCount = 0;
        for (int j = 0; j < count; j++) {
            if (set[j]) {
                char str[16] = "";
                address* addr = set[j];
                int lineNum = -1;
                // Search for the address in the global address array
                for (int k = 0; k < addr_count; k++) {
                    if (allAddress[k] == addr) {
                        lineNum = k + 1;
                        break;
                    }
                }
                // Only print if instruction still exists in allAddress
                if (lineNum != -1) {
                    snprintf(str, 16, "%d", lineNum);
                    
                    int len = strlen(str) + (printedCount > 0 ? 2 : 0);
                    if (charCount + len > 60 && charCount > 0) {
                        fprintf(output_file, "\n║       │            ");
                        charCount = 0;
                    }
                    if (printedCount > 0) fprintf(output_file, ", ");
                    fprintf(output_file, "%s", str);
                    charCount += len;
                    printedCount++;
                }
            }
        }
        if (printedCount > 0) {
            fprintf(output_file, " }");
        } else {
            fprintf(output_file, "∅ }");
        }
    } else {
        fprintf(output_file, "∅");
    }
    fprintf(output_file, "\n");
}

void printBlocksBeforeOptimizationToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                              BASIC BLOCKS (Before Optimization)                                  ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║ Block │ Instructions                                                                             ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (block_count == 0) {
        fprintf(output_file, "║                                    (No blocks generated)                                         ║\n");
    } else {
        for (int i = 0; i < block_count; i++) {
            block* blk = allBlocks[i];
            if (!blk) continue;

            fprintf(output_file, "║ B%-4d │                                                                                      ║\n", blk->blockID);
            fprintf(output_file, "║       │ Instructions: %-3d                                                                   ║\n", blk->numberOfAddressesInBlock);

            if (blk->numCFGIn > 0 && blk->cfg_in) {
                fprintf(output_file, "║       │ In: ");
                for (int j = 0; j < blk->numCFGIn; j++) {
                    if (blk->cfg_in[j]) fprintf(output_file, "B%d", blk->cfg_in[j]->blockID);
                    if (j < blk->numCFGIn - 1) fprintf(output_file, ", ");
                }
                fprintf(output_file, "\n");
            } else {
                fprintf(output_file, "║       │ In: (none)                                                                           ║\n");
            }

            if (blk->numCFGOut > 0 && blk->cfg_out) {
                fprintf(output_file, "║       │ Out: ");
                for (int j = 0; j < blk->numCFGOut; j++) {
                    if (blk->cfg_out[j]) fprintf(output_file, "B%d", blk->cfg_out[j]->blockID);
                    if (j < blk->numCFGOut - 1) fprintf(output_file, ", ");
                }
                fprintf(output_file, "\n");
            } else {
                fprintf(output_file, "║       │ Out: (none)                                                                          ║\n");
            }

            fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");

            for (int j = 0; j < blk->numberOfAddressesInBlock; j++) {
                address* addr = blk->list[j];
                char instruction[256] = "";
                if (!addr) continue;

                int globalLineNum = -1;
                for (int k = 0; k < addr_count; k++) {
                    if (allAddress[k] == addr) {
                        globalLineNum = k + 1;
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
                    fprintf(output_file, "║       │  [%2d] ◦ [%2d] %-68s ║\n", globalLineNum, j + 1, instruction);
                } else {
                    fprintf(output_file, "║       │  [??] ◦ [%2d] %-68s ║\n", j + 1, instruction);
                }
            }

            // Print GEN/KILL/IN/OUT if available
            if (i < block_prop_count && allBlockProps[i]) {
                blockProp* prop = allBlockProps[i];
                
                fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");
                fprintf(output_file, "║       │ A) Reaching Definitions                                                              ║\n");
                
                printAddressSet("GEN", prop->gen, prop->numGen);
                printAddressSet("KILL", prop->kill, prop->numKill);
                printAddressSet("IN", prop->in, prop->numIn);
                printAddressSet("OUT", prop->out, prop->numOut);
            }
            
            // Print Live Variable Analysis if available
            if (i < block_prop_live_count && allBlockPropsLive[i]) {
                blockPropLive* propLive = allBlockPropsLive[i];
                
                fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");
                fprintf(output_file, "║       │ B) Live Variable Analysis                                                            ║\n");
                
                // GEN set (variables used before being defined)
                fprintf(output_file, "║       │     ➢ GEN: ");
                if (propLive->numGenLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numGenLive; j++) {
                        if (propLive->genLive[j]) {
                            int len = strlen(propLive->genLive[j]) + (j < propLive->numGenLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->genLive[j]);
                            charCount += len;
                            if (j < propLive->numGenLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // KILL set (variables defined)
                fprintf(output_file, "║       │     ➢ KILL: ");
                if (propLive->numKillLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numKillLive; j++) {
                        if (propLive->killLive[j]) {
                            int len = strlen(propLive->killLive[j]) + (j < propLive->numKillLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->killLive[j]);
                            charCount += len;
                            if (j < propLive->numKillLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // IN set (live variables at entry)
                fprintf(output_file, "║       │     ➢ IN: ");
                if (propLive->numInLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numInLive; j++) {
                        if (propLive->inLive[j]) {
                            int len = strlen(propLive->inLive[j]) + (j < propLive->numInLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->inLive[j]);
                            charCount += len;
                            if (j < propLive->numInLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // OUT set (live variables at exit)
                fprintf(output_file, "║       │     ➢ OUT: ");
                if (propLive->numOutLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numOutLive; j++) {
                        if (propLive->outLive[j]) {
                            int len = strlen(propLive->outLive[j]) + (j < propLive->numOutLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->outLive[j]);
                            charCount += len;
                            if (j < propLive->numOutLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
            }

            if (i < block_count - 1) {
                fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");
            }
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                    Total Blocks: %-3d                                            ║\n", block_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// new function: print control flow graph only (without gen/kill/in/out analysis)
void printCFGOnlyToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                                    CONTROL FLOW GRAPH (CFG)                                      ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║ Block │ Details                                                                                  ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (block_count == 0) {
        fprintf(output_file, "║                                    (No blocks generated)                                         ║\n");
    } else {
        for (int i = 0; i < block_count; i++) {
            block* blk = allBlocks[i];
            if (!blk) continue;

            fprintf(output_file, "║ B%-4d │                                                                                      ║\n", blk->blockID);
            fprintf(output_file, "║       │ Instructions: %-3d                                                                   ║\n", blk->numberOfAddressesInBlock);

            // print cfg_in
            if (blk->numCFGIn > 0) {
                fprintf(output_file, "║       │ CFG In:  ");
                for (int j = 0; j < blk->numCFGIn; j++) {
                    if (blk->cfg_in[j]) fprintf(output_file, "B%d", blk->cfg_in[j]->blockID);
                    if (j < blk->numCFGIn - 1) fprintf(output_file, ", ");
                }
                for (int pad = 0; pad < 70; pad++) fprintf(output_file, " ");
                fprintf(output_file, "║\n");
            } else {
                fprintf(output_file, "║       │ CFG In:  (none)                                                                      ║\n");
            }

            // print cfg_out
            if (blk->numCFGOut > 0) {
                fprintf(output_file, "║       │ CFG Out: ");
                for (int j = 0; j < blk->numCFGOut; j++) {
                    if (blk->cfg_out[j]) fprintf(output_file, "B%d", blk->cfg_out[j]->blockID);
                    if (j < blk->numCFGOut - 1) fprintf(output_file, ", ");
                }
                for (int pad = 0; pad < 70; pad++) fprintf(output_file, " ");
                fprintf(output_file, "║\n");
            } else {
                fprintf(output_file, "║       │ CFG Out: (none)                                                                      ║\n");
            }

            fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");

            // print all instructions in the block
            for (int j = 0; j < blk->numberOfAddressesInBlock; j++) {
                address* addr = blk->list[j];
                if (!addr) continue;

                char instruction[256];
                switch (addr->type) {
                    case ADDR_LABEL:
                        snprintf(instruction, 256, "L%s:", addr->label.labelNumber);
                        break;
                    case ADDR_GOTO:
                        snprintf(instruction, 256, "goto L%s", addr->goto_stmt.target);
                        break;
                    case ADDR_ASSIGN:
                        snprintf(instruction, 256, "%s = %s", addr->assign.result, addr->assign.arg1);
                        break;
                    case ADDR_BINOP:
                        snprintf(instruction, 256, "%s = %s %s %s", addr->binop.result, addr->binop.arg1, addr->binop.op, addr->binop.arg2);
                        break;
                    case ADDR_UNOP:
                        snprintf(instruction, 256, "%s = %s %s", addr->unop.result, addr->unop.op, addr->unop.arg1);
                        break;
                    case ADDR_IF_F_GOTO:
                        snprintf(instruction, 256, "ifFalse %s goto L%s", addr->if_false.condition, addr->if_false.target);
                        break;
                    case ADDR_IF_T_GOTO:
                        snprintf(instruction, 256, "ifTrue %s goto L%s", addr->if_true.condition, addr->if_true.target);
                        break;
                    case ADDR_ARRAY_READ:
                        snprintf(instruction, 256, "%s = %s[%s]", addr->array_read.result, addr->array_read.array, addr->array_read.index);
                        break;
                    case ADDR_ARRAY_WRITE:
                        snprintf(instruction, 256, "%s[%s] = %s", addr->array_write.array, addr->array_write.index, addr->array_write.value);
                        break;
                    default:
                        snprintf(instruction, 256, "(unknown instruction)");
                }

                fprintf(output_file, "║       │   %2d.  %-80s ║\n", j+1, instruction);
            }

            if (i < block_count - 1) {
                fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");
            }
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                    Total Blocks: %-3d                                            ║\n", block_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

void printBlocksToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                              BASIC BLOCKS (After Optimization)                                   ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║ Block │ Instructions                                                                             ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    if (block_count == 0) {
        fprintf(output_file, "║                                    (No blocks generated)                                         ║\n");
    } else {
        for (int i = 0; i < block_count; i++) {
            block* blk = allBlocks[i];
            if (!blk) continue;

            fprintf(output_file, "║ B%-4d │                                                                                      ║\n", blk->blockID);
            fprintf(output_file, "║       │ Instructions: %-3d                                                                   ║\n", blk->numberOfAddressesInBlock);

            if (blk->numCFGIn > 0 && blk->cfg_in) {
                fprintf(output_file, "║       │ In: ");
                for (int j = 0; j < blk->numCFGIn; j++) {
                    if (blk->cfg_in[j]) fprintf(output_file, "B%d", blk->cfg_in[j]->blockID);
                    if (j < blk->numCFGIn - 1) fprintf(output_file, ", ");
                }
                fprintf(output_file, "\n");
            } else {
                fprintf(output_file, "║       │ In: (none)                                                                           ║\n");
            }

            if (blk->numCFGOut > 0 && blk->cfg_out) {
                fprintf(output_file, "║       │ Out: ");
                for (int j = 0; j < blk->numCFGOut; j++) {
                    if (blk->cfg_out[j]) fprintf(output_file, "B%d", blk->cfg_out[j]->blockID);
                    if (j < blk->numCFGOut - 1) fprintf(output_file, ", ");
                }
                fprintf(output_file, "\n");
            } else {
                fprintf(output_file, "║       │ Out: (none)                                                                          ║\n");
            }

            fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");

            for (int j = 0; j < blk->numberOfAddressesInBlock; j++) {
                address* addr = blk->list[j];
                char instruction[256] = "";
                if (!addr) continue;

                int globalLineNum = -1;
                for (int k = 0; k < addr_count; k++) {
                    if (allAddress[k] == addr) {
                        globalLineNum = k + 1;
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
                    fprintf(output_file, "║       │  [%2d] ◦ [%2d] %-68s ║\n", globalLineNum, j + 1, instruction);
                } else {
                    fprintf(output_file, "║       │  [??] ◦ [%2d] %-68s ║\n", j + 1, instruction);
                }
            }

            if (i < block_prop_count && allBlockProps[i]) {
                blockProp* prop = allBlockProps[i];
                
                fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");
                fprintf(output_file, "║       │ A) Reaching Definitions                                                              ║\n");
                
                printAddressSet("GEN", prop->gen, prop->numGen);
                printAddressSet("KILL", prop->kill, prop->numKill);
                printAddressSet("IN", prop->in, prop->numIn);
                printAddressSet("OUT", prop->out, prop->numOut);
            }
            
            // Print Live Variable Analysis if available
            if (i < block_prop_live_count && allBlockPropsLive[i]) {
                blockPropLive* propLive = allBlockPropsLive[i];
                
                fprintf(output_file, "║       ├──────────────────────────────────────────────────────────────────────────────────────║\n");
                fprintf(output_file, "║       │ B) Live Variable Analysis                                                            ║\n");
                
                // GEN set (variables used before being defined)
                fprintf(output_file, "║       │     ➢ GEN: ");
                if (propLive->numGenLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numGenLive; j++) {
                        if (propLive->genLive[j]) {
                            int len = strlen(propLive->genLive[j]) + (j < propLive->numGenLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->genLive[j]);
                            charCount += len;
                            if (j < propLive->numGenLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // KILL set (variables defined)
                fprintf(output_file, "║       │     ➢ KILL: ");
                if (propLive->numKillLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numKillLive; j++) {
                        if (propLive->killLive[j]) {
                            int len = strlen(propLive->killLive[j]) + (j < propLive->numKillLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->killLive[j]);
                            charCount += len;
                            if (j < propLive->numKillLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // IN set (live variables at entry)
                fprintf(output_file, "║       │     ➢ IN: ");
                if (propLive->numInLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numInLive; j++) {
                        if (propLive->inLive[j]) {
                            int len = strlen(propLive->inLive[j]) + (j < propLive->numInLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->inLive[j]);
                            charCount += len;
                            if (j < propLive->numInLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
                
                // OUT set (live variables at exit)
                fprintf(output_file, "║       │     ➢ OUT: ");
                if (propLive->numOutLive > 0) {
                    fprintf(output_file, "{ ");
                    int charCount = 0;
                    for (int j = 0; j < propLive->numOutLive; j++) {
                        if (propLive->outLive[j]) {
                            int len = strlen(propLive->outLive[j]) + (j < propLive->numOutLive - 1 ? 2 : 0);
                            if (charCount + len > 60 && charCount > 0) {
                                fprintf(output_file, "\n║       │            ");
                                charCount = 0;
                            }
                            fprintf(output_file, "%s", propLive->outLive[j]);
                            charCount += len;
                            if (j < propLive->numOutLive - 1) fprintf(output_file, ", ");
                        }
                    }
                    fprintf(output_file, " }");
                } else {
                    fprintf(output_file, "∅");
                }
                fprintf(output_file, "\n");
            }

            if (i < block_count - 1) {
                fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");
            }
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                    Total Blocks: %-3d                                            ║\n", block_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                              ✓ OPTIMIZATION COMPLETED SUCCESSFULLY                              ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Optimizations Applied:                                                                         ║\n");
    fprintf(output_file, "║    • Constant Propagation                                                                       ║\n");
    fprintf(output_file, "║    • Constant Folding                                                                           ║\n");
    fprintf(output_file, "║    • Copy Propagation                                                                           ║\n");
    fprintf(output_file, "║  Total Blocks: %-3d                                                                             ║\n", block_count);
    fprintf(output_file, "║  Status: Ready for Target Code Generation                                                      ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}

// =====================================================
//          TARGET CODE PRINTING TO FILE
// =====================================================

void printRealTargetCodeToFile() {
    if (!output_file) return;
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                          REAL TARGET CODE (Assemblable x86-64)                                   ║\n");
    fprintf(output_file, "╠═══════╤══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  #    │ Assembly Instruction                                                                     ║\n");
    fprintf(output_file, "╠═══════╪══════════════════════════════════════════════════════════════════════════════════════════╣\n");

    extern AsmInstruction* realAsmCode[];
    extern int real_asm_count;

    if (real_asm_count == 0) {
        fprintf(output_file, "║                                  (No real target code generated)                                 ║\n");
    } else {
        for (int i = 0; i < real_asm_count; i++) {
            if (strlen(realAsmCode[i]->instruction) == 0) {
                fprintf(output_file, "║       │                                                                                      ║\n");
            } else if (realAsmCode[i]->instruction[0] != ' ' && 
                       realAsmCode[i]->instruction[0] != '\t' &&
                       (strchr(realAsmCode[i]->instruction, ':') != NULL || 
                        realAsmCode[i]->instruction[0] == '.')) {
                fprintf(output_file, "║       │ %-84s ║\n", realAsmCode[i]->instruction);
            } else {
                fprintf(output_file, "║ %5d │ %-84s ║\n", i + 1, realAsmCode[i]->instruction);
            }
        }
    }

    fprintf(output_file, "╠═══════╧══════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║                                  Total Instructions: %-3d                                        ║\n", real_asm_count);
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    fprintf(output_file, "\n");
    fprintf(output_file, "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    fprintf(output_file, "║                      ✓ REAL TARGET CODE GENERATION SUCCESSFUL                                   ║\n");
    fprintf(output_file, "╠══════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    fprintf(output_file, "║  Fully assemblable x86-64 code has been generated with proper stack allocation.                ║\n");
    fprintf(output_file, "║  Total Assembly Instructions: %-3d                                                              ║\n", real_asm_count);
    fprintf(output_file, "║  Target Architecture: x86-64 (AT&T syntax)                                                     ║\n");
    fprintf(output_file, "║  Stack Allocated: Variables mapped to stack offsets                                            ║\n");
    fprintf(output_file, "║  Status: Ready for GCC Assembly (save as .s file and assemble)                                 ║\n");
    fprintf(output_file, "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    fprintf(output_file, "\n");
}
