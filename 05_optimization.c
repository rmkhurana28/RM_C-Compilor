/**
 * 05_optimization.c - Code Optimizer
 * 
 * This module performs various code optimizations on the three-address code
 * to improve program efficiency and reduce redundancy.
 * 
 * Features:
 * - Control Flow Graph (CFG) construction
 * - Basic block formation and analysis
 * - Constant folding and propagation
 * - Dead code elimination (DCE)
 * - Copy propagation
 * - Common subexpression elimination
 * - Data flow analysis (reaching definitions, live variable analysis)
 * - Loop optimization considerations
 * 
 * Author: Ridham Khurana
 */

#include "database.h"

int current = 0;
int BlockId = 0;

bool flag = false;

int incomingCount = 0;

address* mark[MAX];
int markCount = 0;

int stringToInt(char* str) {
    return atoi(str);
}

char* calculateConstFoldResultForBool(char* op , char* arg1 , char* arg2){

    char* result = (char*) malloc(MAX_NAME * sizeof(char));
    
    if(isStringSame(op,"&&")){
        if(isStringSame(arg1,"true") && isStringSame(arg2,"true")){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } else if(isStringSame(op,"||")){
        if(isStringSame(arg1,"false") && isStringSame(arg2,"false")){
            strcpy(result , "false");
            return result;
        } else {
            strcpy(result , "true");
            return result;
        }
    } else if(isStringSame(op,"==")){
        if(isStringSame(arg1 , arg2)){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } else if(isStringSame(op,"!=")){
        if(!isStringSame(arg1 , arg2)){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } 

    return NULL;
}

// Perform constant folding - evaluate operations on integer constants at compile time
// This reduces runtime computation by pre-calculating results
char* calculateConstFoldResultForInt(char* op , char* arg1 , char* arg2){
    char* result = (char*) malloc(MAX_NAME * sizeof(char));
    int num1 = stringToInt(arg1);
    int num2 = stringToInt(arg2);

    if(isStringSame(op,"+")){
        sprintf(result, "%d", num1 + num2);
        return result;
    } else if(isStringSame(op,"-")){
        sprintf(result, "%d", num1 - num2);
        return result;
    } else if(isStringSame(op,"*")){
        sprintf(result, "%d", num1 * num2);
        return result;
    } else if(isStringSame(op,"/")) {
        if(num2 == 0){
            return NULL;
        }
        sprintf(result, "%d", num1 / num2);
        return result;
    } else if(isStringSame(op,">")){
        if(num1 > num2){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } else if(isStringSame(op,"<")){
        if(num1 < num2){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } else if(isStringSame(op,">=")){
        if(num1 >= num2){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    } else if(isStringSame(op,"<=")){
        if(num1 <= num2){
            strcpy(result , "true");
            return result;
        } else {
            strcpy(result , "false");
            return result;
        }
    }

    return NULL;
}

bool isInteger(char* str) {
    if (!str || !str[0]) return false;
    
    int i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    
    if (!str[i]) return false;  // just a sign
    
    while (str[i]) {
        if (!isdigit(str[i])) return false;
        i++;
    }
    
    return true;
}

// Check if a string represents a compile-time constant
// Returns true for: integers, doubles, booleans (true/false), string literals
bool isConstant(char* str) {
    if (!str || !str[0]) {
        return false;
    }
    
    // Check if it's an integer
    if (isInteger(str)) {
        return true;
    }
    
    // Check if it's "true" or "false"
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0) {
        return true;
    }
    
    return false;
}

// Helper function to check if a string is a double constant
bool isDouble(char* str) {
    if (!str || !str[0]) return false;
    
    int i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    if (!str[i]) return false;  // just a sign
    
    bool hasDecimal = false;
    while (str[i]) {
        if (str[i] == '.') {
            if (hasDecimal) return false;  // multiple decimals
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
        i++;
    }
    
    return hasDecimal;  // must have a decimal point to be a double
}

// Helper function to check if a string is a character constant 'x'
bool isCharConstant(char* str) {
    if (!str) return false;
    
    int len = strlen(str);
    // Character constant format: 'x' (3 chars) or '\n' (4 chars for escape sequences)
    if (len == 3 && str[0] == '\'' && str[2] == '\'') {
        return true;
    }
    if (len == 4 && str[0] == '\'' && str[1] == '\\' && str[3] == '\'') {
        return true;
    }
    return false;
}

// Helper function to check if a string is a string constant "..."
bool isStringConstant(char* str) {
    if (!str || !str[0]) return false;
    
    int len = strlen(str);
    if (len < 2) return false;
    
    // Must start and end with double quotes
    return (str[0] == '"' && str[len - 1] == '"');
}

// Check if a string is a temporary variable (T0, T1, T2, ...)
// Temporaries are generated during intermediate code generation
bool isTemporaryVariable(char* str) {
    if (!str || !str[0]) return false;
    
    // Temporary variables start with 'T' followed by digits
    if (str[0] != 'T') return false;
    
    // Check if rest is all digits
    int i = 1;
    if (!str[i]) return false;  // just 't' alone
    
    while (str[i]) {
        if (!isdigit(str[i])) return false;
        i++;
    }
    
    return true;
}

// Helper function to lookup a variable in the symbol table
bool isInSymbolTable(char* str) {
    if (!str) return false;
    
    for (int i = 0; i < symbol_count; i++) {
        if (symbolTable[i] && isStringSame(symbolTable[i]->var_name, str)) {
            return true;
        }
    }
    return false;
}

bool isVariable(char* str){
    // NULL or empty string check
    if (!str || !str[0]) {
        return false;
    }
    
    // Check if it's a constant integer (e.g., "123", "-45")
    if (isInteger(str)) {
        return false;
    }
    
    // Check if it's a constant double (e.g., "3.14", "-2.5")
    if (isDouble(str)) {
        return false;
    }
    
    // Check if it's a boolean constant ("true" or "false")
    if (isStringSame(str, "true") || isStringSame(str, "false")) {
        return false;
    }
    
    // Check if it's a character constant (e.g., 'a', '\n')
    if (isCharConstant(str)) {
        return false;
    }
    
    // Check if it's a string constant (e.g., "hello")
    if (isStringConstant(str)) {
        return false;
    }
    
    // Check if it's a temporary variable (e.g., t0, t1, t2)
    if (isTemporaryVariable(str)) {
        return true;
    }
    
    // Check if it's in the symbol table (actual variable)
    if (isInSymbolTable(str)) {
        return true;
    }
    
    // If none of the above, it's not a recognized variable
    return false;
}

bool isKindOfAssign(address* addr){
    // return if instruction is kind of some assignment
    switch(addr->type){
        case ADDR_ASSIGN:
        case ADDR_ARRAY_WRITE:
        case ADDR_ARRAY_READ:
        case ADDR_BINOP:
        case ADDR_UNOP:        
            return true;
        
        // return false if goto/label
        default : 
            return false;
    }
}

bool isProperSimpleAssign(address* addr){
    switch(addr->type){
        case ADDR_ASSIGN:                
            return true;
                
        default : 
            return false;
    }
}

char* getRightSideOfProperAssign(address* addr){
    return addr->assign.arg1;
}

char* getLeftSideOfKindOfAssign(address* addr){
    if (!addr) return NULL;  // Safety check for NULL pointers
    
    switch(addr->type){

        // lhs doesn't exists in goto/labels
        case ADDR_GOTO:
        case ADDR_IF_F_GOTO:
        case ADDR_IF_T_GOTO:
        case ADDR_LABEL:          
            return NULL;        

        // return lhs of all assignment statements
        
        case ADDR_ASSIGN:
            return addr->assign.result;
        
        case ADDR_ARRAY_READ:
            return addr->array_read.result;
        
        case ADDR_ARRAY_WRITE:
            return addr->array_write.array;

        case ADDR_BINOP:
            return addr->binop.result;

        case ADDR_UNOP:
            return addr->unop.result;

        default : 
            return NULL;
    }

    
}

block* generateBlock(int init , int N){
    // allocate memory for block
    block* temp = (block*)malloc(sizeof(block));
    if(temp == NULL){
        printf("05 || Optimization error [05.01] -> Memory allocation failed for basic block\n");
        exit(5);
    }

    // set the ID of block
    temp->blockID = BlockId++;

    // copy next N instructions into the temp starting from init
    for(int i=0 ; i<N ; i++){     
        temp->list[i] = allAddress[init++];
    }
    temp->numberOfAddressesInBlock = N; // set the total number of instructions in the block

    temp->numCFGIn = 0; // initialize number of cfg in=0
    temp->numCFGOut = 0; // initialize number of cfg out=0

    // check if block array is full
    if(block_count >= MAX){
        printf("05 || Optimization error [05.02] -> Maximum limit of basic blocks reached\n");
        exit(5);
    }

    // add block into the block array
    allBlocks[block_count++] = temp;

    // return newly-generated block
    return temp;
}

address** unionOfAddrAndReturnResult(int n1 , address** addr1 , int n2 , address** addr2){
    // allocate memory for array of address pointores

    address** temp = (address**)malloc(MAX_STATEMENTS * sizeof(address*));
    if(temp == NULL){
        printf("05 || Optimization error [05.05] -> Memory allocation failed for address pointer array\n");
        exit(5);
    }
    
    // initialize the entire array to NULL for simplicity
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        temp[i] = NULL;
    }
    
    // number of instructions in temp
    int n = 0;
    
    if(n1 == 0){ // if first set (addr1) is empty, add all instructions of addr2 to temp
        for(int i=0 ; i<n2 ; i++){
            temp[n++] = addr2[i];
        }
    }else{ // if first set(addr1) is NOT empty, add all instructions of addr1 to temp
        for(int i=0 ; i<n1 ; i++){
            temp[n++] = addr1[i]; 
        }
    }
    

    // if n1=0, addr2 elements already eexists, if n2=0, addr1 elements already exists
    if(n1 == 0 || n2 == 0) return temp; // return temp 

    // helper flag
    bool exist = false;

    // At this point, if any of the sets were empty, it alraedy returned the final union
    // Now, addr1 instructions already exists in temp, now add temp2 instructions if they dont exists

    // add addr2 instructions
    for(int i=0 ; i<n2 ; i++){ // loop througth addr2
        exist = false;
        for(int j=0 ; j<n ; j++){ // loop through current temp array (same as addr1)            
            if(addr2[i] == temp[j]){ // if addr2 instruction exists in temp, then set exist=true and break
                exist = true;
                break;
            }
        }
        if(exist) continue; // addr2 instruction already exists in temp, no need to add again

        // add addr2 instruction to temp
        temp[n++] = addr2[i];
    }

    // return final union of both sets
    return temp;
    
}

address** minusOfTwoAddr(int n1 , address** addr1 , int n2 , address** addr2){

    // allocate memory for array of address pointors 
    address** temp = (address**)malloc(MAX_STATEMENTS * sizeof(address*));
    if(temp == NULL){
        printf("05 || Optimization error [05.05] -> Memory allocation failed for address pointer array\n");
        exit(5);
    }
    
    // initialize the array to NULL for simplicity
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        temp[i] = NULL;
    }
    
    // if first set (addr1) is empty, return empty set (NULL)
    if(n1 == 0) return temp;
    
    // number of instructions in temp
    int n = 0;

    // helper flag
    bool change = false;

    // loop thought all instructions in addr1
    for(int i=0 ; i<n1 ; i++){
        change = false;
        // loop through all instructions in addr2
        for(int j=0 ; j<n2 ; j++){
            
            // check if i(th) instruction of addr1 is same as j(th) instruction of addr2
            if(addr1[i] == addr2[j]){                                
                change = true; // change the flag and break the loop
                break;
            }
        }

        // if addr2 instruction also exists in addr1, dont add that to our result (temp)
        if(change) continue;

        // i(th) instruction of addr1 doesnt exists in addr2, so, add to result (temp)
        temp[n++] = addr1[i];
    }

    // return final result array
    return temp;

    
}

char** unionOfCharAndReturnResult(int n1 , char str1[][MAX_STATEMENTS] , int n2 , char** str2){
    // memory allocation for array of strings
    char** temp = (char**)malloc(sizeof(char*) * MAX_STATEMENTS);    
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        temp[i] = (char*)malloc(sizeof(char) * MAX_STATEMENTS);
        memset(temp[i], 0, MAX_STATEMENTS);  // Initialize to zero
    }

    // if first set is empty
    if(n1 == 0){
        if(n2 == 0 || str2 == NULL) return NULL; // if both empty or str2 is NULL, return null
        int copied = 0;
        for(int i=0 ; i<n2 && str2[i] != NULL; i++){
            strcpy(temp[copied] , str2[i]);
            copied++;
        }
        temp[copied] = NULL;
        return temp;
    }

    

    for(int i=0 ; i<n1 ; i++){
        strcpy(temp[i] , str1[i]);
    }
    temp[n1] = NULL;

    

    if(n2 == 0 || str2 == NULL) return temp; // if 2nd set is empty or NULL, return temp [set-1 values already copied]

    bool isExist = false;

    // loop through all elments of set-2
    for(int i=0 ; i<n2 && str2[i] != NULL; i++){
        isExist = false;

        // loop through all elements of temp
        for(int j=0 ; j<n1 ; j++){
            // if str-2 element alr exists in temp, break this loop
            if(isStringSame(str2[i] , temp[j])){
                isExist = true;
                break;
            }
        }

        if(isExist) continue; // element alr exists, continue to next element in str-2

        // add element to temp
        strcpy(temp[n1] , str2[i]);
        n1++;        
    }

    temp[n1] = NULL;

    return temp;
    
}

// Compute set difference: str1 - str2
// Returns elements in str1 that are NOT in str2
// Used for data flow analysis: e.g., IN - KILL
char** minusOfTwoChar(int n1 , char str1[][MAX_STATEMENTS] , int n2 , char str2[][MAX_STATEMENTS]){
    // memory allocation for array of strings
    char** temp = (char**)malloc(sizeof(char*) * MAX_STATEMENTS);    
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        temp[i] = (char*)malloc(sizeof(char) * MAX_STATEMENTS);        
    }

    if(n1 == 0) return NULL;

    if(n2 == 0){
        for(int i=0 ; i<n1 ; i++){
            strcpy(temp[i] , str1[i]);
        }
        temp[n1] = NULL;
        return temp;  // Return here!
    }

    bool isExist = false;
    int n = 0;

    // loop throguh all elements of str1
    for(int i=0 ; i<n1 ; i++){
        isExist = false;
        // loop through all elements of str2
        for(int j=0 ; j<n2 ; j++){
            // check if the string value matches
            if(isStringSame(str1[i] , str2[j])){
                isExist = true;
                break;
            }
        }

        if(isExist) continue;

        // add str[i] to temp
        strcpy(temp[n] , str1[i]);
        n++;
    }

    temp[n] = NULL;

    return temp;
}

int findIndexOfBlock(block* currBlock){
    // loop thought all blocks and return the index of current block
    for(int i=0 ; i<block_count ; i++){
        if(currBlock == allBlocks[i]) return i;
    }

    // current block doesn't exists
    printf("05 || Optimization error [05.06] -> Block not found in block array\n");
    exit(5);
    return -1;
}

blockProp* generateEmptyBlockProp(){
    // generate empty blockProp and return
    blockProp* temp = (blockProp*)malloc(sizeof(blockProp));
    if(temp == NULL){
        printf("05 || Optimization error [05.03] -> Memory allocation failed for block properties\n");
        exit(5);
    }
    return temp;
}

blockPropLive* generateEmptyBlockLiveProp(){
    // generate empty blockProp and return
    blockPropLive* temp = (blockPropLive*)malloc(sizeof(blockPropLive));
    if(temp == NULL){
        printf("05 || Optimization error [05.03] -> Memory allocation failed for block properties\n");
        exit(5);
    }
    return temp;
}

void calculateGENForBlockAndStoreTo(block* currBlock , blockProp* currBlockProp){
    // loop through all the instructions in the current block
    for(int i=0 ; i<currBlock->numberOfAddressesInBlock ; i++){
        if(!isKindOfAssign(currBlock->list[i])) continue; // skip goto and labels

        // find if the left side alr in gen
        // loop though al elements already existing in GEN of current block
        for(int j=0 ; j<currBlockProp->numGen ; j++){ 
            // check if the lhs of (i th) instruction already exists in GEN of current block
            if(isStringSame(getLeftSideOfKindOfAssign(currBlock->list[i]),getLeftSideOfKindOfAssign(currBlockProp->gen[j]))){ // overwriting of variable/temp in same block
                
                // remove prev address from the GEN
                for(int k=j ; k<currBlockProp->numGen ; k++){
                    currBlockProp->gen[k] = currBlockProp->gen[k+1];
                }
                currBlockProp->gen[currBlockProp->numGen--] = NULL;

                // instruction found, can safely break the j loop
                break;                
            }
            
        }

        // add current instruction to the GEN of current block
        currBlockProp->gen[currBlockProp->numGen++] = currBlock->list[i];
    }

    return;
}

void calculateKILLForBlockAndStoreTo(block* currBlock , blockProp* currBlockProp){
    // loop through all instructions in current block
    for(int i=0 ; i<currBlock->numberOfAddressesInBlock ; i++){ 
        if(!isKindOfAssign(currBlock->list[i])) continue; // skip if goto or label
        
        // loop through all blocks except current
        for(int j=0 ; j<block_count ; j++){
            if(allBlocks[j] == currBlock) continue; // skip current block

            // loop through each instruction in each block
            for(int k=0 ; k<allBlocks[j]->numberOfAddressesInBlock ; k++){
                if(!isKindOfAssign(allBlocks[j]->list[k])) continue; // skip if goto or label
                
                // check if left of assign in curr block's ith instruction same as left of assign in j block's kth instruction
                if(isStringSame(getLeftSideOfKindOfAssign(currBlock->list[i]) , getLeftSideOfKindOfAssign(allBlocks[j]->list[k]))){
                    // add to kill if matched
                    currBlockProp->kill[currBlockProp->numKill++] = allBlocks[j]->list[k];
                }
            }
        }
    }

    return;
}

void calculateINForBlockAndStoreTo(block* currBlock , blockProp* currBlockProp){
    // if cfgIN is empty, IN is empty
    if(currBlock->numCFGIn == 0){
        currBlockProp->numIn = 0;
        return;
    }

    // helper to store the results
    address** result = NULL;
    
    // number of instructions in result
    int n = 0;

    // loop through all blocks in cfgIN of current block
    for(int i=0 ; i<currBlock->numCFGIn ; i++){
        int predIndex = findIndexOfBlock(currBlock->cfg_in[i]);
        
        // store the union of result and OUT of predecessor in the result
        result = unionOfAddrAndReturnResult(n , result , allBlockProps[predIndex]->numOut , allBlockProps[predIndex]->out);
        
        // update the number of instructions in result
        n = 0;
        if(result != NULL) {
            while(result[n] != NULL) n++;
        }
    }

    // clear the old IN array
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        currBlockProp->in[i] = NULL;
    }

    // set the number of IN instructions in current blockProp
    currBlockProp->numIn = n;

    // copy all the instructions of result into IN of current blockProp
    for(int i=0 ; i<n ; i++){
        currBlockProp->in[i] = result[i];
    }

    // free the temporary result array (not the addresses themselves, just the array)
    if(result != NULL) free(result);

    return;

}

void calculateOUTForBlockAndStoreTo(block* currBlock , blockProp* currBlockProp){
    (void)currBlock; // unused parameter
    
    // evalute IN - KILL of current block and store in helper
    address** temp = minusOfTwoAddr(currBlockProp->numIn , currBlockProp->in , currBlockProp->numKill , currBlockProp->kill);

    // number of instructions in temp
    int check = 0; // initialize to 0
    while(temp[check] != NULL) check++; // update number of instructions        

    // update temp to be union of GEN and result of previous MINUS
    address** temp2 = unionOfAddrAndReturnResult(currBlockProp->numGen , currBlockProp->gen , check , temp);
    
    // free the first temp (result of minus operation)
    if(temp != NULL) free(temp);
    temp = temp2;
    
    check = 0;
    
    // clear the old OUT array
    for(int i=0 ; i<MAX_STATEMENTS ; i++){
        currBlockProp->out[i] = NULL;
    }
    
    // copy temp into OUT and count elements
    while(temp[check] != NULL){
        currBlockProp->out[check] = temp[check];
        check++;
    }

    // save the number of instructions in temp to blockProp numOUT
    currBlockProp->numOut = check;        

    // free the final temp array (not the addresses themselves, just the array)
    if(temp != NULL) free(temp);

    return;
}

// declaration to avoid error
char* returnConstIfVariableBeReducedToConst(char* varName , int indexOfBlock , int indexOfInstruction);

// 
char* canThisInstructionBeReducedToConstant(address* addr , int indexOfBlockOfAddr , int IndexOfInstructionOfAddr){
    if(!isKindOfAssign(addr)) return NULL;
    
    char* helper1 = NULL;
    char* helper2 = NULL;

    if(addr->type == ADDR_UNOP){
        helper1 = returnConstIfVariableBeReducedToConst(allBlocks[indexOfBlockOfAddr]->list[IndexOfInstructionOfAddr]->unop.arg1 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);
        if(helper1){
            flag = true;
            strcpy(addr->unop.arg1 , helper1);
        } 

        return NULL;
    } else if(addr->type == ADDR_BINOP){
        helper1 = returnConstIfVariableBeReducedToConst(allBlocks[indexOfBlockOfAddr]->list[IndexOfInstructionOfAddr]->binop.arg1 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);
        helper2 = returnConstIfVariableBeReducedToConst(allBlocks[indexOfBlockOfAddr]->list[IndexOfInstructionOfAddr]->binop.arg2 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);
        if(helper1){
            flag = true;
            strcpy(addr->binop.arg1 , helper1);
        }
        if(helper2){
            flag = true;
            strcpy(addr->binop.arg2 , helper2);
        }        

        return NULL;
    }
    
    return NULL;
}

//
char* returnConstIfVariableBeReducedToConst(char* varName , int indexOfBlock , int indexOfInstruction){

    char* helper = NULL;            

    // UPDATE instead of going through gen of current block, go through all the instructions of current block, till it reaches itself
    for(int i=0 ; i<allBlocks[indexOfBlock]->numberOfAddressesInBlock ; i++){
        if(i == indexOfInstruction) break;

        if(!isProperSimpleAssign(allBlocks[indexOfBlock]->list[i])) continue;

        if(isStringSame(getLeftSideOfKindOfAssign(allBlocks[indexOfBlock]->list[i]) , varName)){
            if(isConstant(getRightSideOfProperAssign(allBlocks[indexOfBlock]->list[i]))){
                return getRightSideOfProperAssign(allBlocks[indexOfBlock]->list[i]);
            }

            return returnConstIfVariableBeReducedToConst(getRightSideOfProperAssign(allBlocks[indexOfBlock]->list[i]) , indexOfBlock , i);
        }
    }

    // loop throught IN of current blocks
    incomingCount = 0;
    for(int i=0 ; i<allBlockProps[indexOfBlock]->numIn ; i++){
        if(!isProperSimpleAssign(allBlockProps[indexOfBlock]->in[i])) continue;
        
        if(isStringSame(getLeftSideOfKindOfAssign(allBlockProps[indexOfBlock]->in[i]) , varName)){
            incomingCount++;
            if(isConstant(getRightSideOfProperAssign(allBlockProps[indexOfBlock]->in[i]))){ // x = 10/true/false
                helper = getRightSideOfProperAssign(allBlockProps[indexOfBlock]->in[i]);
                continue;                
            }

            helper = returnConstIfVariableBeReducedToConst(getRightSideOfProperAssign(allBlockProps[indexOfBlock]->in[i]) , indexOfBlock , i);
            continue;
        }
    }

    

    

    if(helper && incomingCount <= 1) return helper;

    return NULL;
}


char* returnReducesVariableIfOriginalVariableCanBeReduces(char* varName , int indexOfBlock , int indexOfInstruction);

//
char* callThisInstructionBeReducesToAnotherVariable(address* addr , int indexOfBlockOfAddr , int IndexOfInstructionOfAddr){
    if(!isKindOfAssign(addr)) return NULL;

    char* helper1 = NULL;
    char* helper2 = NULL;

    if(addr->type == ADDR_BINOP){
        helper1 = returnReducesVariableIfOriginalVariableCanBeReduces(addr->binop.arg1 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);
        helper2 = returnReducesVariableIfOriginalVariableCanBeReduces(addr->binop.arg2 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);

        if(helper1){
            flag = true;
            strcpy(addr->binop.arg1 , helper1);
        }
        if(helper2){
            flag = true;
            strcpy(addr->binop.arg2 , helper2);
        }        

        return NULL;
    } else if(addr->type == ADDR_UNOP){
        helper1 = returnReducesVariableIfOriginalVariableCanBeReduces(addr->unop.arg1 , indexOfBlockOfAddr , IndexOfInstructionOfAddr);

        if(helper1){
            flag = true;
            strcpy(addr->unop.arg1 , helper1);
        } 

        return NULL;
    }
    
    return NULL;
}

//
char* returnReducesVariableIfOriginalVariableCanBeReduces(char* varName , int indexOfBlock , int indexOfInstruction){
    char* helper = NULL;

    // loop through all instructions of current block till that instruction
    for(int i=0 ; i<allBlocks[indexOfBlock]->numberOfAddressesInBlock ; i++){
        if(i == indexOfInstruction) break;

        if(!isProperSimpleAssign(allBlocks[indexOfBlock]->list[i])) continue;

        if(isStringSame(varName , getLeftSideOfKindOfAssign(allBlocks[indexOfBlock]->list[i]))){
            if(isConstant(getRightSideOfProperAssign(allBlocks[indexOfBlock]->list[i]))) continue;

            return getRightSideOfProperAssign(allBlocks[indexOfBlock]->list[i]);
        }
    }

    incomingCount = 0;
    // loop through all instructions of IN of current block
    for(int i=0 ; i<allBlockProps[indexOfBlock]->numIn ; i++){
        if(!isProperSimpleAssign(allBlockProps[indexOfBlock]->in[i])) continue;

        if(isStringSame(varName , getLeftSideOfKindOfAssign(allBlockProps[indexOfBlock]->in[i]))){
            incomingCount++;
            if(isConstant(getRightSideOfProperAssign(allBlockProps[indexOfBlock]->in[i]))) continue;

            helper = getRightSideOfProperAssign(allBlockProps[indexOfBlock]->in[i]);
            continue;
        }
    }

    if(helper && incomingCount <= 1) return helper;

    return NULL;
}

// check if next is leader
bool isNextGoto(int index){

    // if current instruction is last, return false (next instruction doesnt exist, so can NOT be goto obv)
    if(index >= addr_count-1) return false;

    // if next instruction is goto/false_goto/true_goto , return true
    if(allAddress[index+1]->type == ADDR_GOTO || allAddress[index+1]->type == ADDR_IF_F_GOTO || allAddress[index+1]->type == ADDR_IF_T_GOTO) return true;

    // return false
    return false;
}

// check if next is label
bool isNextLabel(int index){
    // if current instruction is last, return false (next instruction doesnt exist, so can NOT be label obv)
    if(index >= addr_count-1) return false;

    // return true if next instruction is label, false if not
    return allAddress[index+1]->type == ADDR_LABEL;
}

void evaluate(){    
    // add current instruction to block
    int numberToAdd = 1;

    // store the current instruction index    
    int init = current;

    while(current+numberToAdd < addr_count){ // while instructions exists    
        // Check if the next instruction (at current+numberToAdd) is a label
        if(allAddress[current+numberToAdd]->type == ADDR_LABEL){ 
            // Block ends here, new block starts from the label
            // Don't include the label in this block
            generateBlock(init , numberToAdd);

            // increment the current index to next instruction (the label)
            current += numberToAdd;
            return;
        }

        // Check if current last instruction (at current+numberToAdd-1) is a goto/if-goto
        if(allAddress[current+numberToAdd-1]->type == ADDR_GOTO || 
           allAddress[current+numberToAdd-1]->type == ADDR_IF_F_GOTO || 
           allAddress[current+numberToAdd-1]->type == ADDR_IF_T_GOTO){ 
            // Block ends after the goto, new block starts from next instruction
            generateBlock(init , numberToAdd);

            // increment the current index to the instruction after goto
            current += numberToAdd;
            return;
        }
        
        // if next instruction is not goto/label , add current instruction and continue the while loop (if instruction exists)
        numberToAdd++; 
    }

    // this block contains all the remaining instructions (last block)
    generateBlock(init , numberToAdd);

    // increment the current index and end the block-generation process
    current += numberToAdd;
    
    return;
    
    
}

void formCFG(){
    for(int i=0 ; i<block_count ; i++){
        int n = allBlocks[i]->numberOfAddressesInBlock;
        int index;
        if(allBlocks[i]->list[n-1]->type == ADDR_GOTO){ // last is goto
            for(int j=0 ; j<block_count ; j++){
                if(allBlocks[j]->list[0]->type != ADDR_LABEL) continue;
                index = 0;
                while(index < allBlocks[j]->numberOfAddressesInBlock && allBlocks[j]->list[index]->type == ADDR_LABEL){                        
                     if(isStringSame(allBlocks[i]->list[allBlocks[i]->numberOfAddressesInBlock-1]->goto_stmt.target , allBlocks[j]->list[index]->label.labelNumber)){
                        allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[j];
                        allBlocks[j]->cfg_in[allBlocks[j]->numCFGIn++] = allBlocks[i];
                    }
                    index++;
                }
            }
        } else if(allBlocks[i]->list[n-1]->type == ADDR_IF_F_GOTO){ // if false goto
            if(i != block_count-1){
                allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[i+1]; // add out to current block           
                allBlocks[i+1]->cfg_in[allBlocks[i+1]->numCFGIn++] = allBlocks[i]; // add id to next block 
            }

            for(int j=0 ; j<block_count ; j++){
                if(allBlocks[j]->list[0]->type != ADDR_LABEL) continue;

                index = 0;
                while(index < allBlocks[j]->numberOfAddressesInBlock && allBlocks[j]->list[index]->type == ADDR_LABEL){
                    if(isStringSame(allBlocks[i]->list[n-1]->if_false.target , allBlocks[j]->list[index]->label.labelNumber)){
                        allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[j];
                        allBlocks[j]->cfg_in[allBlocks[j]->numCFGIn++] = allBlocks[i];
                        break;
                    }
                    index++;
                }                
            }
        } else if(allBlocks[i]->list[n-1]->type == ADDR_IF_T_GOTO){ // if true goto
            if(i != block_count-1){
                allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[i+1]; // add out to current block           
                allBlocks[i+1]->cfg_in[allBlocks[i+1]->numCFGIn++] = allBlocks[i]; // add id to next block 
            }

            for(int j=0 ; j<block_count ; j++){
                if(allBlocks[j]->list[0]->type != ADDR_LABEL) continue;

                index = 0;
                while(index < allBlocks[j]->numberOfAddressesInBlock && allBlocks[j]->list[index]->type == ADDR_LABEL){
                                        
                    if(isStringSame(allBlocks[i]->list[n-1]->if_true.target , allBlocks[j]->list[index]->label.labelNumber)){
                        allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[j];
                        allBlocks[j]->cfg_in[allBlocks[j]->numCFGIn++] = allBlocks[i];
                    }
                    index++;
                }
            }
        } else{
            if(i == block_count-1) return;
            
            allBlocks[i]->cfg_out[allBlocks[i]->numCFGOut++] = allBlocks[i+1]; // add out to current block           
            allBlocks[i+1]->cfg_in[allBlocks[i+1]->numCFGIn++] = allBlocks[i]; // add id to next block 
        }
    }

    return;
}

void getPropertyOfBlock(int index_of_block , block* currBlock){
    // For first-time generation, check index matches
    if(allBlockProps[index_of_block] == NULL){
        // First time generating this block's properties
        if(index_of_block != block_prop_count){
            printf("05 || Optimization error [05.04] -> Block index mismatch during property assignment\n");
            exit(5);
        }
        
        // generate empty prop object
        blockProp* temp_prop = generateEmptyBlockProp();
        allBlockProps[block_prop_count++] = temp_prop;
    }
    
    // Now calculate/recalculate properties (works for both first time and updates)
    blockProp* prop = allBlockProps[index_of_block];
    
    // calculate gen
    calculateGENForBlockAndStoreTo(currBlock , prop);

    // calculate kill
    calculateKILLForBlockAndStoreTo(currBlock , prop);

    // calculate in
    calculateINForBlockAndStoreTo(currBlock , prop);

    // calculate out
    calculateOUTForBlockAndStoreTo(currBlock , prop);
}

void generateAllBlocks(){
    while(current < addr_count){
        // loop through all 3-addr codes
        evaluate();
    }
    return;
}

void getPropertiesOfAllBlocks(){
    // STEP 1: Create all blockProp objects (only first time)
    if(allBlockProps[0] == NULL){
        for(int i=0 ; i<block_count ; i++){
            allBlockProps[i] = generateEmptyBlockProp();
            block_prop_count++;
        }
    }
    
    // STEP 2: Calculate GEN and KILL for all blocks
    for(int i=0 ; i<block_count ; i++){
        calculateGENForBlockAndStoreTo(allBlocks[i] , allBlockProps[i]);
        calculateKILLForBlockAndStoreTo(allBlocks[i] , allBlockProps[i]);
    }
    
    // STEP 3: Calculate IN and OUT (single pass)
    for(int i=0 ; i<block_count ; i++){
        calculateINForBlockAndStoreTo(allBlocks[i] , allBlockProps[i]);
        calculateOUTForBlockAndStoreTo(allBlocks[i] , allBlockProps[i]);
    }
}

void checkForConstantPropagation(){

    char* temp = (char*) malloc(MAX_NAME * sizeof(char));

    // loop throught all blocks
    for(int i=0 ; i<block_count ; i++){
        // loop through all instructions of that block
        for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            if(!isKindOfAssign(allBlocks[i]->list[j])) continue; // skip goto and labels            

            if(allBlocks[i]->list[j]->type == ADDR_ASSIGN){
                if(isConstant(allBlocks[i]->list[j]->assign.arg1)) continue;                

                temp = NULL;
                incomingCount = 0;
                temp = returnConstIfVariableBeReducedToConst(allBlocks[i]->list[j]->assign.arg1 , i , j);
                
                if(temp && incomingCount <= 1){
                    strcpy(allBlocks[i]->list[j]->assign.arg1,temp);
                }
            } else if(allBlocks[i]->list[j]->type == ADDR_BINOP){
                temp = NULL;
                temp = canThisInstructionBeReducedToConstant(allBlocks[i]->list[j] , i,j);
                if(temp){
                    flag = true;
                }
            } else if(allBlocks[i]->list[j]->type == ADDR_UNOP){
                temp = NULL;
                temp = canThisInstructionBeReducedToConstant(allBlocks[i]->list[j] , i,j);
                if(temp){
                    flag = true;
                }
            }            
            
            
        }
    }

    return;
}

void checkForConstantFolding(){

    char* store = NULL;

    // loop through all blocks
    for(int i=0 ; i<block_count ; i++){
        // loop through all instruction of i(th) block
        for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            if(!isKindOfAssign(allBlocks[i]->list[j])) continue; // skip goto and labels
            
            if(allBlocks[i]->list[j]->type == ADDR_BINOP){
                if(!isConstant(allBlocks[i]->list[j]->binop.arg1) || !isConstant(allBlocks[i]->list[j]->binop.arg2)) continue;

                // atleast one of them is integer
                if(isInteger(allBlocks[i]->list[j]->binop.arg1) || isInteger(allBlocks[i]->list[j]->binop.arg2)){
                    if(!isInteger(allBlocks[i]->list[j]->binop.arg1) || !isInteger(allBlocks[i]->list[j]->binop.arg2)) continue;

                    // both int
                    store = NULL;
                    store = calculateConstFoldResultForInt(allBlocks[i]->list[j]->binop.op , allBlocks[i]->list[j]->binop.arg1 , allBlocks[i]->list[j]->binop.arg2);

                    if(store){
                        // change the instruction to assignment
                        char temp[MAX_NAME];
                        strcpy(temp , allBlocks[i]->list[j]->binop.result);
                        allBlocks[i]->list[j]->type = ADDR_ASSIGN;
                        strcpy(allBlocks[i]->list[j]->assign.result , temp);
                        strcpy(allBlocks[i]->list[j]->assign.arg1 , store);

                        flag = true;
                    }
                }

                // both are bool (true/false)
                
                store = NULL;
                store = calculateConstFoldResultForBool(allBlocks[i]->list[j]->binop.op , allBlocks[i]->list[j]->binop.arg1 , allBlocks[i]->list[j]->binop.arg2);

                if(store){
                    // change the instruction to assignment
                    char temp[MAX_NAME];
                    strcpy(temp , allBlocks[i]->list[j]->binop.result);
                    allBlocks[i]->list[j]->type = ADDR_ASSIGN;
                    strcpy(allBlocks[i]->list[j]->assign.result , temp);
                    strcpy(allBlocks[i]->list[j]->assign.arg1 , store);

                    flag = true;
                }

                

            } else if(allBlocks[i]->list[j]->type == ADDR_UNOP){
                // only NOT operator exists
                if(!isConstant(allBlocks[i]->list[j]->unop.arg1)) continue;

                store = NULL;
                if(isStringSame(allBlocks[i]->list[j]->unop.op , "!")){
                    if(isStringSame(allBlocks[i]->list[j]->unop.arg1 , "true")){
                        store = (char*) malloc(MAX_NAME * sizeof(char));
                        strcpy(store , "false");
                    } else if(isStringSame(allBlocks[i]->list[j]->unop.arg1 , "false")){
                        store = (char*) malloc(MAX_NAME * sizeof(char));
                        strcpy(store , "true");
                    }
                }
                if(store){
                    // change the instruction to assignment
                    char temp[MAX_NAME];
                    strcpy(temp , allBlocks[i]->list[j]->unop.result);
                    allBlocks[i]->list[j]->type = ADDR_ASSIGN;
                    strcpy(allBlocks[i]->list[j]->assign.result , temp);
                    strcpy(allBlocks[i]->list[j]->assign.arg1 , store);

                    flag = true;
                }
            }
            
        }
    }
}

void checkForCopyPropagation(){
    char* temp = NULL;

    // loop through all blocks
    for(int i=0 ; i<block_count ; i++){
        // loop through all instructions of that block
        for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            if(!isKindOfAssign(allBlocks[i]->list[j])) continue;

            if(allBlocks[i]->list[j]->type == ADDR_ASSIGN){
                if(isConstant(allBlocks[i]->list[j]->assign.arg1)) continue;
                
                incomingCount = 0;
                temp = returnReducesVariableIfOriginalVariableCanBeReduces(getRightSideOfProperAssign(allBlocks[i]->list[j]) , i , j);
                
                if(temp && incomingCount <= 1 && !isStringSame(temp , getRightSideOfProperAssign(allBlocks[i]->list[j]))){
                    strcpy(allBlocks[i]->list[j]->assign.arg1 , temp);
                    // flag = true;
                }
            } else if(allBlocks[i]->list[j]->type == ADDR_BINOP){
                temp = NULL;
                temp = callThisInstructionBeReducesToAnotherVariable(allBlocks[i]->list[j] , i , j);
                if(temp && !isStringSame(temp , getRightSideOfProperAssign(allBlocks[i]->list[j]))){
                    // flag = true;
                }
            } else if(allBlocks[i]->list[j]->type == ADDR_UNOP){
                temp = NULL;
                temp = callThisInstructionBeReducesToAnotherVariable(allBlocks[i]->list[j] , i , j);
                if(temp && !isStringSame(temp , getRightSideOfProperAssign(allBlocks[i]->list[j]))){
                    // flag = true;
                }
            }

            
        }
    }

    return;
}

void attachBlockLivePropWithAllBlocks(){    
    // Reset to reuse block properties for fresh analysis after DCE
    block_prop_live_count = 0;

    for(int i=0 ; i<block_count ; i++){
        allBlockPropsLive[block_prop_live_count] = generateEmptyBlockLiveProp();
        allBlockPropsLive[block_prop_live_count]->numGenLive = 0;
        allBlockPropsLive[block_prop_live_count]->numKillLive = 0;
        allBlockPropsLive[block_prop_live_count]->numInLive = 0;
        allBlockPropsLive[block_prop_live_count++]->numOutLive = 0;
    }

    return;
}

// total variables used in given 3-addr instruction
char** getAllVariablesUsed(int* n , address* addr){    
    // char temp[MAX_NAME][MAX_NAME];
    char** temp = (char**)malloc(MAX_NAME * sizeof(char*));
    for(int i = 0; i < MAX_NAME; i++){
        temp[i] = (char*)malloc(MAX_NAME * sizeof(char));
    }

    *n = 0;

    switch(addr->type){
        case ADDR_ASSIGN : 
            if(isVariable(addr->assign.arg1)) strcpy(temp[(*n)++], addr->assign.arg1);                
            break;
        
        case ADDR_ARRAY_READ : 
            if(isVariable(addr->array_read.array)) strcpy(temp[(*n)++], addr->array_read.array);   
            if(isVariable(addr->array_read.index)) strcpy(temp[(*n)++], addr->array_read.index);
            break;

        case ADDR_ARRAY_WRITE : 
            // Array write USES index and value, but NOT the array itself
            if(isVariable(addr->array_write.index)) strcpy(temp[(*n)++], addr->array_write.index);
            if(isVariable(addr->array_write.value)) strcpy(temp[(*n)++], addr->array_write.value);
            break;

        case ADDR_BINOP : 
            if(isVariable(addr->binop.arg1)) strcpy(temp[(*n)++], addr->binop.arg1);
            if(isVariable(addr->binop.arg2)) strcpy(temp[(*n)++], addr->binop.arg2);
            break;

        case ADDR_UNOP : 
            if(isVariable(addr->unop.arg1)) strcpy(temp[(*n)++], addr->unop.arg1);
            break;

        case ADDR_IF_T_GOTO : 
            if(isVariable(addr->if_true.condition)) strcpy(temp[(*n)++], addr->if_true.condition);
            break;

        case ADDR_IF_F_GOTO : 
            if(isVariable(addr->if_false.condition)) strcpy(temp[(*n)++], addr->if_false.condition);
            break;

        default:
            break;
    }

    return temp;
}

void calculateLiveGenKill(){
    char* temp;
    bool found;
    int n = 0;
    char** temp_array;

    // loop through all the blocks
    for(int i=0 ; i<block_count ; i++){
        // CRITICAL: Reset GEN and KILL for fresh calculation
        allBlockPropsLive[i]->numGenLive = 0;
        allBlockPropsLive[i]->numKillLive = 0;
        
        // loop through all instructions of that block from the end to start
        for(int j=allBlocks[i]->numberOfAddressesInBlock-1 ; j>=0 ; j--){
        // for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            
            // gen starts
            n = 0;
            temp_array = getAllVariablesUsed(&n , allBlocks[i]->list[j]);               

            // loop through all the variable names in temp_array
            for(int k=0 ; k<n ; k++){
                flag = false;
                // loop through all the variable names in KILL_LIVE
                for(int l=0 ; l<allBlockPropsLive[i]->numKillLive ; l++){
                    if(isStringSame(temp_array[k] , allBlockPropsLive[i]->killLive[l])){
                        flag = true;
                        break;
                    }
                }
                if(flag) continue; // var was defined before using

                // check if word already exists in gen
                for(int l=0 ; l<allBlockPropsLive[i]->numGenLive ; l++){
                    if(isStringSame(temp_array[k] , allBlockPropsLive[i]->genLive[l])){
                        flag = true;
                        break;  
                    }
                }

                if(flag) continue; // var already in gen

                // add to gen
                flag = true;
                strcpy(allBlockPropsLive[i]->genLive[allBlockPropsLive[i]->numGenLive++] , temp_array[k]);
            }


            
            // kill starts
            if(allBlocks[i]->list[j]->type == ADDR_ARRAY_WRITE) continue; // skip array write - doesn't kill scalar variables
            if(!isKindOfAssign(allBlocks[i]->list[j])) continue; // skip goto and labels

            temp = getLeftSideOfKindOfAssign(allBlocks[i]->list[j]); // get left side varibale name
            // loop through all varibales in the OUT of current block live property
            found = false;
            for(int k=0 ; k<allBlockPropsLive[i]->numKillLive ; k++){
                if(isStringSame(temp,allBlockPropsLive[i]->killLive[k])){
                    found = true;
                    break; // variable lhs name already in live_KILL     
                }            
            }
            if(found) continue; // variable lhs name already in live_KILL, dont do anything, just check next isntruction now

            
            // if the same variable exists in gen, remove it
            for(int k=0 ; k<allBlockPropsLive[i]->numGenLive ; k++){
                if(isStringSame(temp , allBlockPropsLive[i]->genLive[k])){
                    // remove that from the genLive
                    for(int l=k ; l<allBlockPropsLive[i]->numGenLive-1 ; l++){
                        strcpy(allBlockPropsLive[i]->genLive[l] , allBlockPropsLive[i]->genLive[l+1]);                    
                    }
                    allBlockPropsLive[i]->numGenLive--;
                    break;
                }
            }

            // add variable to kill
            flag = true;
            strcpy(allBlockPropsLive[i]->killLive[allBlockPropsLive[i]->numKillLive++] , temp);    
            
        }
        
    }    

    return;
}

void calculateLiveIn(){
    
    // loop through all blocks
    for(int i=0 ; i<block_count ; i++){
        // Clear current IN
        allBlockPropsLive[i]->numInLive = 0;
        
        // First, add all GEN variables
        for(int j=0 ; j<allBlockPropsLive[i]->numGenLive ; j++){
            strcpy(allBlockPropsLive[i]->inLive[allBlockPropsLive[i]->numInLive] , allBlockPropsLive[i]->genLive[j]);
            allBlockPropsLive[i]->numInLive++;
        }
        
        // Then, add variables from (OUT - KILL)
        for(int j=0 ; j<allBlockPropsLive[i]->numOutLive ; j++){
            // Check if this OUT variable is in KILL
            bool in_kill = false;
            for(int k=0 ; k<allBlockPropsLive[i]->numKillLive ; k++){
                if(isStringSame(allBlockPropsLive[i]->outLive[j] , allBlockPropsLive[i]->killLive[k])){
                    in_kill = true;
                    break;
                }
            }
            
            if(in_kill) continue; // Skip if in KILL
            
            // Check if already in IN (from GEN)
            bool already_in = false;
            for(int k=0 ; k<allBlockPropsLive[i]->numInLive ; k++){
                if(isStringSame(allBlockPropsLive[i]->outLive[j] , allBlockPropsLive[i]->inLive[k])){
                    already_in = true;
                    break;
                }
            }
            
            if(!already_in){
                strcpy(allBlockPropsLive[i]->inLive[allBlockPropsLive[i]->numInLive] , allBlockPropsLive[i]->outLive[j]);
                allBlockPropsLive[i]->numInLive++;
            }
        }
    }

    return;
}

void calculateLiveOut(){    
    
    int index;

    // loop through all blocks
    for(int i=0 ; i<block_count ; i++){
        // Clear current OUT
        allBlockPropsLive[i]->numOutLive = 0;
        
        if(allBlocks[i]->numberOfAddressesInBlock > 0){
            int lastIdx = allBlocks[i]->numberOfAddressesInBlock - 1;
            address* lastInstr = allBlocks[i]->list[lastIdx];
            
            if(lastInstr->type == ADDR_IF_T_GOTO || lastInstr->type == ADDR_IF_F_GOTO){
                char* cond = (lastInstr->type == ADDR_IF_T_GOTO) ? 
                            lastInstr->if_true.condition : lastInstr->if_false.condition;
                
                if(isVariable(cond)){
                    strcpy(allBlockPropsLive[i]->outLive[allBlockPropsLive[i]->numOutLive], cond);
                    allBlockPropsLive[i]->numOutLive++;
                }
            }
        }
        
        // For each successor
        for(int j=0 ; j<allBlocks[i]->numCFGOut ; j++){            
            index = findIndexOfBlock(allBlocks[i]->cfg_out[j]); 

            // Add all variables from successor's IN to this block's OUT (union)
            for(int k=0 ; k<allBlockPropsLive[index]->numInLive ; k++){
                // Check if already in OUT
                bool found = false;
                for(int l=0 ; l<allBlockPropsLive[i]->numOutLive ; l++){
                    if(isStringSame(allBlockPropsLive[index]->inLive[k] , allBlockPropsLive[i]->outLive[l])){
                        found = true;
                        break;
                    }
                }
                
                // Add if not found
                if(!found){
                    strcpy(allBlockPropsLive[i]->outLive[allBlockPropsLive[i]->numOutLive] , allBlockPropsLive[index]->inLive[k]);
                    allBlockPropsLive[i]->numOutLive++;
                }
            }
        }
    }

    return;
}

void startReachingDefiniionOptPhase(){
    int iteration = 0;
    int maxIterations = 5;  // Safety limit
    
    do {
        flag = false;  // Reset flag at start of each iteration
        iteration++;
        
        // Apply all three optimizations in sequence
        checkForConstantPropagation();
        checkForConstantFolding();
        checkForCopyPropagation();
        
        
    } while (flag && iteration < maxIterations);

    return;
}

void startDCE(){
    char* temp;
    char** used_vars;
    int num_used;
    bool isExist = false;
    bool usedInBlock = false;

    // reset mark array
    for(int i=0 ; i<MAX ; i++) mark[i] = NULL;
    markCount = 0;

    // loop through all the blocks
    for(int i=0 ; i<block_count ; i++){
        // loop through all the instructions of this block
        for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            if(!isKindOfAssign(allBlocks[i]->list[j])) continue;
            
            // CRITICAL: Never eliminate array writes - they have side effects!
            if(allBlocks[i]->list[j]->type == ADDR_ARRAY_WRITE) continue;
            if(allBlocks[i]->list[j]->type == ADDR_ARRAY_READ) continue; // Array reads can have side effects via indexing


            temp = getLeftSideOfKindOfAssign(allBlocks[i]->list[j]);

            isExist = false;
            
            // Check if variable is live-OUT
            for(int k=0 ; k<allBlockPropsLive[i]->numOutLive ; k++){
                if(isStringSame(temp , allBlockPropsLive[i]->outLive[k])){
                    isExist = true;
                    break;
                }
            }
            
            // Also check if variable is live-IN (used by control flow in this block)
            if(!isExist){
                for(int k=0 ; k<allBlockPropsLive[i]->numInLive ; k++){
                    if(isStringSame(temp , allBlockPropsLive[i]->inLive[k])){
                        isExist = true;
                        break;
                    }
                }
            }
            
            // CRITICAL: Check if this variable is used by any later instruction in same block
            // This handles intra-block dependencies (e.g., T15=a followed by arr[x]=T15)
            if(!isExist){
                usedInBlock = false;
                for(int k=j+1 ; k<allBlocks[i]->numberOfAddressesInBlock ; k++){
                    num_used = 0;
                    used_vars = getAllVariablesUsed(&num_used , allBlocks[i]->list[k]);
                    for(int l=0 ; l<num_used ; l++){
                        if(isStringSame(temp , used_vars[l])){
                            usedInBlock = true;
                            break;
                        }
                    }
                    if(usedInBlock) break;
                }
                if(usedInBlock) isExist = true;
            }

            if(isExist) continue; // skip if the variable is live


            // mark that instruction to be removed
            mark[markCount++] = allBlocks[i]->list[j];

        }
    }
}

void removeMarkedInstructions(){
    if(markCount <= 0) return;

    // loop through all blocks
    for(int i=0 ; i<block_count ; i++){
        // loop through all the instrutctions
        for(int j=0 ; j<allBlocks[i]->numberOfAddressesInBlock ; j++){
            // loop through all instructions in mark
            for(int k=0 ; k<markCount ; k++){
                if(allBlocks[i]->list[j] == mark[k]){
                    // Double-check: never remove array writes
                    if(mark[k]->type == ADDR_ARRAY_WRITE || mark[k]->type == ADDR_ARRAY_READ){
                        continue;
                    }
                    // remove the instruction from block
                    for(int l=j ; l<allBlocks[i]->numberOfAddressesInBlock ; l++){
                        allBlocks[i]->list[l] = allBlocks[i]->list[l+1];
                    }
                    allBlocks[i]->numberOfAddressesInBlock--;
                    allBlocks[i]->list[allBlocks[i]->numberOfAddressesInBlock] = NULL;
                    j--; // Adjust index since we removed an element
                    break;
                }
            }
        }
    }

    // loop through all instructions
    for(int i=0 ; i<addr_count ; i++){
        // loop through all instructions in mark
        for(int j=0 ; j<markCount ; j++){
            if(allAddress[i] == mark[j]){
                // Double-check: never remove array writes
                if(mark[j]->type == ADDR_ARRAY_WRITE || mark[j]->type == ADDR_ARRAY_READ){
                    continue;
                }
                // remove the instruction from the array
                for(int k=i ; k<addr_count ; k++){
                    allAddress[k] = allAddress[k+1];
                }
                addr_count--;
                allAddress[addr_count] = NULL;
                i--; // Adjust index since we removed an element
                break;
            }
        }
    }
}

void startLiveVaraiblePhase(){    
    int max_iteration = 5;
    int iteration_count = 0;

    attachBlockLivePropWithAllBlocks();    

    again : 
    iteration_count = 0;

    do{
        flag = false;

        calculateLiveGenKill();
        calculateLiveIn();
        calculateLiveOut();        

        iteration_count++;
    } while(flag && iteration_count < max_iteration);


    
    startDCE();


    if(markCount > 0){
        removeMarkedInstructions();
        goto again;
    }

    return;

    
}

void startOptimization(){    
    // Reset ALL optimization state for fresh compilation
    current = 0;
    flag = false;
    block_count = 0;
    block_prop_count = 0;
    
    // Clear all block arrays
    for(int i=0 ; i<MAX ; i++){
        allBlocks[i] = NULL;
        allBlockProps[i] = NULL;
    }
    
    // generate all blocks
    generateAllBlocks();

    // reset the current
    current = 0;

    // generate cfg 
    formCFG();
    
    // print cfg only (without gen/kill/in/out)
    printCFGOnlyToFile();

    // generate GEN/KILL/IN/OUT
    getPropertiesOfAllBlocks();

    // start reaching definition optimization phase
    startReachingDefiniionOptPhase();

    // start live variables analysis
    startLiveVaraiblePhase();

    // print blocks AFTER optimization
    printBlocksToFile();

    return;
}