/**
 * 03_semanticCheck.c - Semantic Analyzer
 * 
 * This module performs semantic analysis on the AST, ensuring type safety,
 * scope correctness, and other semantic rules are followed.
 * 
 * Features:
 * - Type checking for all expressions and statements
 * - Variable declaration and scope validation
 * - Symbol table construction and management
 * - Array bounds and initialization checking
 * - Operator compatibility verification
 * - Warning generation for unused variables
 * 
 * Author: Ridham Khurana
 */

#include "database.h"

// helping variable used to do type checking
int currScope = 0;
int currBlockId = 0;

int stackId[MAX] = {-1};
int stackCount = 1;

char tempWarningContainer[MAX];

// add warning to all warning
void addWarning(char* warning){
    if(warning_count >= MAX){
        printf("Warning buffer is full, cannot add more warnings\n");
        return;
    }
    
    // Allocate memory for the new warning string
    all_warnings[warning_count] = (char*)malloc(strlen(warning) + 1);
    
    if(all_warnings[warning_count] == NULL){
        printf("Memory allocation failed for warning\n");
        return;
    }
    
    // Copy the warning message
    strcpy(all_warnings[warning_count], warning);
    
    // Increment warning count
    warning_count++;
}


// chech if the current symbol block is in the block_stack
bool isBlockIdInTheStack(int tempBlock){
    for(int i=0 ; i<stackCount ; i++){
        if(stackId[i] == tempBlock) return true;
    }

    return false;
}

// check if symbol is initalized
bool isSymbolInitalized(char* name){
    for(int i=0 ; i<symbol_count ; i++){
        symbol* store = symbolTable[i];
        if(isStringSame(name , store->var_name) && !store->isInitialized && currScope >= store->scope  && isBlockIdInTheStack(store->blockId)){ // if var exists and is initialized
            return false;
        }
    }

    return true;
}

// get type of symbol from symbol table
tokenType getTypeOfSymbolFromName(char* name , bool isArray){  

    for(int i=0 ; i<symbol_count ; i++){
        symbol* store = symbolTable[i];
        if(isStringSame(name , store->var_name) && isArray == store->isArray && currScope >= store->scope  && isBlockIdInTheStack(store->blockId)){ // if var name ,  type and isArray matched, then symbol exists in symbol table
            return store->type;
        }
    }

    printf("Symbol does NOT exists\n\n");
    exit(3);
}

// Check if a symbol is declared and accessible from the current scope
// Variables are accessible if declared in current or outer scopes (currScope >= store->scope)
bool isSymbolDeclared(char* name , bool isArray){
    for(int i=0 ; i<symbol_count ; i++){
        symbol* store = symbolTable[i];
        if(isStringSame(name , store->var_name) && isArray == store->isArray && currScope >= store->scope && isBlockIdInTheStack(store->blockId)){ // if var name ,  type and isArray matched, then symbol exists in symbol table
            return true;
        }
    }


    return false; // symbol does NOT exists
}

// is given string a number
bool isStringANumber(char* arrSize){
    int i = 0;
    while(arrSize[i] != '\0'){
        if(!isdigit(arrSize[i])) return false;
        i++;
    }

    return true;
}

// add symbol to symbol table
symbol* addSymbol(char* name , tokenType type , bool isArray , char* arrSize , bool isInitialized , int scope , int blockId){
    (void)blockId; // unused parameter
    
    if(isSymbolDeclared(name , isArray)){ // symbol already declared in the scope
        printf("03 || Semantic Check error [03.22] -> Variable (%s) already exists in current scope\n" , name);
        exit(3);
    }

    if(isArray){ // declarion of array
        if(!isStringANumber(arrSize)){ // index is variable
            if(!isSymbolDeclared(arrSize , false)){ // index variable is NOT in symbol table
                printf("03 || Semantic Check error [03.23] -> Index symbol (%s) in undefined\n", arrSize);
                exit(3);
            }
            if(getTypeOfSymbolFromName(arrSize , false) != KEYWORD_INT){ // the type of index variable should be int
                printf("03 || Semantic Check error [03.24] -> Index symbol (%s) MUST be of type INT\n", arrSize);
                exit(3);
            }
            if(!isSymbolInitalized(arrSize)){ // give warning is the symbol is uninitialized [maybe assined later]
                strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.25] -> Index Symbol (%s) is NOT initialized\n");
                addWarning(tempWarningContainer);
            }
        }
    }

    symbol* temp = (symbol*)malloc(sizeof(symbol)); // allocate memory for new symbol

    // set the details
    strcpy(temp->var_name , name);
    temp->type = type;
    temp->isInitialized = isInitialized;
    temp->scope = scope; 
    if(scope == 0){ // if scope = 0 , then blockId MUST be 0
        temp->blockId = 0;
    } else{ // else
        temp->blockId = currBlockId;        
    }
    temp->isArray = isArray;
    if(isArray) strcpy(temp->arraySize , arrSize); // set only if variable is array

    // no space in symbol table
    if(symbol_count > MAX){
        printf("Symbol table is full\n\n");
        exit(3);
    }

    // add symbol to symbol table
    symbolTable[symbol_count++] = temp;

    return temp; // return symbol
}

tokenType getOutputTokenTypeOfAST(ASTNode* top); // declarion ONLY
void checkAST(ASTNode* top); // declaration ONLY

// verify if the expr matches with the supposed data type
bool checkIfTokenTypeMatch(tokenType type , ASTNode* expr){
    if(type == getOutputTokenTypeOfAST(expr)) return true; // if given type matches with expr type, return true

    return false; // type didnt match, hence return false
}

// get binary operand code
int getCodeOfBinOp(tokenType type){
    switch(type){
        case OP_PLUS :
        case OP_MINUS : 
        case OP_MUL:
        case OP_DIV: 
            return 1; // return 1 for + - * /

        case OP_IS_EQUAL:
        case OP_NOT_EQUAL:
        case OP_GREAT_THAN:
        case OP_LESS_THAN:
        case OP_GREAT_EQUAL:
        case OP_LESS_EQUAL:
            return 2; // return 2 for == != > < >= <=

        case OP_AND_AND:
        case OP_OR_OR:
            return 3; // return 3 for && ||

        default : return -1; // return -1 if something else
    }
}

// get Unary operand code
int getCodeOfUnOp(tokenType type){
    switch(type){
        case OP_NOT:
            return 1; // return 1 for !

        case OP_PLUS_PLUS:
        case OP_MINUS_MINUS:
            return 2; // return 2 for ++ --

        default: 
            return -1; // return -1 if something else
    }
}

// get final tokenType of the AST
tokenType getOutputTokenTypeOfAST(ASTNode* top){
    if(!top){
        printf("03 || Semantic Check error [03.10] -> AST is NOT accessible\n");
        exit(3);
    }

    if(top->type == AST_NUM){ // Integer node
        return KEYWORD_INT;
    } else if(top->type == AST_DOUBLE){ // Double node
        return KEYWORD_DOUBLE;
    } else if(top->type == AST_CHAR){ // Char node
        return KEYWORD_CHAR;
    } else if(top->type == AST_BOOL){ // Bool node
        return KEYWORD_BOOL;
    } else if(top->type == AST_VAR){ // Variable Node
        return getTypeOfSymbolFromName(top->var.var_name , false); // return the type of var from symbol table
    } else if(top->type == AST_ARRAY_ACCESS){ // Array foundd
        if(!checkIfTokenTypeMatch(KEYWORD_INT , top->array_access.sizeExpr)){
            printf("03 || Semantic Check error [03.11] -> Index of array MUST be int\n");
            exit(3);
        }
        return getTypeOfSymbolFromName(top->array_access.array_name , true); // return date type of array from symbol table
    } else if(top->type == AST_BINOP){ // Binary node found
        tokenType left = getOutputTokenTypeOfAST(top->binop.left); // get type of left side of binary node
        tokenType right = getOutputTokenTypeOfAST(top->binop.right); // get type of right side of binary node

        int check = getCodeOfBinOp(top->binop.op); // get the code of binary operator

        if(check == 1){ // found + - * /
            if((left != KEYWORD_INT && left != KEYWORD_DOUBLE) || (right != KEYWORD_INT && right != KEYWORD_DOUBLE)){ // left and right must be either int/double
                printf("03 || Semantic Check error [03.12] -> Expected INT/DOUBLE around binop\n");
                exit(3);                                
            }
            
            // Check for division by zero
            if(top->binop.op == OP_DIV){
                if(top->binop.right->type == AST_NUM && top->binop.right->int_value == 0){
                    printf("03 || Semantic Check error [03.22] -> Division by zero detected\n");
                    exit(3);
                } else if(top->binop.right->type == AST_DOUBLE && top->binop.right->double_value == 0.0){
                    printf("03 || Semantic Check error [03.22] -> Division by zero detected\n");
                    exit(3);
                }
            }
            
            if(left == KEYWORD_INT && right == KEYWORD_INT) return KEYWORD_INT; // both are int
            
            return KEYWORD_DOUBLE; // either both are int or one is int and one is double
        } else if(check == 2){ // found == != > < >= <=
            if((left != KEYWORD_INT && left != KEYWORD_DOUBLE && left != KEYWORD_BOOL) || (right != KEYWORD_INT && right != KEYWORD_DOUBLE && right != KEYWORD_BOOL)){ // left and right must be int/double/bool
                printf("03 || Semantic Check error [03.13] -> Expected INT/DOUBLE/BOOLEON around binop\n");
                exit(3);
            }            

            if(left != right){ // both sides do NOT have same type
                strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.14] -> Different data type found around binop\n");
                addWarning(tempWarningContainer);
            }

            return KEYWORD_BOOL; // return bool (comparision operators are analyzed)
            
        } else if(check == 3){ // found && ||
            if(left != KEYWORD_BOOL || right != KEYWORD_BOOL){ // exptecd bool on both sides
                printf("03 || Semantic Check error [03.15] -> Expected BOOLEON around binop\n");
                exit(3);
            }
            return KEYWORD_BOOL; // return bool
        } else{
            printf("03 || Semantic Check error [03.16] -> Unexpected binop code\n");
            exit(3);
        }
    } else if(top->type == AST_UNOP){ // Unary node found
        tokenType expr = getOutputTokenTypeOfAST(top->unop.expr); // get type of expr of unary node

        int check = getCodeOfUnOp(top->unop.op); // get code of unary operand

        if(check == 1){ // found !
            if(expr != KEYWORD_BOOL){ // expr is not bool
                printf("Expected BOOL type after !\n");
                printf("03 || Semantic Check error [03.17] -> Expected BOOLEON after !\n");
                exit(3);
            }

            return KEYWORD_BOOL; // return bool
        } else if(check == 2){ // found ++ --
            if(expr != KEYWORD_INT){ // expr is not int
                printf("03 || Semantic Check error [03.18] -> Expected INT around ++/--\n");
                exit(3);
            }

            return KEYWORD_INT; // return int
        } else{
            printf("03 || Semantic Check error [03.19] -> Unexpected unary code\n");
            exit(3);
        }
    } else if(top->type == AST_ASSIGN){ // Assign node found
        strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.20] -> Unexpected = found in expression\n");
        addWarning(tempWarningContainer);
        checkAST(top); // check this assign node seperately
        return getOutputTokenTypeOfAST(top->assign.var); // return the token type right of expr
    } 
    else{
        printf("03 || Semantic Check error [03.21] -> Unknows AST found\n");
        exit(3);
    }

    return MISC_UNKNOWN;

}

// evaluate update part of for
void evaluateUpdate(ASTNode* update){
    if(!update){
        printf("03 || Semantic Check error [03.26] -> AST is NOT accessible\n");
        exit(3);
    }

    if(update->type == AST_ASSIGN){ // found assign node
        checkAST(update); // check assign node seperately
        return;
    } else if(update->type == AST_UNOP){ // found unary operand
        if(update->unop.op == OP_NOT){ // found !
            printf("03 || Semantic Check error [03.27] -> ! NOT allowed in update section of FOR\n");
            exit(3);
        }

        // unary operands possble ++ --
        checkAST(update); // check unary node now
        return; // return
    } else{
        printf("03 || Semantic Check error [03.28] -> Expectedd either INCREMENTS/DECREMENTS/ASSIGNMENTS statements\n");
        exit(3);
    }
}

// evaluaate AST
void checkAST(ASTNode* top){ 

    if(!top){
        printf("03 || Semantic Check error [03.01] -> AST is NOT accessible\n");
        exit(3);
    }

    if(top->type == AST_DECL){ // found declaration AST
        bool init = true; // assume it also contains initialization

        if(!top->decl.init_expr) init = false; // ONLY declaration, no initialization
        
        // check if the right expr type is same as decl type 
        if(init){ // initialization exist
            // Special case: array initialization with {...} creates AST_BLOCK
            if(top->decl.is_array && top->decl.init_expr->type == AST_BLOCK){
                // Array initialization with constant values - validate each element
                for(int i = 0; i < top->decl.init_expr->block.statement_count; i++){
                    if(!checkIfTokenTypeMatch(top->decl.type, top->decl.init_expr->block.statements[i])){
                        printf("03 || Semantic Check error [03.02] -> Array initialization element type mismatch\n");
                        exit(3);
                    }
                }
            } else if(!checkIfTokenTypeMatch(top->decl.type , top->decl.init_expr)){ // expr didnt match             
                printf("03 || Semantic Check error [03.02] -> Declaration type error\n");
                exit(3);            
            }
        }

        // right expr type matched with decl type
        addSymbol(top->decl.var_name , top->decl.type , top->decl.is_array , top->decl.array_size , init , currScope , currBlockId); // try to add symbol to symbol table

        return;
    } else if(top->type == AST_ASSIGN){ // found assignment AST
        if(!checkIfTokenTypeMatch(getOutputTokenTypeOfAST(top->assign.var) , top->assign.expr)){
            printf("03 || Semantic Check error [03.03] -> Assignment type error\n");
            exit(3);
        }
    } else if(top->type == AST_IF){ // entering into a new IF block
        stackId[stackCount++] = ++currBlockId; // add blockId to stack and increment blockId and stack_counter

        currScope++; // increment the scope for condition of IF
        checkAST(top->if_stmt.condition); // evaluate if condition branch
        if(!checkIfTokenTypeMatch(KEYWORD_BOOL , top->if_stmt.condition)){ // BOOL not found in condition of IF            
            strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.04] -> Expected BOOL in condition of IF\n");
            addWarning(tempWarningContainer);
        }
        
        currScope++; // increment the scope for then_branch of IF                
        checkAST(top->if_stmt.then_branch); // evaluate if then branch

        currScope = currScope - 2; // reset the scope counter leaving the IF scope
        stackId[--stackCount] = -1; // remove the blockId from stack as we are leaving the IF block 
        
    } else if(top->type == AST_IF_ELSE){ // entering into a new IF-ELSE block
        stackId[stackCount++] = ++currBlockId; // add blockId to stack and increment blockId and stack_counter

        currScope++; // increment the scope for condition of IF-ELSE 
        checkAST(top->if_else_stmt.condition); // evaluate if condition branch  
        if(!checkIfTokenTypeMatch(KEYWORD_BOOL , top->if_else_stmt.condition)){ // BOOL not found in condition of IF-ELSE
            strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.05] -> Expected BOOL in condition of IF-ELSE\n");
            addWarning(tempWarningContainer);
        }  

        currScope++; // increment the scope for then_branch of IF-ELSE        
        checkAST(top->if_else_stmt.then_branch); // evaluate if_then branch
        
        stackId[--stackCount] = -1; // remove the then_branch blockId from the stack
        currScope--; // decrement the scope exiting then_branch of IF-ELSE [1] 

        stackId[stackCount++] = ++currBlockId; // add blockId to stack and increment blockId and stack_counter
        currScope++; // increment the scope for else_branch of IF-ELSE [2]   

        // [1] and [2] result in empty set, i wrote just clarity

        // evaluate else branch
        checkAST(top->if_else_stmt.else_branch); // evaluate else_then branch        

        currScope = currScope - 2; // reset the scope counter leaving the IF-ELSE scope
        stackId[--stackCount] = -1; // remove the blockId from stack as we are leaving the IF-ELSE block
        
    } else if(top->type == AST_WHILE){ // entering into a new WHILE block
        stackId[stackCount++] = ++currBlockId; // add blockId to stack and increment blockId and stack_counter
        currScope++; // increment the scope for condition of WHILE        
        checkAST(top->while_stmt.condition); // evaluate while condition
        if(!checkIfTokenTypeMatch(KEYWORD_BOOL , top->while_stmt.condition)){ // BOOL not found in condition of WHILE
            strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.06] -> Expected BOOL in condition of WHILE\n");
            addWarning(tempWarningContainer);
        }

        currScope++; // increment the scope for body of WHILE        
        checkAST(top->while_stmt.body); //evaluate while body

        currScope = currScope - 2; // reset the scope counter leaving the WHILE scope
        stackId[--stackCount] = -1; // remove the blockId from stack as we are leaving the WHILE block
        
    } else if(top->type == AST_FOR){ // entering into a new WHILE block
        stackId[stackCount++] = ++currBlockId; // add blockId to stack and increment blockId and stack_counter
        
        currScope++; // increment the scope for init, conditon and update of FOR
        checkAST(top->for_stmt.init); // evaluate FOR init
        checkAST(top->for_stmt.condition); // evaluate FOR condition
        if(!checkIfTokenTypeMatch(KEYWORD_BOOL , top->for_stmt.condition)){
            strcpy(tempWarningContainer , "03 || Semantic Check WARNING [W || 03.07] -> Expected BOOL in condition of FOR\n");
            addWarning(tempWarningContainer);
        }

        evaluateUpdate(top->for_stmt.update); // evaluate FOR update
        
        
        currScope++; // increment the scope for body of FOR
        checkAST(top->for_stmt.body); //evaluate for body

        currScope = currScope - 2; // reset the scope counter leaving the FOR scope
        stackId[--stackCount] = -1; // remove the blockId from stack as we are leaving the FOR block

    } else if(top->type == AST_UNOP){
        if(top->unop.op == OP_NOT){
            if(!checkIfTokenTypeMatch(KEYWORD_BOOL , top->unop.expr)){
                printf("03 || Semantic Check error [03.08] -> Expected BOOL after !\n");
                exit(3);
            }
        } else{
            if(!checkIfTokenTypeMatch(KEYWORD_INT , top->unop.expr)){
                printf("03 || Semantic Check error [03.09] -> Expected INT around ++/--\n");
                exit(3);
            }
        }
        
        
    }
    else if(top->type == AST_BLOCK){ // Found the ast block
        for(int i=0 ; i<top->block.statement_count ; i++){ // evaluate each ast in the block
            checkAST(top->block.statements[i]);
        }
    }

    return;
}

// Evaluate all AST
void doSemanticCheck(){ // Called
    stackId[0] = 0; // 0 block is always used
    for(int i=0 ; i<ast_count-1 ; i++){
        checkAST(all_ast[i]);
    }
    
    return;
}