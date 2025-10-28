#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ------------------- Token print helpers -------------------

// print a single token
void printToken(Token* t) {
    if (!t) return;
    
    printf("Type: %s\n", tokenTypeNames[t->token_type]);

    switch(t->token_type) {
        case VAL_INT:
            printf("value: %d\n", t->int_value);
            break;
        case VAL_DOUBLE:
            printf("value: %lf\n", t->double_value);
            break;
        case VAL_BOOL:
            printf("value: %s\n", t->bool_value ? "true" : "false");
            break;
        case VAL_CHAR:
            printf("value: ->%s<-\n", t->var_name);
            break;
        default:
            // keywords, identifiers, operators, punctuation
            printf("value: %s\n", t->var_name);
            break;
    }

    printf("\n");
}

// print all global tokens
void printAllTokens() {
    for (int i = 0; i < token_count; i++) {
        printf("Token-%d\n", i+1);   // optional: show token index
        printToken(tokens[i]);
    }
}

// printing ast 
#include <stdio.h>

void printIndent(int indent) {
    for (int i = 0; i < indent; i++)
        printf("  "); // 2 spaces per indent
}

// Map tokenType operators to string
const char* opToString(tokenType op) {
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

// tooken type to string for delcations ONLY
char* tokenTypeToString(tokenType type) {
    switch(type) {
        case KEYWORD_INT:    return "int";
        case KEYWORD_DOUBLE: return "double";
        case KEYWORD_CHAR:   return "char";
        case KEYWORD_BOOL:   return "bool";
        // Add other types if needed
        default:           return "unknown_type";
    }
}

// Recursive AST printer
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





// ------------------- Main -------------------

int main(void) {
    // File name to read
    char file_name[MAX_NAME];
    strcpy(file_name, "tester/test.c"); // hard-coded for testing
    // scanf("%s", file_name); // optional: read from user

    // Generate tokens
    generateTokens(file_name);    

    // Print all tokens
    printAllTokens();
    printf("Token generation and printing succesful...\n\n");

    generateAllASTNodes();

    ASTNode* root = all_ast[0];  // assuming first AST node is your program
    printASTNode(root, 0);



    

    exit(0);
}
