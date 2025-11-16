#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(void) {
    char file_name[MAX_NAME];
    strcpy(file_name, "tester/test.c");

    // generate all tokens
    generateTokens(file_name);

    // print all tokens
    printAllTokens();

    // generate all AST
    generateAllASTNodes();

    // print all AST
    printAllASTNodes();

    // do semantic check
    doSemanticCheck();

    // print symbol table
    printSymbolTable();

    // generate 3-address code
    startICG();

    print3AddressCode();

    // generate target code (x86-64 assembly)
    // generateTargetCode();
    // printTargetCode();

    // generate real target code (fully assemblable x86-64)
    // generateRealTargetCode();
    // printRealTargetCode();

    exit(0);
}
