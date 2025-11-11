#include "database.h"

int temp_var_used = 0;
int labels_used = 0;


// generate temp variables like t0 , t1 , t2 ...
char* generateNewAddrResult(){
    char* temp = (char*)malloc(16*sizeof(char)); //  allocated new memory;

    sprintf(temp , "T%d", temp_var_used++);

    return temp;
}

// generate labels like L0 , L1 , L2 ...
char* generateNewAddrLabel(){
    char* temp = (char*)malloc(16*sizeof(char)); //  allocated new memory;

    sprintf(temp , "L%d", labels_used++);

    return temp;
}

// get result temp variable whenever needed
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
    return NULL; // For other types, there is no result variable
}

// Convert tokenType operator to string
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

// helper functions to generate 3-address code objects

// Generate: result = arg1
address* genAssign(const char* result, const char* arg1) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_ASSIGN;
    strcpy(temp->assign.result, result);
    strcpy(temp->assign.arg1, arg1);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: result = arg1 op arg2
address* genBinOp(const char* result, const char* arg1, const char* op, const char* arg2) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_BINOP;
    strcpy(temp->binop.result, result);
    strcpy(temp->binop.arg1, arg1);
    strcpy(temp->binop.op, op);
    strcpy(temp->binop.arg2, arg2);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: result = op arg1
address* genUnOp(const char* result, const char* op, const char* arg1) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_UNOP;
    strcpy(temp->unop.result, result);
    strcpy(temp->unop.op, op);
    strcpy(temp->unop.arg1, arg1);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: goto label
address* genGoto(const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_GOTO;
    strcpy(temp->goto_stmt.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: ifFalse condition goto label
address* genIfFalseGoto(const char* condition, const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_IF_F_GOTO;
    strcpy(temp->if_false.condition, condition);
    strcpy(temp->if_false.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: ifTrue condition goto label
address* genIfTrueGoto(const char* condition, const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_IF_T_GOTO;
    strcpy(temp->if_true.condition, condition);
    strcpy(temp->if_true.target, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: L1:
address* genLabel(const char* label) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_LABEL;
    strcpy(temp->label.labelNumber, label);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: result = array[index]
address* genArrayRead(const char* result, const char* array, const char* index) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_ARRAY_READ;
    strcpy(temp->array_read.result, result);
    strcpy(temp->array_read.array, array);
    strcpy(temp->array_read.index, index);
    
    allAddress[addr_count++] = temp;
    return temp;
}

// Generate: array[index] = value
address* genArrayWrite(const char* array, const char* index, const char* value) {
    address* temp = (address*)malloc(sizeof(address));
    temp->type = ADDR_ARRAY_WRITE;
    strcpy(temp->array_write.array, array);
    strcpy(temp->array_write.index, index);
    strcpy(temp->array_write.value, value);
    
    allAddress[addr_count++] = temp;
    return temp;
}

address* genAddr(ASTNode* top);

// generate 3-address code of decl node
address* handleDeclNodes(ASTNode* top){
    address* expr = genAddr(top->decl.init_expr);
    
    if(top->decl.is_array){
        char value[MAX_NAME];
        sprintf(value , "%c" , top->decl.array_size);
        address* index = genAssign(generateNewAddrResult() , value);
        return genArrayWrite(top->decl.var_name , getVariableName(index) , getVariableName(expr));
    } else{
        return genAssign(top->decl.var_name , getVariableName(expr));
    }
}

// generate 3-address code of assign node
address* handleAssignNodes(ASTNode* top){    
    address* expr = genAddr(top->assign.expr);

    if(top->assign.var->type == AST_VAR){ // single variable assignment
        return genAssign(top->assign.var->var.var_name , getVariableName(expr));
    } else if(top->assign.var->type == AST_ARRAY_ACCESS){
        address* index = genAddr(top->assign.var->array_access.sizeExpr);
        return genArrayWrite(top->assign.var->array_access.array_name , getVariableName(index) , getVariableName(expr));
    }

}

// generate 3-address code of binary operation node
address* handleBinOpNodes(ASTNode* top){
    address* left = genAddr(top->binop.left);
    address* right = genAddr(top->binop.right);
    
    return genBinOp(generateNewAddrResult(), getVariableName(left), opToString(top->binop.op), getVariableName(right)); // generate t2 = t1 + t0
}

// generate 3-address code of unary operation node
address* handleUnOpNodes(ASTNode* top){
    address* var = genAddr(top->unop.expr);

    // return var;
    if(top->unop.op == OP_NOT){ // !var
        return genUnOp(generateNewAddrResult() , opToString(top->unop.op) , getVariableName(var));
    } else { // ++ or --
        if(top->unop.isPrefix){ // ++var or --var
            address* help = genAssign(generateNewAddrResult() , getVariableName(var));
            help = genBinOp(generateNewAddrResult() , getVariableName(help) , opToString(top->unop.op) , "1");
            if(top->unop.expr->type == AST_ARRAY_ACCESS){ // array
                address* index = genAddr(top->unop.expr->array_access.sizeExpr); // index
                address* helper = genArrayWrite(top->unop.expr->array_access.array_name , getVariableName(index) , getVariableName(help));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            } else{ // variable
                genAssign(top->unop.expr->var.var_name , getVariableName(help));
                return genAssign(generateNewAddrResult() , top->unop.expr->var.var_name);
            }
        } else{ // var++ or var--
            address* help = genAssign(generateNewAddrResult() , getVariableName(var));
            address* spec = genBinOp(generateNewAddrResult() , getVariableName(help) , opToString(top->unop.op) , "1");
            if(top->unop.expr->type == AST_ARRAY_ACCESS){
                address* index = genAddr(top->unop.expr->array_access.sizeExpr); // index
                address* helper = genArrayWrite(top->unop.expr->array_access.array_name , getVariableName(index) , getVariableName(spec));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            } else{
                genAssign(top->unop.expr->var.var_name , getVariableName(spec));
                return genAssign(generateNewAddrResult() , getVariableName(help));
            }
        }
    }
}

// generate 3-address code of IF node
address* handleIfNode(ASTNode* top){
    address* cond = genAddr(top->if_stmt.condition);

    char* temp_label = generateNewAddrLabel();
    
    genIfFalseGoto(getVariableName(cond) , temp_label);

    for(int i=0 ; i<top->if_stmt.then_branch->block.statement_count ; i++){
        genAddr(top->if_stmt.then_branch->block.statements[i]);
    }

    genLabel(temp_label);

    return NULL;
}

// generate 3-address code fo IF-ELSE node
address* handleIfElseNode(ASTNode* top){
    address* cond = genAddr(top->if_else_stmt.condition);

    char* temp_label = generateNewAddrLabel();
    
    genIfFalseGoto(getVariableName(cond) , temp_label);

    for(int i=0 ; i<top->if_else_stmt.then_branch->block.statement_count ; i++){
        genAddr(top->if_else_stmt.then_branch->block.statements[i]);
    }

    char* temp_label_2 = generateNewAddrLabel();

    genGoto(temp_label_2);

    genLabel(temp_label);

    for(int i=0 ; i<top->if_else_stmt.else_branch->block.statement_count ; i++){
        genAddr(top->if_else_stmt.else_branch->block.statements[i]);
    }

    genLabel(temp_label_2);
}

// generate 3-address code fo WHILE node
address* handleWhileNode(ASTNode* top){

    char* temp_label_2 = generateNewAddrLabel();

    genLabel(temp_label_2);

    address* cond = genAddr(top->if_stmt.condition);

    char* temp_label = generateNewAddrLabel();

    genIfFalseGoto(getVariableName(cond) , temp_label);

    for(int i=0 ; i<top->while_stmt.body->block.statement_count ; i++){
        genAddr(top->while_stmt.body->block.statements[i]);
    }

    genGoto(temp_label_2);
    genLabel(temp_label);
}

// generate 3-address code fo FOR node
address* handleForNode(ASTNode* top){
    address* init = genAddr(top->for_stmt.init);

    char* temp_label = generateNewAddrLabel();

    genLabel(temp_label);

    address* cond = genAddr(top->for_stmt.condition);

    char* temp_label_2 = generateNewAddrLabel();

    genIfFalseGoto(getVariableName(cond) , temp_label_2);

    for(int i=0 ; i<top->for_stmt.body->block.statement_count ; i++){
        genAddr(top->for_stmt.body->block.statements[i]);
    }

    address* update = genAddr(top->for_stmt.update);

    genGoto(temp_label);

    genLabel(temp_label_2);
}

address* genAddr(ASTNode* top){

    char value[MAX_NAME];

    if(top->type == AST_NUM){
        sprintf(value , "%d" , top->int_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_DOUBLE){
        sprintf(value , "%f" , top->double_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_BOOL){
        // sprintf(value , "%d" , top->bool_value ? : "true" , "false");
        if(top->bool_value){
            strcpy(value , "true");
        } else{
            strcpy(value , "false");
        }
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_CHAR){    
        sprintf(value , "%c" , top->char_value);
        return genAssign(generateNewAddrResult() , value);
    } else if(top->type == AST_VAR){
        return genAssign(generateNewAddrResult() , top->var.var_name);
    } else if(top->type == AST_ARRAY_ACCESS){ // array read
        address* index = genAddr(top->array_access.sizeExpr);
        // address* result = genAssign(top->array_access.array_name , getVariableName(index));

        // return result;
        return genArrayRead( generateNewAddrResult() , top->array_access.array_name , getVariableName(index));
    } else if(top->type == AST_DECL){
        if(!top->decl.init_expr) return NULL;

        return handleDeclNodes(top);
        
    } else if(top->type == AST_ASSIGN){
        return handleAssignNodes(top);        
    } else if(top->type == AST_BINOP){
        return handleBinOpNodes(top);   
    } else if(top->type == AST_UNOP){
        return handleUnOpNodes(top);        
    } else if(top->type == AST_IF){
        return handleIfNode(top);
    } else if(top->type == AST_IF_ELSE){
        return handleIfElseNode(top);
    } else if(top->type == AST_FOR){
        return handleForNode(top);
    } else if(top->type == AST_WHILE){
        return handleWhileNode(top);
    }
    
    return NULL;
}

void startICG(){ // Called from main
    for(int i=0 ; i<ast_count-1 ; i++){
        genAddr(all_ast[i]);
    }

    return;
}