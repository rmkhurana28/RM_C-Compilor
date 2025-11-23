#include "database.h"

// storage for generated assembly code
AsmInstruction* asmCode[MAX];
int asm_count = 0;

// add assembly instruction to output array
void addAsmInstruction(const char* instr) {
    if (asm_count >= MAX) {
        printf("06 || Target Code Generation error [06.02] -> Assembly buffer is full - maximum instruction limit reached\n");
        exit(6);
    }
    
    asmCode[asm_count] = (AsmInstruction*)malloc(sizeof(AsmInstruction));
    if (asmCode[asm_count] == NULL) {
        printf("06 || Target Code Generation error [06.01] -> Memory allocation failed for assembly instruction\n");
        exit(6);
    }
    
    strncpy(asmCode[asm_count]->instruction, instr, 255);
    asmCode[asm_count]->instruction[255] = '\0';
    asm_count++;
}

// check if string is a numeric value (immediate operand)
bool isNumber(const char* str) {
    if (str == NULL || *str == '\0') return false;
    
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
bool isTempVar(const char* str) {
    if (str == NULL || str[0] != 'T') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// check if operand is a label (L0, L1, etc.)
bool isLabel(const char* str) {
    if (str == NULL || str[0] != 'L') return false;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// map operand to x86-64 location (register/memory/immediate)
void getOperandLocation(const char* operand, char* location) {
    if (isNumber(operand)) {
        sprintf(location, "$%s", operand); // immediate value
    } else if (isTempVar(operand)) {
        sprintf(location, "%%%s", operand); // temp variables to registers
    } else {
        sprintf(location, "%s(%%rbp)", operand); // user variables on stack
    }
}

// convert 3-address code operator to x86-64 instruction mnemonic
const char* getAsmOp(const char* op) {
    if (strcmp(op, "+") == 0) return "addq";
    if (strcmp(op, "-") == 0) return "subq";
    if (strcmp(op, "*") == 0) return "imulq";
    if (strcmp(op, "/") == 0) return "idivq";
    if (strcmp(op, "<") == 0) return "cmpq_lt";
    if (strcmp(op, ">") == 0) return "cmpq_gt";
    if (strcmp(op, "<=") == 0) return "cmpq_le";
    if (strcmp(op, ">=") == 0) return "cmpq_ge";
    if (strcmp(op, "==") == 0) return "cmpq_eq";
    if (strcmp(op, "!=") == 0) return "cmpq_ne";
    if (strcmp(op, "&&") == 0) return "andq";
    if (strcmp(op, "||") == 0) return "orq";
    return "movq";
}

// generate x86-64 function prologue (setup stack frame)
void generatePrologue() {
    addAsmInstruction(".section .text");
    addAsmInstruction(".globl main");
    addAsmInstruction("");
    addAsmInstruction("main:");
    addAsmInstruction("    pushq   %rbp");
    addAsmInstruction("    movq    %rsp, %rbp");
    addAsmInstruction("    subq    $512, %rsp    # Allocate stack space for locals");
    addAsmInstruction("");
}

// generate x86-64 function epilogue (cleanup and return)
void generateEpilogue() {
    addAsmInstruction("");
    addAsmInstruction("    # Function epilogue");
    addAsmInstruction("    movq    $0, %rax      # Return 0");
    addAsmInstruction("    movq    %rbp, %rsp");
    addAsmInstruction("    popq    %rbp");
    addAsmInstruction("    ret");
}

// generate assembly for simple assignment: result = arg1
void generateAssign(address* addr) {
    char instr[256];
    char src[128], dst[128];
    
    if (isNumber(addr->assign.arg1)) {
        // Immediate to register/memory
        sprintf(instr, "    movq    $%s, %s    # %s = %s", 
                addr->assign.arg1, 
                addr->assign.result,
                addr->assign.result, 
                addr->assign.arg1);
    } else {
        // Variable/temp to variable/temp
        sprintf(instr, "    movq    %s, %%rax", addr->assign.arg1);
        addAsmInstruction(instr);
        sprintf(instr, "    movq    %%rax, %s    # %s = %s", 
                addr->assign.result,
                addr->assign.result, 
                addr->assign.arg1);
    }
    addAsmInstruction(instr);
}

// generate assembly for binary operation: result = arg1 op arg2
void generateBinOp(address* addr) {
    char instr[256];
    
    // Comment showing the operation
    sprintf(instr, "    # %s = %s %s %s", 
            addr->binop.result, 
            addr->binop.arg1, 
            addr->binop.op, 
            addr->binop.arg2);
    addAsmInstruction(instr);
    
    // Move first operand to %rax
    if (isNumber(addr->binop.arg1)) {
        sprintf(instr, "    movq    $%s, %%rax", addr->binop.arg1);
    } else {
        sprintf(instr, "    movq    %s, %%rax", addr->binop.arg1);
    }
    addAsmInstruction(instr);
    
    // Perform operation
    if (strcmp(addr->binop.op, "+") == 0) {
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    addq    $%s, %%rax", addr->binop.arg2);
        } else {
            sprintf(instr, "    addq    %s, %%rax", addr->binop.arg2);
        }
    } else if (strcmp(addr->binop.op, "-") == 0) {
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    subq    $%s, %%rax", addr->binop.arg2);
        } else {
            sprintf(instr, "    subq    %s, %%rax", addr->binop.arg2);
        }
    } else if (strcmp(addr->binop.op, "*") == 0) {
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    movq    $%s, %%rbx", addr->binop.arg2);
            addAsmInstruction(instr);
            sprintf(instr, "    imulq   %%rbx, %%rax");
        } else {
            sprintf(instr, "    imulq   %s, %%rax", addr->binop.arg2);
        }
    } else if (strcmp(addr->binop.op, "/") == 0) {
        // Division requires special handling
        sprintf(instr, "    cqto                  # Sign extend rax to rdx:rax");
        addAsmInstruction(instr);
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    movq    $%s, %%rbx", addr->binop.arg2);
            addAsmInstruction(instr);
            sprintf(instr, "    idivq   %%rbx");
        } else {
            sprintf(instr, "    idivq   %s", addr->binop.arg2);
        }
    } else if (strcmp(addr->binop.op, "<") == 0 || strcmp(addr->binop.op, ">") == 0 || 
               strcmp(addr->binop.op, "<=") == 0 || strcmp(addr->binop.op, ">=") == 0 ||
               strcmp(addr->binop.op, "==") == 0 || strcmp(addr->binop.op, "!=") == 0) {
        // Comparison operations
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    cmpq    $%s, %%rax", addr->binop.arg2);
        } else {
            sprintf(instr, "    cmpq    %s, %%rax", addr->binop.arg2);
        }
        addAsmInstruction(instr);
        
        // Set result based on comparison
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
        addAsmInstruction(instr);
        sprintf(instr, "    movzbq  %%al, %%rax   # Zero extend to 64-bit");
    } else if (strcmp(addr->binop.op, "&&") == 0) {
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    andq    $%s, %%rax", addr->binop.arg2);
        } else {
            sprintf(instr, "    andq    %s, %%rax", addr->binop.arg2);
        }
    } else if (strcmp(addr->binop.op, "||") == 0) {
        if (isNumber(addr->binop.arg2)) {
            sprintf(instr, "    orq     $%s, %%rax", addr->binop.arg2);
        } else {
            sprintf(instr, "    orq     %s, %%rax", addr->binop.arg2);
        }
    }
    addAsmInstruction(instr);
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", addr->binop.result);
    addAsmInstruction(instr);
}

// generate assembly for unary operation: result = op arg1
void generateUnOp(address* addr) {
    char instr[256];
    
    sprintf(instr, "    # %s = %s %s", 
            addr->unop.result, 
            addr->unop.op, 
            addr->unop.arg1);
    addAsmInstruction(instr);
    
    // Load operand
    if (isNumber(addr->unop.arg1)) {
        sprintf(instr, "    movq    $%s, %%rax", addr->unop.arg1);
    } else {
        sprintf(instr, "    movq    %s, %%rax", addr->unop.arg1);
    }
    addAsmInstruction(instr);
    
    // Apply unary operation
    if (strcmp(addr->unop.op, "-") == 0) {
        sprintf(instr, "    negq    %%rax");
    } else if (strcmp(addr->unop.op, "!") == 0) {
        sprintf(instr, "    xorq    $1, %%rax     # Logical NOT");
    } else if (strcmp(addr->unop.op, "++") == 0) {
        sprintf(instr, "    incq    %%rax");
    } else if (strcmp(addr->unop.op, "--") == 0) {
        sprintf(instr, "    decq    %%rax");
    }
    addAsmInstruction(instr);
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", addr->unop.result);
    addAsmInstruction(instr);
}

// generate assembly for unconditional jump: goto label
void generateGoto(address* addr) {
    char instr[256];
    sprintf(instr, "    jmp     %s", addr->goto_stmt.target);
    addAsmInstruction(instr);
}

// generate assembly for conditional jump if false: ifFalse condition goto label
void generateIfFalseGoto(address* addr) {
    char instr[256];
    
    sprintf(instr, "    # ifFalse %s goto %s", 
            addr->if_false.condition, 
            addr->if_false.target);
    addAsmInstruction(instr);
    
    // Load condition
    if (isNumber(addr->if_false.condition)) {
        sprintf(instr, "    movq    $%s, %%rax", addr->if_false.condition);
    } else {
        sprintf(instr, "    movq    %s, %%rax", addr->if_false.condition);
    }
    addAsmInstruction(instr);
    
    // Test if zero (false)
    sprintf(instr, "    testq   %%rax, %%rax");
    addAsmInstruction(instr);
    sprintf(instr, "    je      %s", addr->if_false.target);
    addAsmInstruction(instr);
}

// generate assembly for conditional jump if true: ifTrue condition goto label
void generateIfTrueGoto(address* addr) {
    char instr[256];
    
    sprintf(instr, "    # ifTrue %s goto %s", 
            addr->if_true.condition, 
            addr->if_true.target);
    addAsmInstruction(instr);
    
    // Load condition
    if (isNumber(addr->if_true.condition)) {
        sprintf(instr, "    movq    $%s, %%rax", addr->if_true.condition);
    } else {
        sprintf(instr, "    movq    %s, %%rax", addr->if_true.condition);
    }
    addAsmInstruction(instr);
    
    // Test if non-zero (true)
    sprintf(instr, "    testq   %%rax, %%rax");
    addAsmInstruction(instr);
    sprintf(instr, "    jne     %s", addr->if_true.target);
    addAsmInstruction(instr);
}

// generate assembly for label definition: label:
void generateLabel(address* addr) {
    char instr[256];
    sprintf(instr, "%s:", addr->label.labelNumber);
    addAsmInstruction(instr);
}

// generate assembly for array element read: result = array[index]
void generateArrayRead(address* addr) {
    char instr[256];
    
    sprintf(instr, "    # %s = %s[%s]", 
            addr->array_read.result, 
            addr->array_read.array, 
            addr->array_read.index);
    addAsmInstruction(instr);
    
    // Load index
    if (isNumber(addr->array_read.index)) {
        sprintf(instr, "    movq    $%s, %%rbx", addr->array_read.index);
    } else {
        sprintf(instr, "    movq    %s, %%rbx", addr->array_read.index);
    }
    addAsmInstruction(instr);
    
    // Calculate offset (index * 8 for 8-byte elements)
    sprintf(instr, "    imulq   $8, %%rbx");
    addAsmInstruction(instr);
    
    // Load array base address and read element
    sprintf(instr, "    leaq    %s, %%rax", addr->array_read.array);
    addAsmInstruction(instr);
    sprintf(instr, "    movq    (%%rax, %%rbx), %%rax");
    addAsmInstruction(instr);
    
    // Store result
    sprintf(instr, "    movq    %%rax, %s", addr->array_read.result);
    addAsmInstruction(instr);
}

// generate assembly for array element write: array[index] = value
void generateArrayWrite(address* addr) {
    char instr[256];
    
    sprintf(instr, "    # %s[%s] = %s", 
            addr->array_write.array, 
            addr->array_write.index, 
            addr->array_write.value);
    addAsmInstruction(instr);
    
    // Load value to write
    if (isNumber(addr->array_write.value)) {
        sprintf(instr, "    movq    $%s, %%rax", addr->array_write.value);
    } else {
        sprintf(instr, "    movq    %s, %%rax", addr->array_write.value);
    }
    addAsmInstruction(instr);
    
    // Load index
    if (isNumber(addr->array_write.index)) {
        sprintf(instr, "    movq    $%s, %%rbx", addr->array_write.index);
    } else {
        sprintf(instr, "    movq    %s, %%rbx", addr->array_write.index);
    }
    addAsmInstruction(instr);
    
    // Calculate offset (index * 8 for 8-byte elements)
    sprintf(instr, "    imulq   $8, %%rbx");
    addAsmInstruction(instr);
    
    // Load array base address and write element
    sprintf(instr, "    leaq    %s, %%rcx", addr->array_write.array);
    addAsmInstruction(instr);
    sprintf(instr, "    movq    %%rax, (%%rcx, %%rbx)");
    addAsmInstruction(instr);
}

// main function to convert 3-address code to x86-64 assembly
void generateTargetCode() {
    // reset assembly code counter
    asm_count = 0;
    
    // add function prologue
    generatePrologue();
    
    // convert each 3-address instruction to assembly
    for (int i = 0; i < addr_count; i++) {
        address* addr = allAddress[i];
        
        switch (addr->type) {
            case ADDR_ASSIGN:
                generateAssign(addr);
                break;
                
            case ADDR_BINOP:
                generateBinOp(addr);
                break;
                
            case ADDR_UNOP:
                generateUnOp(addr);
                break;
                
            case ADDR_GOTO:
                generateGoto(addr);
                break;
                
            case ADDR_IF_F_GOTO:
                generateIfFalseGoto(addr);
                break;
                
            case ADDR_IF_T_GOTO:
                generateIfTrueGoto(addr);
                break;
                
            case ADDR_LABEL:
                generateLabel(addr);
                break;
                
            case ADDR_ARRAY_READ:
                generateArrayRead(addr);
                break;
                
            case ADDR_ARRAY_WRITE:
                generateArrayWrite(addr);
                break;
                
            default:
                break;
        }
    }
    
    // add function epilogue
    generateEpilogue();
}