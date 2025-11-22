

#include "database.h"

int current = 0;
int BlockId = 0;

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

char* getLeftSideOfKindOfAssign(address* addr){
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

    // set the ID of block
    temp->blockID = BlockId++;

    // copy next N instructions into the temp starting from init
    for(int i=0 ; i<N ; i++){     
        temp->list[i] = allAddress[init++];
    }
    temp->numberOfAddressesInBlock = N; // set the total number of instructions in the block

    // temp->out = NULL;
    // temp->in = NULL;
    temp->numCFGIn = 0; // initialize number of cfg in=0
    temp->numCFGOut = 0; // initialize number of cfg out=0

    // add block into the block array
    allBlocks[block_count++] = temp;

    // return newly-generated block
    return temp;
}

address** unionOfAddrAndReturnResult(int n1 , address** addr1 , int n2 , address** addr2){
    // allocate memory for array of address pointores
    address** temp = (address**)malloc(MAX_STATEMENTS * sizeof(address*));
    
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

int findIndexOfBlock(block* currBlock){
    // loop thought all blocks and return the index of current block
    for(int i=0 ; i<block_count ; i++){
        if(currBlock == allBlocks[i]) return i;
    }

    // current block doesn't exists
    return -1;
}

blockProp* generateEmptyBlockProp(){
    // generate empty blockProp and return
    return (blockProp*)malloc(sizeof(blockProp));
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
    // if cfgIN is empty, return
    if(currBlock->numCFGIn == 0) return;

    // helper to store the results
    address** result;

    // number of instructions in result
    int n = 0;

    // loop through all blocks in cfgIN of current block
    for(int i=0 ; i<currBlock->numCFGIn ; i++){
        // store the union of result and OUT of predecesor in the result
        result = unionOfAddrAndReturnResult(n , result , allBlockProps[findIndexOfBlock(currBlock->cfg_in[i])]->numOut , allBlockProps[findIndexOfBlock(currBlock->cfg_in[i])]->out); // passing result and OUT of current block's predecesor
        
        // update the number of instructions in result
        while(result[n] != NULL) n++;        
    }

    // set the number of IN insructions in current blockProp
    currBlockProp->numIn = n;

    // copy all the instructiosn of result into IN of current blockProp
    for(int i=0 ; i<n ; i++){
        currBlockProp->in[i] = result[i];
    }

    return;

}

void calculateOUTForBlockAndStoreTo(block* currBlock , blockProp* currBlockProp){
    // evalute IN - KILL of current block and store in helper
    address** temp = minusOfTwoAddr(currBlockProp->numIn , currBlockProp->in , currBlockProp->numKill , currBlockProp->kill);

    // number of instructions in temp
    int check = 0; // initialize to 0
    while(temp[check] != NULL) check++; // update number of instructions        

    // update temp to be union of GEN and result of previous MINUS
    temp = unionOfAddrAndReturnResult(currBlockProp->numGen , currBlockProp->gen , check , temp);
    check = 0;
    
    // copy temp into OUT and count elements
    while(temp[check] != NULL){
        currBlockProp->out[check] = temp[check];
        check++;
    }

    // save the number of instructions in temp to blockProp numOUT
    currBlockProp->numOut = check;        

    return;
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
        if(isNextLabel(current+numberToAdd)){ // if next address-instruction is label, block ends here , and new block starts form next
                        
            // add current instruction
            numberToAdd++;

            // generate block till label (excluding label) (till just before label)
            generateBlock(init , numberToAdd);

            // increment the current index to next instruction
            current += numberToAdd;
            return;
        }

        if(isNextGoto(current+numberToAdd)){ // if next address-instruction is leader, block ends here , and new block starts from next

            // add current and next(goto) instruction
            numberToAdd += 2; 

            // generate block goto (including goto)
            generateBlock(init , numberToAdd);

            // increment the current index to the instruction next to goto
            current += numberToAdd;
            return;
        }
        
        // if next instruction is not goto/label , add current isntruction and continue the while loop (if instruction exists)
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
    // error in assignment of block-properties
    if(index_of_block != block_prop_count){
        printf("Error\n");
        exit(5);
    }
    
    // generate empty prop object
    blockProp* temp_prop = generateEmptyBlockProp();
    
    // calculate gen
    calculateGENForBlockAndStoreTo(currBlock , temp_prop);

    // calculate kill
    calculateKILLForBlockAndStoreTo(currBlock , temp_prop);

    // calculate in
    calculateINForBlockAndStoreTo(currBlock , temp_prop);

    // calculate out
    calculateOUTForBlockAndStoreTo(currBlock , temp_prop);
    
    // add block with matching index wiht block
    allBlockProps[block_prop_count++] = temp_prop;
}

void generateAllBlocks(){
    while(current < addr_count){
        // loop through all 3-addr codes
        evaluate();
    }
    return;
}

void getPropertiesOfAllBlocks(){
    for(int i=0 ; i<block_count ; i++){
        // loop thought all blocks 
        getPropertyOfBlock(i,allBlocks[i]);
    }
}

void startOptimization(){    
    // generate all blocks
    generateAllBlocks();

    // reset the current
    current = 0;

    // generate cfg 
    formCFG();

    // generate GEN/KILL/IN/OUT
    getPropertiesOfAllBlocks();

    

    return;
}