/**
 * 04_icg.c - Intermediate Code Generator (Three-Address Code)
 * 
 * This module generates three-address code (TAC) from the validated AST.
 * TAC is a linearized, low-level representation that simplifies optimization
 * and target code generation.
 * 
 * Features:
 * - Expression translation to TAC
 * - Control flow translation (if, while, for)
 * - Array operations handling
 * - Temporary variable generation
 * - Label generation for jumps
 * - Structured TAC instruction stream
 * 
 * Author: Ridham Khurana
 */

#include "database.h"

int temp_var_used = 0;
int labels_used = 0;


// generate temp variables like t0 , t1 , t2 ...
char* generateNewAddrResult(){
    char* temp = (char*)malloc(16*sizeof(char)); //  allocated new memory;
    if(temp == NULL){
        printf("04 || ICG error [04.02] -> Memory allocation failed for temporary variable\n");
        exit(4);
    }

    sprintf(temp , "T%d", temp_var_used++);

    return temp;
}

// generate labels like L0 , L1 , L2 ...
char* generateNewAddrLabel(){
    char* temp = (char*)malloc(16*sizeof(char)); //  allocated new memory;
    if(temp == NULL){
        printf("04 || ICG error [04.03] -> Memory allocation failed for label\n");
        exit(4);
    }

    sprintf(temp , "L%d", labels_used++);

    return temp;
}

// extract result variable from 3-address code instruction
char* getVariableName(address* addr){
    if(addr->type == ADDR_ASSIGN){
        return addr->assign.result;
    } else if(addr->type == ADDR_BINOP){
        return addr->binop.result;
    } else if(addr->type == ADDR_UNOP){
        return addr->unop.result;
    } else if(addr->type == ADDR_ARRAY_READ){
        return addr->array_read.result;
    } else if(addr->type == ADDR_ARRAY_WRITE){
        return addr->array_write.array;
    }
    return NULL; // for other types, there is no result variable
}

// convert tokenType operator to string for 3-address code
char* opToString(tokenType op) {
    switch(op) {
        // Arithmetic operators
        case OP_PLUS:           return "+";
        case OP_MINUS:          return "-";
        case OP_MUL:            return "*";
        case OP_DIV:            return "/";
        
        // Assignment operator
        case OP_EQUAL:          return "=";
        
        // Relational operators
        case OP_IS_EQUAL:       return "==";
        case OP_NOT_EQUAL:      return "!=";
        case OP_LESS_THAN:      return "<";
        case OP_GREAT_THAN:     return ">";
        case OP_LESS_EQUAL:     return "<=";
        case OP_GREAT_EQUAL:    return ">=";
        
        // Logical operators
        case OP_AND_AND:        return "&&";
        case OP_OR_OR:          return "||";
        
        // Unary operators
        case OP_NOT:            return "!";
        case OP_PLUS_PLUS:      return "+";  // returning + instead of ++ for unop
        case OP_MINUS_MINUS:    return "-";  // returning - instead of -- for unop
        
        default:                return "?";
    }
}

// ----- helper functions to generate 3-address code objects -----

// generate assignment: result = arg1
address* genAssign(const char* result, const char* arg1) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_ASSIGN;
    strcpy(temp->assign.result, result);
    strcpy(temp->assign.arg1, arg1);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate binary operation: result = arg1 op arg2
address* genBinOp(const char* result, const char* arg1, const char* op, const char* arg2) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_BINOP;
    strcpy(temp->binop.result, result);
    strcpy(temp->binop.arg1, arg1);
    strcpy(temp->binop.op, op);
    strcpy(temp->binop.arg2, arg2);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate unary operation: result = op arg1
address* genUnOp(const char* result, const char* op, const char* arg1) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_UNOP;
    strcpy(temp->unop.result, result);
    strcpy(temp->unop.op, op);
    strcpy(temp->unop.arg1, arg1);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate unconditional jump: goto label
address* genGoto(const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_GOTO;
    strcpy(temp->goto_stmt.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate conditional jump if false: ifFalse condition goto label
address* genIfFalseGoto(const char* condition, const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_IF_F_GOTO;
    strcpy(temp->if_false.condition, condition);
    strcpy(temp->if_false.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate conditional jump if true: ifTrue condition goto label
address* genIfTrueGoto(const char* condition, const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_IF_T_GOTO;
    strcpy(temp->if_true.condition, condition);
    strcpy(temp->if_true.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate label: L1:
address* genLabel(const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_LABEL;
    strcpy(temp->label.labelNumber, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate array read: result = array[index]
address* genArrayRead(const char* result, const char* array, const char* index) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_ARRAY_READ;
    strcpy(temp->array_read.result, result);
    strcpy(temp->array_read.array, array);
    strcpy(temp->array_read.index, index);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// generate array write: array[index] = value
address* genArrayWrite(const char* array, const char* index, const char* value) {
    address* temp = (address*)malloc(sizeof(address));
    if(temp == NULL){
        printf("04 || ICG error [04.04] -> Memory allocation failed for three-address code instruction\n");
        exit(4);
    }
    if(addr_count >= MAX){
        printf("04 || ICG error [04.05] -> Maximum limit of three-address code instructions reached\n");
        exit(4);
    }
    temp->type = ADDR_ARRAY_WRITE;
    strcpy(temp->array_write.array, array);
    strcpy(temp->array_write.index, index);
    strcpy(temp->array_write.value, value);
    
    allAddress[addr_count++] = temp;
    return temp;
}

address* genAddr(ASTNode* top);

// ----- handler functions for different AST node types -----

// handle variable declaration and array initialization
address* handleDeclNodes(ASTNode* top){
    
    if(top->decl.is_array){ // array initialization
        // Check if initialization exists and is an AST_BLOCK (array initializer list)
        if(top->decl.init_expr && top->decl.init_expr->type == AST_BLOCK){
            // Array initialization with {...} values
            // Generate code to assign each element: arr[i] = value
            for(int i = 0; i < top->decl.init_expr->block.statement_count; i++){
                address* elem = genAddr(top->decl.init_expr->block.statements[i]);
                char index_str[MAX_NAME];
                sprintf(index_str, "%d", i);
                genArrayWrite(top->decl.var_name, index_str, getVariableName(elem));
            }
            return NULL; // array initialization doesn't return a value
        } else if(top->decl.init_expr){
            // Single value initialization (shouldn't happen for arrays, but handle it)
            address* expr = genAddr(top->decl.init_expr);
            char value[MAX_NAME];
            sprintf(value , "%s" , top->decl.array_size);
            address* index = genAssign(generateNewAddrResult() , value);
            return genArrayWrite(top->decl.var_name , getVariableName(index) , getVariableName(expr));
        }
        return NULL; // array declaration without initialization
    } else{ // simple variable assignment
        if(top->decl.init_expr){
            address* expr = genAddr(top->decl.init_expr);
            return genAssign(top->decl.var_name , getVariableName(expr));
        }
        return NULL; // declaration without initialization
    }
}

// handle assignment statements for variables and arrays
address* handleAssignNodes(ASTNode* top){    
    address* expr = genAddr(top->assign.expr);

    if(top->assign.var->type == AST_VAR){ // simple variable assignment: x = expr
        return genAssign(top->assign.var->var.var_name , getVariableName(expr));
    } else if(top->assign.var->type == AST_ARRAY_ACCESS){ // array element assignment: arr[i] = expr
        address* index = genAddr(top->assign.var->array_access.sizeExpr);
        return genArrayWrite(top->assign.var->array_access.array_name , getVariableName(index) , getVariableName(expr));
    }
    
    return NULL;
}

// handle binary operations like +, -, *, /, <, >, ==, etc.
address* handleBinOpNodes(ASTNode* top){
    address* left = genAddr(top->binop.left);
    address* right = genAddr(top->binop.right);
    
    return genBinOp(generateNewAddrResult(), getVariableName(left), opToString(top->binop.op), getVariableName(right)); // t2 = t1 op t0
}

// handle unary operations like !, ++, --
address* handleUnOpNodes(ASTNode* top){
    address* var = genAddr(top->unop.expr);

    if(top->unop.op == OP_NOT){ // !var - logical not
        return genUnOp(generateNewAddrResult() , opToString(top->unop.op) , getVariableName(var));
    } else { // ++ or -- operations
        if(top->unop.isPrefix){ // ++var or --var (prefix)
            address* help = genAssign(generateNewAddrResult() , getVariableName(var));
            help = genBinOp(generateNewAddrResult() , getVariableName(help) , opToString(top->unop.op) , "1");
            if(top->unop.expr->type == AST_ARRAY_ACCESS){ // array element
                address* index = genAddr(top->unop.expr->array_access.sizeExpr);
                genArrayWrite(top->unop.expr->array_access.array_name , getVariableName(index) , getVariableName(help));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            } else{ // simple variable
                genAssign(top->unop.expr->var.var_name , getVariableName(help));
                return genAssign(generateNewAddrResult() , top->unop.expr->var.var_name);
            }
        } else{ // var++ or var-- (postfix)
            address* help = genAssign(generateNewAddrResult() , getVariableName(var));
            address* spec = genBinOp(generateNewAddrResult() , getVariableName(help) , opToString(top->unop.op) , "1");
            if(top->unop.expr->type == AST_ARRAY_ACCESS){ // array element
                address* index = genAddr(top->unop.expr->array_access.sizeExpr);
                genArrayWrite(top->unop.expr->array_access.array_name , getVariableName(index) , getVariableName(spec));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            } else{ // simple variable
                genAssign(top->unop.expr->var.var_name , getVariableName(spec));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            }
        }
    }
}

// handle IF statement: if(condition) then_branch
address* handleIfNode(ASTNode* top){
    address* cond = genAddr(top->if_stmt.condition);

    char* temp_label = generateNewAddrLabel();
    
    genIfFalseGoto(getVariableName(cond) , temp_label); // jump to label if condition is false

    // generate code for then branch
    for(int i=0 ; i<top->if_stmt.then_branch->block.statement_count ; i++){
        genAddr(top->if_stmt.then_branch->block.statements[i]);
    }

    genLabel(temp_label); // label after if block

    return NULL;
}

// handle IF-ELSE statement: if(condition) then_branch else else_branch
address* handleIfElseNode(ASTNode* top){
    address* cond = genAddr(top->if_else_stmt.condition);

    char* temp_label = generateNewAddrLabel();
    
    genIfFalseGoto(getVariableName(cond) , temp_label); // jump to else block if condition is false

    // generate code for then branch
    for(int i=0 ; i<top->if_else_stmt.then_branch->block.statement_count ; i++){
        genAddr(top->if_else_stmt.then_branch->block.statements[i]);
    }

    char* temp_label_2 = generateNewAddrLabel();

    genGoto(temp_label_2); // skip else block after then block

    genLabel(temp_label); // start of else block

    // generate code for else branch
    for(int i=0 ; i<top->if_else_stmt.else_branch->block.statement_count ; i++){
        genAddr(top->if_else_stmt.else_branch->block.statements[i]);
    }

    genLabel(temp_label_2); // label after if-else block
    
    return NULL;
}

// handle WHILE loop: while(condition) body
address* handleWhileNode(ASTNode* top){

    char* temp_label_2 = generateNewAddrLabel();

    genLabel(temp_label_2); // start of while loop

    address* cond = genAddr(top->if_stmt.condition);

    char* temp_label = generateNewAddrLabel();

    genIfFalseGoto(getVariableName(cond) , temp_label); // exit loop if condition is false

    // generate code for loop body
    for(int i=0 ; i<top->while_stmt.body->block.statement_count ; i++){
        genAddr(top->while_stmt.body->block.statements[i]);
    }

    genGoto(temp_label_2); // jump back to condition check
    genLabel(temp_label); // label after while loop
    
    return NULL;
}

// handle FOR loop: for(init; condition; update) body
address* handleForNode(ASTNode* top){
    genAddr(top->for_stmt.init); // initialization

    char* temp_label = generateNewAddrLabel();

    genLabel(temp_label); // start of for loop

    address* cond = genAddr(top->for_stmt.condition);

    char* temp_label_2 = generateNewAddrLabel();

    genIfFalseGoto(getVariableName(cond) , temp_label_2); // exit loop if condition is false

    // generate code for loop body
    for(int i=0 ; i<top->for_stmt.body->block.statement_count ; i++){
        genAddr(top->for_stmt.body->block.statements[i]);
    }

    genAddr(top->for_stmt.update); // update expression

    genGoto(temp_label); // jump back to condition check

    genLabel(temp_label_2); // label after for loop
    
    return NULL;
}

// main function to generate 3-address code from AST nodes
address* genAddr(ASTNode* top){

    if(top == NULL){
        printf("04 || ICG error [04.01] -> AST is NOT accessible or NULL\n");
        exit(4);
    }

    char value[MAX_NAME];

    // handle literal values
    if(top->type == AST_NUM){ // integer literal
        sprintf(value , "%d" , top->int_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_DOUBLE){ // floating point literal
        sprintf(value , "%f" , top->double_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_BOOL){ // boolean literal
        if(top->bool_value){
            strcpy(value , "true");
        } else{
            strcpy(value , "false");
        }
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_CHAR){ // character literal
        sprintf(value , "%c" , top->char_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_VAR){ // variable access
        return genAssign(generateNewAddrResult() , top->var.var_name);
    } else if(top->type == AST_ARRAY_ACCESS){ // array element access
        address* index = genAddr(top->array_access.sizeExpr);
        return genArrayRead( generateNewAddrResult() , top->array_access.array_name , getVariableName(index));
    } else if(top->type == AST_DECL){ // variable declaration
        if(!top->decl.init_expr) return NULL;

        return handleDeclNodes(top);
        
    } else if(top->type == AST_ASSIGN){ // assignment statement
        return handleAssignNodes(top);        
    } else if(top->type == AST_BINOP){ // binary operation
        return handleBinOpNodes(top);   
    } else if(top->type == AST_UNOP){ // unary operation
        return handleUnOpNodes(top);        
    } else if(top->type == AST_IF){ // if statement
        return handleIfNode(top);
    } else if(top->type == AST_IF_ELSE){ // if-else statement
        return handleIfElseNode(top);
    } else if(top->type == AST_FOR){ // for loop
        return handleForNode(top);
    } else if(top->type == AST_WHILE){ // while loop
        return handleWhileNode(top);
    }
    
    return NULL;
}

// start intermediate code generation for all AST nodes
void startICG(){ // called from main
    for(int i=0 ; i<ast_count-1 ; i++){
        genAddr(all_ast[i]);
    }

    return;
}