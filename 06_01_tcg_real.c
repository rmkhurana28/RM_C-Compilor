#include "database.h"

// Structure to map variables to stack offsets
typedef struct {
    char var_name[MAX_NAME];
    int stack_offset;       // Offset from %rbp (negative for local vars)
    bool is_array;
    int array_size;         // Number of elements for arrays
} VarStackMap;

// Array to store real assembly code
AsmInstruction* realAsmCode[MAX];
int real_asm_count = 0;

// Variable to stack offset mapping
VarStackMap varStackMap[MAX];
int var_map_count = 0;
int current_stack_offset = -8; // Start at -8(%rbp)

// Helper function to add real assembly instruction
void addRealAsmInstruction(const char* instr) {
    if (real_asm_count >= MAX) {
        printf("Real assembly buffer is full!\n");
        return;
    }
    
    realAsmCode[real_asm_count] = (AsmInstruction*)malloc(sizeof(AsmInstruction));
    if (realAsmCode[real_asm_count] == NULL) {
        printf("Memory allocation failed for real assembly instruction\n");
        return;
    }
    
    strncpy(realAsmCode[real_asm_count]->instruction, instr, 255);
    realAsmCode[real_asm_count]->instruction[255] = '\0';
    real_asm_count++;
}

// Check if operand is a number (immediate value)
bool isRealNumber(const char* str) {
    if (str == NULL || *str == '\0') return false;
    
    // Check for "true" or "false"
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

// Check if operand is a temp variable (T0, T1, etc.)
bool isRealTempVar(const char* str) {
    if (str == NULL || str[0] != 'T') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Check if operand is a label (L0, L1, etc.)
bool isRealLabel(const char* str) {
    if (str == NULL || str[0] != 'L') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Get stack offset for a variable or temp
int getStackOffset(const char* var_name) {
    // Check if already mapped
    for (int i = 0; i < var_map_count; i++) {
        if (strcmp(varStackMap[i].var_name, var_name) == 0) {
            return varStackMap[i].stack_offset;
        }
    }
    
    // Not found, create new mapping
    if (var_map_count >= MAX) {
        printf("Variable stack map is full!\n");
        return -8; // default fallback
    }
    
    VarStackMap* map = &varStackMap[var_map_count];
    strncpy(map->var_name, var_name, MAX_NAME - 1);
    map->var_name[MAX_NAME - 1] = '\0';
    map->is_array = false;
    map->array_size = 0;
    
    // Check if it's an array in symbol table
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbolTable[i]->var_name, var_name) == 0 && symbolTable[i]->isArray) {
            map->is_array = true;
            int arr_size = atoi(symbolTable[i]->arraySize);
            map->array_size = arr_size;
            // Allocate space for entire array (8 bytes per element)
            map->stack_offset = current_stack_offset;
            current_stack_offset -= (arr_size * 8);
            var_map_count++;
            return map->stack_offset;
        }
    }
    
    // Regular variable - allocate 8 bytes
    map->stack_offset = current_stack_offset;
    current_stack_offset -= 8;
    var_map_count++;
    
    return map->stack_offset;
}

// Get operand as string (with proper addressing)
void getRealOperand(const char* operand, char* output) {
    if (isRealNumber(operand)) {
        sprintf(output, "$%s", operand);
    } else if (strcmp(operand, "true") == 0) {
        sprintf(output, "$1");
    } else if (strcmp(operand, "false") == 0) {
        sprintf(output, "$0");
    } else if (isRealLabel(operand)) {
        sprintf(output, "%s", operand);
    } else {
        // Variable or temp - use stack offset
        int offset = getStackOffset(operand);
        sprintf(output, "%d(%%rbp)", offset);
    }
}

// Build variable stack map from symbol table
void buildVariableStackMap() {
    var_map_count = 0;
    current_stack_offset = -8;
    
    // First pass: allocate all user variables from symbol table
    for (int i = 0; i < symbol_count; i++) {
        symbol* sym = symbolTable[i];
        
        // Check if already mapped
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

// Generate x86-64 prologue
void generateRealPrologue() {
    addRealAsmInstruction("    .section .text");
    addRealAsmInstruction("    .globl main");
    addRealAsmInstruction("    .type main, @function");
    addRealAsmInstruction("");
    addRealAsmInstruction("main:");
    addRealAsmInstruction("    pushq   %rbp");
    addRealAsmInstruction("    movq    %rsp, %rbp");
    
    // Calculate total stack space needed
    int total_stack = (-current_stack_offset) + 16; // Add padding
    total_stack = ((total_stack + 15) / 16) * 16;   // Align to 16 bytes
    
    char instr[256];
    sprintf(instr, "    subq    $%d, %%rsp    # Allocate stack space", total_stack);
    addRealAsmInstruction(instr);
    addRealAsmInstruction("");
}

// Generate x86-64 epilogue
void generateRealEpilogue() {
    addRealAsmInstruction("");
    addRealAsmInstruction("    # Function epilogue");
    addRealAsmInstruction("    movq    $0, %rax");
    addRealAsmInstruction("    leave");
    addRealAsmInstruction("    ret");
}

// Generate assembly for ADDR_ASSIGN: result = arg1
void generateRealAssign(address* addr) {
    char instr[256];
    char src[128], dst[128];
    
    getRealOperand(addr->assign.arg1, src);
    getRealOperand(addr->assign.result, dst);
    
    // Load source to %rax
    sprintf(instr, "    movq    %s, %%rax    # %s = %s", 
            src, addr->assign.result, addr->assign.arg1);
    addRealAsmInstruction(instr);
    
    // Store to destination
    sprintf(instr, "    movq    %%rax, %s", dst);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_BINOP: result = arg1 op arg2
void generateRealBinOp(address* addr) {
    char instr[256];
    char arg1[128], arg2[128], result[128];
    
    getRealOperand(addr->binop.arg1, arg1);
    getRealOperand(addr->binop.arg2, arg2);
    getRealOperand(addr->binop.result, result);
    
    // Comment
    sprintf(instr, "    # %s = %s %s %s", 
            addr->binop.result, addr->binop.arg1, addr->binop.op, addr->binop.arg2);
    addRealAsmInstruction(instr);
    
    // Load first operand to %rax
    sprintf(instr, "    movq    %s, %%rax", arg1);
    addRealAsmInstruction(instr);
    
    // Perform operation
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
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_UNOP: result = op arg1
void generateRealUnOp(address* addr) {
    char instr[256];
    char arg1[128], result[128];
    
    getRealOperand(addr->unop.arg1, arg1);
    getRealOperand(addr->unop.result, result);
    
    sprintf(instr, "    # %s = %s %s", 
            addr->unop.result, addr->unop.op, addr->unop.arg1);
    addRealAsmInstruction(instr);
    
    // Load operand
    sprintf(instr, "    movq    %s, %%rax", arg1);
    addRealAsmInstruction(instr);
    
    // Apply unary operation
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
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_GOTO: goto label
void generateRealGoto(address* addr) {
    char instr[256];
    sprintf(instr, "    jmp     %s", addr->goto_stmt.target);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_IF_F_GOTO: ifFalse condition goto label
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

// Generate assembly for ADDR_IF_T_GOTO: ifTrue condition goto label
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

// Generate assembly for ADDR_LABEL: label:
void generateRealLabel(address* addr) {
    char instr[256];
    sprintf(instr, "%s:", addr->label.labelNumber);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_ARRAY_READ: result = array[index]
void generateRealArrayRead(address* addr) {
    char instr[256];
    char index[128], result[128];
    
    getRealOperand(addr->array_read.index, index);
    getRealOperand(addr->array_read.result, result);
    
    sprintf(instr, "    # %s = %s[%s]", 
            addr->array_read.result, addr->array_read.array, addr->array_read.index);
    addRealAsmInstruction(instr);
    
    // Load index to %rbx
    sprintf(instr, "    movq    %s, %%rbx", index);
    addRealAsmInstruction(instr);
    
    // Scale by 8 (size of each element)
    sprintf(instr, "    imulq   $8, %%rbx");
    addRealAsmInstruction(instr);
    
    // Get array base address
    int array_offset = getStackOffset(addr->array_read.array);
    sprintf(instr, "    leaq    %d(%%rbp), %%rax", array_offset);
    addRealAsmInstruction(instr);
    
    // Load array element
    sprintf(instr, "    movq    (%%rax, %%rbx), %%rax");
    addRealAsmInstruction(instr);
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", result);
    addRealAsmInstruction(instr);
}

// Generate assembly for ADDR_ARRAY_WRITE: array[index] = value
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

// Main function to generate real target code from 3-address code
void generateRealTargetCode() {
    // Reset assembly code array
    real_asm_count = 0;
    
    // Build variable to stack offset mapping
    buildVariableStackMap();
    
    // PASS 1: Pre-allocate stack space for all temp variables
    // Process all instructions to discover all temps and allocate their stack space
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
    
    // PASS 2: Generate actual assembly code
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
    
    // Generate epilogue
    generateRealEpilogue();
}
