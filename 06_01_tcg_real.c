/**
 * 06_01_tcg_real.c - Real x86-64 Target Code Generator
 * 
 * This module generates actual x86-64 assembly code from the optimized
 * intermediate representation. The output can be assembled and linked
 * to create an executable program.
 * 
 * Features:
 * - Register allocation and management
 * - Stack frame setup and management
 * - Variable-to-stack-offset mapping
 * - x86-64 instruction generation
 * - Function prologue and epilogue
 * - Memory addressing modes
 * 
 * Author: Ridham Khurana
 */

// suppress format overflow warnings - buffers are sized correctly
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-overflow"

#include "database.h"

// structure to map variables to stack offsets
typedef struct {
    char var_name[MAX_NAME];
    int stack_offset;       // offset from %rbp (negative for local vars)
    bool is_array;
    int array_size;         // number of elements for arrays
} VarStackMap;

// storage for generated real assembly code
AsmInstruction* realAsmCode[MAX];
int real_asm_count = 0;

// variable to stack offset mapping table
VarStackMap varStackMap[MAX];
int var_map_count = 0;
int current_stack_offset = -8; // start at -8(%rbp)

// add real assembly instruction to output array
void addRealAsmInstruction(const char* instr) {
    if (real_asm_count >= MAX) {
        printf("06_01 || Real Target Code Generation error [06_01.01] -> Real assembly buffer is full - maximum instruction limit reached\n");
        exit(6);
    }
    
    realAsmCode[real_asm_count] = (AsmInstruction*)malloc(sizeof(AsmInstruction));
    if (realAsmCode[real_asm_count] == NULL) {
        printf("06_01 || Real Target Code Generation error [06_01.02] -> Memory allocation failed for real assembly instruction\n");
        exit(6);
    }
    
    strncpy(realAsmCode[real_asm_count]->instruction, instr, 255);
    realAsmCode[real_asm_count]->instruction[255] = '\0';
    real_asm_count++;
}

// check if string is a numeric value (immediate operand)
bool isRealNumber(const char* str) {
    if (str == NULL || *str == '\0') return false;
    
    // check for boolean literals
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0) return false;
    
    int i = 0;
    if (str[0] == '-' || str[0] == '+') i++;
    
    bool hasDigit = false;
    bool hasDot = false;
    
    while (str[i] != '\0') {
        if (isdigit(str[i])) {
            hasDigit = true;
        } else if (str[i] == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else {
            return false;
        }
        i++;
    }
    
    return hasDigit;
}

// check if operand is a temporary variable (T0, T1, etc.)
bool isRealTempVar(const char* str) {
    if (str == NULL || str[0] != 'T') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// check if operand is a label (L0, L1, etc.)
bool isRealLabel(const char* str) {
    if (str == NULL || str[0] != 'L') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Get or allocate stack offset for a variable
// Variables are stored on the stack with negative offsets from %rbp
// Arrays get space for all elements (8 bytes each)
int getStackOffset(const char* var_name) {
    // check if already mapped
    for (int i = 0; i < var_map_count; i++) {
        if (strcmp(varStackMap[i].var_name, var_name) == 0) {
            return varStackMap[i].stack_offset;
        }
    }
    
    // not found, create new mapping
    if (var_map_count >= MAX) {
        printf("06_01 || Real Target Code Generation error [06_01.03] -> Variable stack map is full - maximum variable limit reached\n");
        exit(6);
    }
    
    VarStackMap* map = &varStackMap[var_map_count];
    strncpy(map->var_name, var_name, MAX_NAME - 1);
    map->var_name[MAX_NAME - 1] = '\0';
    map->is_array = false;
    map->array_size = 0;
    
    // check if it's an array in symbol table
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbolTable[i]->var_name, var_name) == 0 && symbolTable[i]->isArray) {
            map->is_array = true;
            int arr_size = atoi(symbolTable[i]->arraySize);
            map->array_size = arr_size;
            // allocate space for entire array (8 bytes per element)
            map->stack_offset = current_stack_offset;
            current_stack_offset -= (arr_size * 8);
            var_map_count++;
            return map->stack_offset;
        }
    }
    
    // regular variable - allocate 8 bytes
    map->stack_offset = current_stack_offset;
    current_stack_offset -= 8;
    var_map_count++;
    
    return map->stack_offset;
}

// convert operand to proper x86-64 addressing format
void getRealOperand(const char* operand, char* output) {
    if (isRealNumber(operand)) {
        sprintf(output, "$%s", operand); // immediate value
    } else if (strcmp(operand, "true") == 0) {
        sprintf(output, "$1"); // boolean true as 1
    } else if (strcmp(operand, "false") == 0) {
        sprintf(output, "$0"); // boolean false as 0
    } else if (isRealLabel(operand)) {
        sprintf(output, "%s", operand); // label reference
    } else {
        // variable or temp - use stack offset
        int offset = getStackOffset(operand);
        sprintf(output, "%d(%%rbp)", offset);
    }
}

// build variable stack map from symbol table
void buildVariableStackMap() {
    var_map_count = 0;
    current_stack_offset = -8;
    
    // first pass: allocate all user variables from symbol table
    for (int i = 0; i < symbol_count; i++) {
        symbol* sym = symbolTable[i];
        
        // check if already mapped
        bool already_mapped = false;
        for (int j = 0; j < var_map_count; j++) {
            if (strcmp(varStackMap[j].var_name, sym->var_name) == 0) {
                already_mapped = true;
                break;
            }
        }
        
        if (!already_mapped) {
            VarStackMap* map = &varStackMap[var_map_count];
            strncpy(map->var_name, sym->var_name, MAX_NAME - 1);
            map->var_name[MAX_NAME - 1] = '\0';
            map->is_array = sym->isArray;
            
            if (sym->isArray) {
                int arr_size = atoi(sym->arraySize);
                map->array_size = arr_size;
                map->stack_offset = current_stack_offset;
                current_stack_offset -= (arr_size * 8);
            } else {
                map->array_size = 0;
                map->stack_offset = current_stack_offset;
                current_stack_offset -= 8;
            }
            
            var_map_count++;
        }
    }
}

// generate x86-64 function prologue with correct stack allocation
void generateRealPrologue() {
    addRealAsmInstruction("    .section .text");
    addRealAsmInstruction("    .globl main");
    addRealAsmInstruction("    .type main, @function");
    addRealAsmInstruction("");
    addRealAsmInstruction("main:");
    addRealAsmInstruction("    pushq   %rbp");
    addRealAsmInstruction("    movq    %rsp, %rbp");
    
    // calculate total stack space needed
    int total_stack = (-current_stack_offset) + 16; // add padding
    total_stack = ((total_stack + 15) / 16) * 16;   // align to 16 bytes
    
    char instr[256];
    sprintf(instr, "    subq    $%d, %%rsp    # Allocate stack space", total_stack);
    addRealAsmInstruction(instr);
    addRealAsmInstruction(""); 
}

// generate x86-64 function epilogue (cleanup and return)
void generateRealEpilogue() {
    addRealAsmInstruction("");
    addRealAsmInstruction("    # Function epilogue");
    addRealAsmInstruction("    movq    $0, %rax");
    addRealAsmInstruction("    leave");
    addRealAsmInstruction("    ret");
}

// generate assembly for simple assignment: result = arg1
void generateRealAssign(address* addr) {
    char instr[256];
    char src[128], dst[128];
    
    getRealOperand(addr->assign.arg1, src);
    getRealOperand(addr->assign.result, dst);
    
    // load source to %rax
    sprintf(instr, "    movq    %s, %%rax    # %s = %s", 
            src, addr->assign.result, addr->assign.arg1);
    addRealAsmInstruction(instr);
    
    // store to destination
    sprintf(instr, "    movq    %%rax, %s", dst);
    addRealAsmInstruction(instr);
}

// generate assembly for binary operation: result = arg1 op arg2
void generateRealBinOp(address* addr) {
    char instr[256];
    char arg1[128], arg2[128], result[128];
    
    getRealOperand(addr->binop.arg1, arg1);
    getRealOperand(addr->binop.arg2, arg2);
    getRealOperand(addr->binop.result, result);
    
    // comment showing the operation
    sprintf(instr, "    # %s = %s %s %s", 
            addr->binop.result, addr->binop.arg1, addr->binop.op, addr->binop.arg2);
    addRealAsmInstruction(instr);
    
    // load first operand to %rax
    sprintf(instr, "    movq    %s, %%rax", arg1);
    addRealAsmInstruction(instr);
    
    // perform operation
    if (strcmp(addr->binop.op, "+") == 0) {
        sprintf(instr, "    addq    %s, %%rax", arg2);
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "-") == 0) {
        sprintf(instr, "    subq    %s, %%rax", arg2);
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "*") == 0) {
        sprintf(instr, "    movq    %s, %%rbx", arg2);
        addRealAsmInstruction(instr);
        sprintf(instr, "    imulq   %%rbx, %%rax");
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "/") == 0) {
        sprintf(instr, "    cqto");
        addRealAsmInstruction(instr);
        sprintf(instr, "    movq    %s, %%rbx", arg2);
        addRealAsmInstruction(instr);
        sprintf(instr, "    idivq   %%rbx");
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "<") == 0 || strcmp(addr->binop.op, ">") == 0 || 
               strcmp(addr->binop.op, "<=") == 0 || strcmp(addr->binop.op, ">=") == 0 ||
               strcmp(addr->binop.op, "==") == 0 || strcmp(addr->binop.op, "!=") == 0) {
        sprintf(instr, "    cmpq    %s, %%rax", arg2);
        addRealAsmInstruction(instr);
        
        if (strcmp(addr->binop.op, "<") == 0) {
            sprintf(instr, "    setl    %%al");
        } else if (strcmp(addr->binop.op, ">") == 0) {
            sprintf(instr, "    setg    %%al");
        } else if (strcmp(addr->binop.op, "<=") == 0) {
            sprintf(instr, "    setle   %%al");
        } else if (strcmp(addr->binop.op, ">=") == 0) {
            sprintf(instr, "    setge   %%al");
        } else if (strcmp(addr->binop.op, "==") == 0) {
            sprintf(instr, "    sete    %%al");
        } else if (strcmp(addr->binop.op, "!=") == 0) {
            sprintf(instr, "    setne   %%al");
        }
        addRealAsmInstruction(instr);
        sprintf(instr, "    movzbq  %%al, %%rax");
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "&&") == 0) {
        sprintf(instr, "    andq    %s, %%rax", arg2);
        addRealAsmInstruction(instr);
    } else if (strcmp(addr->binop.op, "||") == 0) {
        sprintf(instr, "    orq     %s, %%rax", arg2);
        addRealAsmInstruction(instr);
    }
    
    // store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// generate assembly for unary operation: result = op arg1
void generateRealUnOp(address* addr) {
    char instr[256];
    char arg1[128], result[128];
    
    getRealOperand(addr->unop.arg1, arg1);
    getRealOperand(addr->unop.result, result);
    
    sprintf(instr, "    # %s = %s %s", 
            addr->unop.result, addr->unop.op, addr->unop.arg1);
    addRealAsmInstruction(instr);
    
    // load operand
    sprintf(instr, "    movq    %s, %%rax", arg1);
    addRealAsmInstruction(instr);
    
    // apply unary operation
    if (strcmp(addr->unop.op, "-") == 0) {
        sprintf(instr, "    negq    %%rax");
    } else if (strcmp(addr->unop.op, "!") == 0) {
        sprintf(instr, "    xorq    $1, %%rax");
    } else if (strcmp(addr->unop.op, "++") == 0) {
        sprintf(instr, "    incq    %%rax");
    } else if (strcmp(addr->unop.op, "--") == 0) {
        sprintf(instr, "    decq    %%rax");
    }
    addRealAsmInstruction(instr);
    
    // store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// generate assembly for unconditional jump: goto label
void generateRealGoto(address* addr) {
    char instr[256];
    sprintf(instr, "    jmp     %s", addr->goto_stmt.target);
    addRealAsmInstruction(instr);
}

// generate assembly for conditional jump if false: ifFalse condition goto label
void generateRealIfFalseGoto(address* addr) {
    char instr[256];
    char condition[128];
    
    getRealOperand(addr->if_false.condition, condition);
    
    sprintf(instr, "    # ifFalse %s goto %s", 
            addr->if_false.condition, addr->if_false.target);
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    movq    %s, %%rax", condition);
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    testq   %%rax, %%rax");
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    je      %s", addr->if_false.target);
    addRealAsmInstruction(instr);
}

// generate assembly for conditional jump if true: ifTrue condition goto label
void generateRealIfTrueGoto(address* addr) {
    char instr[256];
    char condition[128];
    
    getRealOperand(addr->if_true.condition, condition);
    
    sprintf(instr, "    # ifTrue %s goto %s", 
            addr->if_true.condition, addr->if_true.target);
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    movq    %s, %%rax", condition);
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    testq   %%rax, %%rax");
    addRealAsmInstruction(instr);
    
    sprintf(instr, "    jne     %s", addr->if_true.target);
    addRealAsmInstruction(instr);
}

// generate assembly for label definition: label:
void generateRealLabel(address* addr) {
    char instr[256];
    sprintf(instr, "%s:", addr->label.labelNumber);
    addRealAsmInstruction(instr);
}

// generate assembly for array element read: result = array[index]
void generateRealArrayRead(address* addr) {
    char instr[256];
    char index[128], result[128];
    
    getRealOperand(addr->array_read.index, index);
    getRealOperand(addr->array_read.result, result);
    
    sprintf(instr, "    # %s = %s[%s]", 
            addr->array_read.result, addr->array_read.array, addr->array_read.index);
    addRealAsmInstruction(instr);
    
    // load index to %rbx
    sprintf(instr, "    movq    %s, %%rbx", index);
    addRealAsmInstruction(instr);
    
    // scale by 8 (size of each element)
    sprintf(instr, "    imulq   $8, %%rbx");
    addRealAsmInstruction(instr);
    
    // get array base address
    int array_offset = getStackOffset(addr->array_read.array);
    sprintf(instr, "    leaq    %d(%%rbp), %%rax", array_offset);
    addRealAsmInstruction(instr);
    
    // load array element
    sprintf(instr, "    movq    (%%rax, %%rbx), %%rax");
    addRealAsmInstruction(instr);
    
    // store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// generate assembly for array element write: array[index] = value
void generateRealArrayWrite(address* addr) {
    char instr[256];
    char index[128], value[128];
    
    getRealOperand(addr->array_write.index, index);
    getRealOperand(addr->array_write.value, value);
    
    sprintf(instr, "    # %s[%s] = %s", 
            addr->array_write.array, addr->array_write.index, addr->array_write.value);
    addRealAsmInstruction(instr);
    
    // Load value to %rax
    sprintf(instr, "    movq    %s, %%rax", value);
    addRealAsmInstruction(instr);
    
    // Load index to %rbx
    sprintf(instr, "    movq    %s, %%rbx", index);
    addRealAsmInstruction(instr);
    
    // Scale by 8
    sprintf(instr, "    imulq   $8, %%rbx");
    addRealAsmInstruction(instr);
    
    // Get array base address
    int array_offset = getStackOffset(addr->array_write.array);
    sprintf(instr, "    leaq    %d(%%rbp), %%rcx", array_offset);
    addRealAsmInstruction(instr);
    
    // Store to array
    sprintf(instr, "    movq    %%rax, (%%rcx, %%rbx)");
    addRealAsmInstruction(instr);
}

// main function to convert 3-address code to real x86-64 assembly
void generateRealTargetCode() {
    // reset assembly code counter
    real_asm_count = 0;
    
    // build variable to stack offset mapping
    buildVariableStackMap();
    
    // PASS 1: pre-allocate stack space for all temp variables
    // process all instructions to discover all temps and allocate their stack space
    for (int i = 0; i < addr_count; i++) {
        address* addr = allAddress[i];
        
        switch (addr->type) {
            case ADDR_ASSIGN:
                getStackOffset(addr->assign.result);
                getStackOffset(addr->assign.arg1);
                break;
                
            case ADDR_BINOP:
                getStackOffset(addr->binop.result);
                getStackOffset(addr->binop.arg1);
                getStackOffset(addr->binop.arg2);
                break;
                
            case ADDR_UNOP:
                getStackOffset(addr->unop.result);
                getStackOffset(addr->unop.arg1);
                break;
                
            case ADDR_IF_F_GOTO:
                getStackOffset(addr->if_false.condition);
                break;
                
            case ADDR_IF_T_GOTO:
                getStackOffset(addr->if_true.condition);
                break;
                
            case ADDR_ARRAY_READ:
                getStackOffset(addr->array_read.result);
                getStackOffset(addr->array_read.index);
                break;
                
            case ADDR_ARRAY_WRITE:
                getStackOffset(addr->array_write.index);
                getStackOffset(addr->array_write.value);
                break;
                
            default:
                break;
        }
    }
    
    // NOW generate prologue with correct stack size
    generateRealPrologue();
    
    // PASS 2: generate actual assembly code
    for (int i = 0; i < addr_count; i++) {
        address* addr = allAddress[i];
        
        switch (addr->type) {
            case ADDR_ASSIGN:
                generateRealAssign(addr);
                break;
                
            case ADDR_BINOP:
                generateRealBinOp(addr);
                break;
                
            case ADDR_UNOP:
                generateRealUnOp(addr);
                break;
                
            case ADDR_GOTO:
                generateRealGoto(addr);
                break;
                
            case ADDR_IF_F_GOTO:
                generateRealIfFalseGoto(addr);
                break;
                
            case ADDR_IF_T_GOTO:
                generateRealIfTrueGoto(addr);
                break;
                
            case ADDR_LABEL:
                generateRealLabel(addr);
                break;
                
            case ADDR_ARRAY_READ:
                generateRealArrayRead(addr);
                break;
                
            case ADDR_ARRAY_WRITE:
                generateRealArrayWrite(addr);
                break;
                
            default:
                break;
        }
    }
    
    // generate epilogue
    generateRealEpilogue();
}
