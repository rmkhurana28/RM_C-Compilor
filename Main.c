// #include "database.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>


// int main(void) {
//     char file_name[MAX_NAME];
//     strcpy(file_name, "tester/test.c");

//     // generate all tokens
//     generateTokens(file_name);

//     // print all tokens
//     printAllTokens();
//     printf("Token generation and printing successful...\n\n");

//     // generate all AST
//     generateAllASTNodes();

//     // print all AST
//     printAllASTNodes();
//     printf("AST generation and printing successful...\n\n");

//     // do semantic check
//     doSemanticCheck();

//     printf("Type checking COMPLETED\n\n");

//     printSymbolTable();



//     exit(0);
// }


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
    void startICG();

    exit(0);
}
