#include "database.h"

// precedence lookup
typedef struct
{
    int preNum;
    tokenType type;
} PrecedenceLookup;

PrecedenceLookup preLookup[] = {
    // empty for ++ -- [precedence = 8(highest)]

    {10, L_PARAN}, // (
    {10, R_PARAN}, // )
    {8, OP_NOT},   // !
    {8 , OP_PLUS_PLUS}, // ++
    {8 , OP_MINUS_MINUS}, // --
    {7, OP_MUL}, // *
    {7, OP_DIV}, // /
    {6, OP_PLUS}, // +
    {6, OP_MINUS}, // -
    {5, OP_LESS_THAN}, // <
    {5, OP_GREAT_THAN}, // >
    {5, OP_LESS_EQUAL}, // <=
    {5, OP_GREAT_EQUAL}, // >=
    {4, OP_IS_EQUAL}, // ==
    {4, OP_NOT_EQUAL}, // !=
    {3, OP_AND_AND}, // &&
    {2, OP_OR_OR}, // ||
    {1, OP_EQUAL}, // =
    {0, SEMI} // ;
};

ASTNode *parseStatement();

int getPrecedenceValueOf(tokenType type)
{
    int size = sizeof(preLookup) / sizeof(preLookup[0]);
    for (int i = 0; i<size ; i++)
    {
        if (preLookup[i].type == type)
            return preLookup[i].preNum;
    }
    return -1;
}

ASTNode *generateIntASTNode(int int_value)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_NUM;
    temp->int_value = int_value;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateDoubleASTNode(double double_value)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_DOUBLE;
    temp->double_value = double_value;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateBoolASTNode(bool bool_value)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_BOOL;
    temp->bool_value = bool_value;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateCharASTNode(char char_value)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_CHAR;
    temp->char_value = char_value;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode* generateStringASTNode(char* str_value){
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_STRING;
    strcpy(temp->string_value , str_value);

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    return temp;
}

ASTNode *generateVarASTNode(char *var_name)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_VAR;
    strcpy(temp->var.var_name, var_name);

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateDeclASTNode(char *var_name , tokenType type , ASTNode *init_expr , bool isArray , char* arrSize)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_DECL;
    strcpy(temp->decl.var_name , var_name);
    temp->decl.type = type;
    temp->decl.init_expr = init_expr;
    temp->decl.is_array = isArray;
    if(isArray) strcpy(temp->decl.array_size , arrSize);

    

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateAssignASTNode(ASTNode* var, ASTNode *expr)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_ASSIGN;
    temp->assign.var = var;
    temp->assign.expr = expr;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateArrayASTNode(char *array_name, ASTNode *index)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_ARRAY_ACCESS;
    strcpy(temp->array_access.array_name, array_name);
    temp->array_access.sizeExpr = index;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateUnaryASTNode(tokenType type, ASTNode *expr)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_UNOP;
    temp->unop.op = type;
    temp->unop.expr = expr;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateBinaryASTNode(tokenType type, ASTNode *left, ASTNode *right)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_BINOP;
    temp->unop.op = type;
    temp->binop.left = left;
    temp->binop.right = right;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateIfASTNode(ASTNode *condition, ASTNode *then_expr)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_IF;
    temp->if_stmt.condition = condition;
    temp->if_stmt.then_branch = then_expr;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateIfElseASTNode(ASTNode *condition, ASTNode *then_expr, ASTNode *else_expr)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_IF_ELSE;
    temp->if_else_stmt.condition = condition;
    temp->if_else_stmt.then_branch = then_expr;
    temp->if_else_stmt.else_branch = else_expr;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateWhileASTNode(ASTNode *condition, ASTNode *body)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_WHILE;
    temp->while_stmt.condition = condition;
    temp->while_stmt.body = body;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateForASTNode(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_FOR;
    temp->for_stmt.init = init;
    temp->for_stmt.condition = condition;
    temp->for_stmt.update = update;
    temp->for_stmt.body = body;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

ASTNode *generateBlockASTNode(ASTNode **statements, int statement_count)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));
    temp->type = AST_BLOCK;

    // ✅ Allocate new memory for the statement list
    temp->block.statements = malloc(statement_count * sizeof(ASTNode*));
    for (int i = 0; i < statement_count; i++)
        temp->block.statements[i] = statements[i];

    temp->block.statement_count = statement_count;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    return temp;
}


ASTNode *generateProgramASTNode(ASTNode **statements, int statement_count)
{
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));

    temp->type = AST_PROGRAM;
    temp->program.statements = statements;
    temp->program.statement_count = statement_count;

    if (ast_count >= MAX)
    {
        printf("NOT enough storage..\n");
        exit(4);
    }

    // all_ast[ast_count++] = temp;

    return temp;
}

char getCharFromQuotes(char* sQuotes){
    if(sQuotes[0] != '\'' || sQuotes[2] != '\''){
        printf("Token is NOT validd...\n");
        exit(2);
    }

    return sQuotes[1];
}

char* getStringFromQuotes(const char* dQuotes) {
    if (dQuotes[0] != '\"') {
        printf("Error: Token is NOT valid...\n");
        exit(2);
    }

    int len = strlen(dQuotes);

    if (dQuotes[len - 1] != '\"') {
        printf("Error: Missing closing quote\n");
        exit(2);
    }

    // Allocate memory for the string content
    char* result = (char*)malloc(len - 1);  // len-2 chars + 1 for '\0'
    if (!result) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(result, dQuotes + 1, len - 2); // copy without quotes
    result[len - 2] = '\0';                // null-terminate

    return result;
}

bool isLvalue(ASTNode* top){
    if (!top) return false;

    switch (top->type) {
        case AST_VAR:
            // variable name like "a"
            return true;

        case AST_ARRAY_ACCESS:
            // array element like "arr[i]"
            return true;

        default:
            // anything else — literals, expressions, etc.
            return false;
    }
}

bool isBinaryOrUnaryOp(tokenType type) {
    switch (type) {
        // Binary operators
        case OP_PLUS:
        case OP_MINUS:
        case OP_MUL:
        case OP_DIV:
        case OP_IS_EQUAL:
        case OP_NOT_EQUAL:
        case OP_LESS_THAN:
        case OP_GREAT_THAN:
        case OP_LESS_EQUAL:
        case OP_GREAT_EQUAL:
        case OP_AND_AND:
        case OP_OR_OR:
        case OP_EQUAL:
            return true;

        // Unary operators
        case OP_NOT:
        case OP_PLUS_PLUS:
        case OP_MINUS_MINUS:
            return true;

        default:
            return false;
    }
}

bool isUnaryOp(tokenType type) {
    switch (type) {        

        // Unary operators
        case OP_NOT:
        case OP_PLUS_PLUS:
        case OP_MINUS_MINUS:
            return true;

        default:
            return false;
    }
}


ASTNode *parseExpression(int minPrecedence, bool findClosingParan , bool findClosingBrack , bool findClosingBraces , bool findComma)
{
    // used to evaluate right side of =    

    ASTNode *left = NULL; // the variable or literal tokens
    Token *tok = tokens[ast_current_index]; // save current token


    if (tok->token_type == VAL_INT) // int value
    {
        left = generateIntASTNode(tok->int_value);
        ast_current_index++;
    }
    else if (tok->token_type == VAL_DOUBLE) // double value
    {
        left = generateDoubleASTNode(tok->double_value);
        ast_current_index++;
    }
    else if (tok->token_type == VAL_BOOL) // booleon value
    {
        left = generateBoolASTNode(tok->bool_value);
        ast_current_index++;
    }
    else if (tok->token_type == VAL_CHAR) // character value
    {
        left = generateCharASTNode(tok->var_name[1]);
        ast_current_index++;
    }
    else if (tok->token_type == ID) // variable
    {
        if(tokens[ast_current_index+1]->token_type == L_BRACK){ // array || found [
            if(tokens[ast_current_index+2]->token_type != VAL_INT && tokens[ast_current_index+2]->token_type != ID){ // index of array MUST be either int or variable name
                printf("02 || Syntax error [02.13] -> index of array must be either int or variable\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            ast_current_index++; // variable token -> [ token  
            ast_current_index++; // [ token -> expression starting token
            ASTNode* arraySize = parseExpression(0,false,true,false,false); // parse the expression inside the bracket
            if(tokens[ast_current_index]->token_type != R_BRACK){ // closing ] missing
                printf("02 || Syntax error [02.14] -> Expected ]\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            ast_current_index++; // skip ]
            
            left = generateArrayASTNode(tok->var_name , arraySize); // generate array node
        }
        else{ // not an array, analize ID token
            left = generateVarASTNode(tok->var_name); // generate variable node
            ast_current_index++;
        }

        if(tokens[ast_current_index]->token_type == OP_PLUS_PLUS || tokens[ast_current_index]->token_type == OP_MINUS_MINUS){ // found post-fix unary operator
            // update left to be a unary node instead of var node
            left = generateUnaryASTNode(tokens[ast_current_index++]->token_type , left); //generate unary operator and advance the pointor
        }
        
    }
    else if (tok->token_type == L_PARAN) // found (
    {
        ast_current_index++; // ( token -> expression token
        left = parseExpression(0, true,false,false,false); // evaluate expression inside the braket
        if(tokens[ast_current_index]->token_type != R_PARAN){ // closing ] missing
            printf("02 || Syntax error [02.15] -> Expected )\n");
            printf("Exiting...\n\n");
            exit(2);
        }
        ast_current_index++; // skip )        
            
        if(tokens[ast_current_index]->token_type == OP_PLUS_PLUS || tokens[ast_current_index]->token_type == OP_MINUS_MINUS){ // found post-fix unary operator
            // update left to be a unary node
            left = generateUnaryASTNode(tokens[ast_current_index++]->token_type , left); //generate unary operator and advance the pointor
        }
    }
    else if(tok->token_type == OP_PLUS_PLUS || tok->token_type == OP_MINUS_MINUS || tok->token_type == OP_NOT){ // found pre-fix unary operator
        ast_current_index++; // unary token -> expression token        
        left = parseExpression(9,findClosingParan,findClosingBrack,findClosingBraces,findComma); //  parse the expression on which unary is applied
        if(tok->token_type == OP_NOT){
            left = generateUnaryASTNode(tok->token_type , left); // generate unary node
        }
        else if(!isLvalue(left)){ // unary operators only allowed before/after lvalues
            printf("02 || Syntax error [02.16] -> Expected lvalue after unary operator\n");
            printf("Exiting...\n\n");
            exit(2);
        }
        left = generateUnaryASTNode(tok->token_type , left); // generate unary node
    }
    else if(tok->token_type == VAL_CHAR){ // 'c'
        left = generateCharASTNode(getCharFromQuotes(tok->var_name));
        ast_current_index++;
    }
    else if(tok->token_type == VAL_STRING){ // "string"
        left = generateStringASTNode(getStringFromQuotes(tok->var_name));
        ast_current_index++;
    }
    else if(tok->token_type == R_BRACES){
        ast_current_index++;
        return NULL;
    }
    else
    { 
        printf("02 || Syntax error [02.17] -> Unexpected token found in expression\n");
        printf("Exiting...\n\n");
        exit(2);
    }

    while (tokens[ast_current_index] && ast_current_index < token_count - 1 && tokens[ast_current_index-1]->token_type != SEMI)
    {
        Token *nextTok = tokens[ast_current_index]; // store current token

        if (nextTok->token_type == SEMI) // found semi colon
        {
            ast_current_index++; // semi token -> next token
            if (findClosingParan) // expected ) but foundd ;
            {
                printf("02 || Syntax error [02.18] -> Expected )\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            if (findClosingBrack) // expected ] but foundd ;
            {
                printf("02 || Syntax error [02.19] -> Expected ]\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            return left;
        }

        if (nextTok->token_type == R_PARAN){
            if(findClosingParan){
                return left;
            }else{
                printf("02 || Syntax error [02.20] -> Unexpected ) found in expression\n");
                printf("Exiting...\n\n");
                exit(2);
            }
        }
        
        if(nextTok->token_type == R_BRACK){
            if(findClosingBrack){
                return left;
            } else{
                printf("02 || Syntax error [02.21] -> Unexpected ] found in expression\n");
                printf("Exiting...\n\n");
                exit(2);
            }
        }

        if(nextTok->token_type == R_BRACES){            
            if(findClosingBraces){
                return left;
            } else{
                printf("02 || Syntax error [02.22] -> Unexpected } found in expression\n");
                printf("Exiting...\n\n");
                exit(2);
            }
        }

        if(nextTok->token_type == COMMA){
            if(findComma){
                return left;
            } else{
                printf("02 || Syntax error [02.23] -> Unexpected , found in expression\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            
        }

        if(!isBinaryOrUnaryOp(nextTok->token_type)){ // check if next token is operator or not [binary/unary both allowed]
            printf("02 || Syntax error [02.24] -> Expected unary or binary operator in expression\n");
            printf("Exiting...\n\n");
            exit(2);
        }

        int nextPrec = getPrecedenceValueOf(nextTok->token_type); // get precedence value of current operator
        // if(nextPrec == -1){
        //     printf("Error\n");
        //     exit(2);
        // }
        if (nextPrec < minPrecedence){ // precedence is lower
            return left;
        }

        tokenType tempOp = nextTok->token_type; // store the operator
        ast_current_index++; // operator token -> next token

        ASTNode *right = parseExpression(nextPrec + 1, findClosingParan,findClosingBrack,findClosingBraces,findComma); // parse right side of the operator

        if(tempOp == OP_EQUAL){
            left = generateAssignASTNode(left,right);            
        } else{
            left = generateBinaryASTNode(tempOp, left, right); // generate operator node
        }
        
        
    }

    return left;
}

ASTNode* pareseArrayInit(int length){
    
    ASTNode* list[MAX];
    int count = 0;

    ASTNode* temp = NULL;

    for(int i=0 ; i<length ; i++){ // loop throught length number of elements
        
        if(i == length-1){ // for last element
            temp = parseExpression(0,false,false,true,false); 
            list[count++] = temp;            
            break;
        }
        temp = parseExpression(0,false,false,false,true);
        if(tokens[ast_current_index]->token_type == COMMA){            
            ast_current_index++;
        }
        if(tokens[ast_current_index]->token_type == R_BRACES){            
            ast_current_index++;
        }
        list[count++] = temp;
    }
    
    if(tokens[ast_current_index]->token_type != R_BRACES){ 
        printf("02 || Syntax error [02.25] -> Expected }\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    ast_current_index++;

    if(tokens[ast_current_index]->token_type != SEMI){ 
        printf("02 || Syntax error [02.26] -> Expected ;\n");
        printf("Exiting...\n\n");
        exit(2);
    }

    ast_current_index++; // skip ;

    temp = generateBlockASTNode(list , count);

    
    return temp;
}

ASTNode *parseWhile()
{
    ast_current_index++; // skip while
    
    if(tokens[ast_current_index]->token_type != L_PARAN){
        printf("02 || Syntax error [02.39] -> Expected (\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    
    ast_current_index++; // skip (
    
    ASTNode *cond = parseExpression(0,true,false,false,false);
    
    if (tokens[ast_current_index]->token_type != R_PARAN)
    {
        printf("02 || Syntax error [02.40] -> Expected )\n");
        printf("Exiting...\n\n");
        exit(2);        
    }

    ast_current_index++; // skip )

    if (tokens[ast_current_index]->token_type != L_BRACES){
        printf("02 || Syntax error [02.41] -> Expected {\n");
        printf("Exiting...\n\n");
        exit(2);
    }

    ast_current_index++; // skip {
    
    ASTNode *list[MAX];
    int count = 0;
    while (tokens[ast_current_index]->token_type != R_BRACES)
    {
        ASTNode *ans = parseStatement();
        list[count++] = ans;
    }

    ast_current_index++; // skip }

    ASTNode *body = generateBlockASTNode(list, count); // generate block node

    ASTNode *temp = generateWhileASTNode(cond, body); // generate while node

    return temp; // return while
}

ASTNode *parseFor()
{
    ast_current_index++; // skip 'for'

    if (tokens[ast_current_index]->token_type != L_PARAN)
    {
        printf("02 || Syntax error [02.42] -> Expected (\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    ast_current_index++; // skip (

    ASTNode* init = parseStatement();        
        
    ASTNode *cond = parseExpression(0, false ,false,false,false);    
    
    ASTNode *upd = parseExpression(0,true,false,false,false);
    
    if (tokens[ast_current_index]->token_type != R_PARAN){
        printf("02 || Syntax error [02.45] -> Expected )\n");
        printf("Exiting...\n\n");
        exit(2);  
    }
    
    ast_current_index++; // skip ')'


    // 👇 ADD THIS
    if (tokens[ast_current_index]->token_type != L_BRACES){
        printf("02 || Syntax error [02.46] -> Expected {\n");
        printf("Exiting...\n\n");
        exit(2);  
    }
    
    ast_current_index++; // skip '{'

    ASTNode *list[MAX];
    int count = 0;

    while (tokens[ast_current_index]->token_type != R_BRACES)
    {
        ASTNode *ans = parseStatement();
        list[count++] = ans;
    }

    ast_current_index++; // skip '}'

    ASTNode *body = generateBlockASTNode(list, count); // generate block node

    ASTNode *temp = generateForASTNode(init, cond, upd, body); // generate for node

    return temp; // return for node
}

ASTNode* parseIf();

ASTNode* parseElse(){
    ast_current_index++; // skip else
    
    if(tokens[ast_current_index]->token_type == KEYWORD_IF){
        ASTNode* body[MAX];
        body[0] = parseIf(); // body of else only contain 1 ast
        int count = 1; // count of statements in else block = 1
        ASTNode *body_2 = generateBlockASTNode(body, count); // generate block node for else

        return body_2;
    } else if(tokens[ast_current_index]->token_type == L_BRACES){
        ast_current_index++; // skip {

        ASTNode* list[MAX];
        int count = 0;

        while (tokens[ast_current_index]->token_type != R_BRACES)
        {
            ASTNode *ans = parseStatement();
            list[count++] = ans;
        }        

        ast_current_index++; // skip }

        ASTNode *body_2 = generateBlockASTNode(list, count); // generate block node for else

        return body_2;
    } else{
        printf("02 || Syntax error [02.33] -> Expected if or {\n");
        printf("Exiting...\n\n");
        exit(2);
    }

    
}

ASTNode* parseIf(){
    ast_current_index++; // skip if

    if(tokens[ast_current_index]->token_type != L_PARAN){
        printf("02 || Syntax error [02.30] -> Expected (\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    
    ast_current_index++; // skip (

    ASTNode* cond = parseExpression(0,true,false,false,false);

    if (tokens[ast_current_index]->token_type != R_PARAN){
        printf("02 || Syntax error [02.31] -> Expected )\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    
    ast_current_index++; // skip ')'

    if (tokens[ast_current_index]->token_type != L_BRACES){
        printf("02 || Syntax error [02.32] -> Expected {\n");
        printf("Exiting...\n\n");
        exit(2);
    }
    
    ast_current_index++; // skip '{'
    
    ASTNode* list[MAX];
    int count = 0;

    while (tokens[ast_current_index]->token_type != R_BRACES)
    {
        ASTNode *ans = parseStatement();
        list[count++] = ans;
    }

    ast_current_index++; // skip }

    ASTNode *body = generateBlockASTNode(list, count);

    if(tokens[ast_current_index]->token_type == KEYWORD_ELSE){ // found else condition also
        ASTNode* body_2 = parseElse();
        ASTNode *temp = generateIfElseASTNode(cond,body,body_2); // generate if-else node
        return temp; // return node
    }

    
    ASTNode *temp = generateIfASTNode(cond,body); // generate if node      

    return temp; // return node
        

}

ASTNode* parsePreUnary(){
    tokenType type = tokens[ast_current_index]->token_type; // store unary operator

    ast_current_index++; // skip current unary operator
    
    ASTNode* right = parseExpression(9,false,false,false,false); // parse right side of unary operator

    if(!isLvalue(right)){ // right is NOT lvalue
        printf("02 || Syntax error [02.38] -> Expected lvalue\n");
        printf("Exiting...\n\n");
        exit(2);   
    }

    right = generateUnaryASTNode(type , right); // generate unary node
    
    return right; // return
}

ASTNode *parseAssignment()
{
    //  a = b + c;
    // reqruires parseExpressions    

    ASTNode* var = NULL;


    if(tokens[ast_current_index+1]->token_type == OP_EQUAL){ // variable assignment
        var = generateVarASTNode(tokens[ast_current_index]->var_name);        
        ast_current_index = ast_current_index + 2;        
    } else if(tokens[ast_current_index+1]->token_type == L_BRACK){ // starting [ of array found
        int store = ast_current_index;
        ast_current_index = ast_current_index + 2;
        ASTNode* arraySize = parseExpression(0,false,true,false,false); // parse the index for assignment
        var = generateArrayASTNode(tokens[store]->var_name , arraySize);
        if(tokens[ast_current_index]->token_type != R_BRACK){ // missing ]
            printf("02 || Syntax error [02.27] -> Expected ]\n");
            printf("Exiting...\n\n");
            exit(2);            
        }
        ast_current_index++; // skip ] token

        if(isUnaryOp(tokens[ast_current_index]->token_type)){ // found unary operator after array
            if(tokens[ast_current_index]->token_type == OP_NOT){ // ! not allowed in post fix
                printf("02 || Syntax error [02.36] -> ! NOT allowed in post-fix\n");
                printf("Exiting...\n\n");
                exit(2);
            }

            var = generateUnaryASTNode(tokens[ast_current_index]->token_type , var); // generate unary node

            if(tokens[ast_current_index+1]->token_type != SEMI){ // expected ; at end of statement
                printf("02 || Syntax error [02.37] -> Expected ;\n");
                printf("Exiting...\n\n");
                exit(2);
            }

            ast_current_index = ast_current_index + 2; // skip unary operand and semi

            return var;
        }

        if(tokens[ast_current_index]->token_type != OP_EQUAL){ // missing =
            printf("02 || Syntax error [02.28] -> Expected =\n");
            printf("Exiting...\n\n");
            exit(2);
        }
        ast_current_index++; // = token -> starting of expression  
            
    } else if(isUnaryOp(tokens[ast_current_index+1]->token_type)){
        if(tokens[ast_current_index+1]->token_type == OP_NOT){ // ! not allowed in post fix
            printf("02 || Syntax error [02.34] -> ! NOT allowed in post-fix\n");
            printf("Exiting...\n\n");
            exit(2);
        }
        var = generateUnaryASTNode(tokens[ast_current_index+1]->token_type , generateVarASTNode(tokens[ast_current_index]->var_name));
        
        if(tokens[ast_current_index+2]->token_type != SEMI){ // expected ; at end of statement
            printf("02 || Syntax error [02.35] -> Expected ;\n");
            printf("Exiting...\n\n");
            exit(2);
        }

        ast_current_index = ast_current_index + 3;
        return var;
    }  
    else{
        printf("02 || Syntax error [02.29] -> Expected = or [\n");
        printf("Exiting...\n\n");
        exit(2);
    }
        
    ASTNode *right = parseExpression(0, false,false,false,false);

    return generateAssignASTNode(var, right);
}

ASTNode *parseDeclarations()
{
    // int a = b + c;
    // might require parseExpression
    
    if(tokens[ast_current_index+1]->token_type != ID){
        printf("02 || Syntax error [02.02] -> Expected variable name for declaration...\n");
        printf("Exiting...\n\n");
        exit(2);
    }    

    if(tokens[ast_current_index+2]->token_type == OP_EQUAL){ // simple variable assignments
        ast_current_index = ast_current_index + 3; // advance index to starting of right side-expression
        
        int store = ast_current_index; // store it for reference to create decl AST node
        ASTNode* temp = NULL; // initialize the node to NULL
        temp = parseExpression(0,false,false,false,false); // parse right side of =
        if(!temp){ // parseExpression Errora
            printf("02 || Syntax error [02.03] -> right side node NOT generated properly\n");
            printf("Exiting...\n\n");
            exit(2);
        }
        temp = generateDeclASTNode(tokens[store-2]->var_name , tokens[store-3]->token_type , temp , false , '\0');
        if(!temp){ // error in decl node
            printf("02 || Syntax error [02.04] -> Declaration node NOT generaated properly\n");
            printf("Exiting...\n\n");
            exit(2);
        }        
        return temp; // return the decl node
    } 
    else if(tokens[ast_current_index+2]->token_type == L_BRACK){ // array starting point
        if(tokens[ast_current_index+3]->token_type != VAL_INT && tokens[ast_current_index+3]->token_type != ID){
            printf("02 || Syntax error [02.05] -> Size of array is invalid\n");
            printf("Exiting...\n\n");
            exit(2);
        }

        if(tokens[ast_current_index+4]->token_type != R_BRACK){
            printf("02 || Syntax error [02.06] -> Expected ]\n");
            printf("Exiting...\n\n");
            exit(2);
        }

        char size_name[MAX_NAME];
        if(tokens[ast_current_index+3]->token_type == VAL_INT){ // size of array is int
            sprintf(size_name , "%d" , tokens[ast_current_index+3]->int_value); // convert int to variable
        } else{ // size of array is variable name
            strcpy(size_name , tokens[ast_current_index+3]->var_name); 
        }

        if(tokens[ast_current_index+5]->token_type == OP_EQUAL){ // array declatationa and initialization
            int store = ast_current_index;
            ASTNode* temp = NULL;

            if(tokens[ast_current_index+3]->token_type != VAL_INT){
                printf("02 || Syntax error [02.07] -> Array initialization error\n");
                printf("Exiting...\n\n");
                exit(2);
            }

            if(tokens[ast_current_index+6]->token_type != L_BRACES){
                printf("02 || Syntax error [02.08] -> Missing { for initialization\n");
                printf("Exiting...\n\n");
                exit(2);
            }

            ast_current_index = ast_current_index + 7; // advance index to first element of initilaization object 
            
            ASTNode* init = pareseArrayInit(tokens[store+3]->int_value); // make and return initialized array node
            char arrSize[MAX_NAME];
            sprintf(arrSize , "%d" , tokens[store+3]->int_value); // convert int to char

            temp = generateDeclASTNode(tokens[store+1]->var_name , tokens[store]->token_type , init , true , arrSize); // generate decl node
            if(!temp){
                printf("02 || Syntax error [02.09] -> Declaration node NOT generated properly\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            return temp; // return decl+init node
            
        } else if (tokens[ast_current_index+5]->token_type == SEMI) { // array declaration ONLY
            ASTNode* temp = NULL;
            temp = generateDeclASTNode(tokens[ast_current_index+1]->var_name , tokens[ast_current_index]->token_type , NULL , true , size_name);
            if(!temp){
                printf("02 || Syntax error [02.10] -> Declaration node NOT generaated properly\n");
                printf("Exiting...\n\n");
                exit(2);
            }
            ast_current_index = ast_current_index + 6; // skip the current declaratiopn line            
            
            return temp; // return decl noed
            
        }
        else{
            printf("02 || Syntax error [02.11] -> Expected either = or ;\n");
            printf("Exiting...\n\n");
            exit(2);
        }
    } else if(tokens[ast_current_index+2]->token_type == SEMI) {  // variable declaration ONLY
        ASTNode* temp = NULL;
        temp = generateDeclASTNode(tokens[ast_current_index+1]->var_name , tokens[ast_current_index]->token_type , NULL , false , NULL); 
                
        ast_current_index = ast_current_index + 3; // skip the current decl line

        return temp; // return decl node
    } 
    else{
        printf("AST = %d\n", ast_current_index);
        printf("02 || Syntax error [02.12] -> Expected either = or [ or ;\n");
        printf("Exiting...\n\n");
        exit(2);
    }   
    
}

ASTNode *parseStatement()
{
    // decision maker and choose appropriate parsers to use
    ASTNode *temp = NULL;
    if (
        tokens[ast_current_index]->token_type == KEYWORD_INT ||
        tokens[ast_current_index]->token_type == KEYWORD_CHAR ||
        tokens[ast_current_index]->token_type == KEYWORD_BOOL ||
        tokens[ast_current_index]->token_type == KEYWORD_DOUBLE)
    {        
        temp = parseDeclarations();
    }
    else if (tokens[ast_current_index]->token_type == ID)
    {
        temp = parseAssignment();
    }
    else if (tokens[ast_current_index]->token_type == R_BRACES) // syntax check remaining
    {        
        ast_current_index++; // last }
        return NULL;
    }
    else if (tokens[ast_current_index]->token_type == KEYWORD_IF)
    {
        temp = parseIf();
    }
    else if (tokens[ast_current_index]->token_type == KEYWORD_ELSE)
    {
        //
    }
    else if (tokens[ast_current_index]->token_type == KEYWORD_FOR)
    {
        temp = parseFor();
    }
    else if (tokens[ast_current_index]->token_type == KEYWORD_WHILE)
    {
        temp = parseWhile();
    } else if(isUnaryOp(tokens[ast_current_index]->token_type)){
        temp = parsePreUnary();        
    }    
    else{
        printf("02 || Syntax error [02.01] -> Invalid token found !!!\n");
        printf("Exiting...\n\n");
        exit(2);
        ast_current_index++;        
    }

    return temp;
}

void generateAllASTNodes()
{ // parseProgram
    ASTNode *node;
    while (ast_current_index < token_count)
    {
        // printf("checking-1\n");
        // printf("ast_current_index -----= %d\n", ast_current_index);
        node = parseStatement();
        // printf("ast_current_index +++++ = %d\n", ast_current_index);
        all_ast[ast_count++] = node;
        // printf("checking-2\n");
    }
    return;
}
