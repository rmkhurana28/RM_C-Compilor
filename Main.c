/**
 * RMc4 Compiler - Main Entry Point
 * 
 * RMc4 = RM (author: Ridham Khurana) + c4 (Level-4 C Compiler)
 * 
 * A Level-4 C Compiler implementation that performs complete compilation pipeline:
 * - Lexical Analysis (Tokenization)
 * - Syntax Analysis (AST Generation)
 * - Semantic Analysis (Type Checking)
 * - Intermediate Code Generation (Three-Address Code)
 * - Code Optimization (CFG, Basic Blocks, Data Flow Analysis)
 * - Target Code Generation (x86-64 Assembly)
 * 
 * usage: RMc4 <source_file.c> <output_file>
 * output file name must be: compiler_output.txt
 * 
 * Author: Ridham Khurana
 */

#include "database.h"
#include <time.h>

// global variables for instruction count tracking
int instruction_count_before_optimization = 0;
int instruction_count_after_optimization = 0;

// global output filename
char output_filename[MAX_NAME];

int main(int argc, char* argv[]) {
    char file_name[MAX_NAME];
    char output_file_name[MAX_NAME];
    
    // check if correct number of arguments provided
    if (argc != 3) {
        printf("Error: Incorrect usage\n");
        printf("Usage: RMc4 <source_file.c> <output_file>\n");
        printf("Example: RMc4 test.c compiler_output.txt\n");
        exit(1);
    }
    
    // copy source file name from command line argument
    strcpy(file_name, argv[1]);
    
    // copy output file name from command line argument
    strcpy(output_file_name, argv[2]);
    
    // also store in global variable for file output functions
    strcpy(output_filename, argv[2]);
    
    // check if output file name is exactly "compiler_output.txt"
    if (strcmp(output_file_name, "compiler_output.txt") != 0) {
        printf("Error: Output file name must be 'compiler_output.txt'\n");
        printf("Usage: RMc4 <source_file.c> compiler_output.txt\n");
        exit(1);
    }

    // Variables for timing
    clock_t start_time, end_time, phase_start, phase_end;
    double total_time_ms, phase_time_ms;
    
    // Start total compilation timer
    start_time = clock();

    // Print compilation header
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  Compiling Program: %s\n", file_name);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // Initialize output file
    initializeOutputFile();

    // ===== PHASE 1: LEXICAL ANALYSIS =====
    printf("⚙️  Generating tokens...\n");
    phase_start = clock();
    generateTokens(file_name);
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ Token generation completed [took %.3f ms]\n\n", phase_time_ms);

    printAllTokensToFile();

    // ===== PHASE 2: SYNTAX ANALYSIS =====
    printf("⚙️  Generating AST...\n");
    phase_start = clock();
    generateAllASTNodes();
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ AST generation completed [took %.3f ms]\n\n", phase_time_ms);

    printAllASTNodesToFile();

    // ===== PHASE 3: SEMANTIC ANALYSIS =====
    printf("⚙️  Performing semantic check...\n");
    phase_start = clock();
    doSemanticCheck();
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ Semantic check completed [took %.3f ms]\n\n", phase_time_ms);

    printSymbolTableToFile();

    // ===== PHASE 4: INTERMEDIATE CODE GENERATION =====
    printf("⚙️  Generating 3-address code...\n");
    phase_start = clock();
    startICG();
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ 3-address code generation completed [took %.3f ms]\n\n", phase_time_ms);

    print3AddressCodeToFile();
    
    // store instruction count before optimization for comparison
    instruction_count_before_optimization = addr_count;

    // ===== PHASE 5: OPTIMIZATION =====

    printf("⚙️  Running optimization process...\n");
    phase_start = clock();
    startOptimization();
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ Optimization process completed [took %.3f ms]\n\n", phase_time_ms);
    
    // store instruction count after optimization
    instruction_count_after_optimization = addr_count;
    
    // print optimized code with before/after statistics to show optimization effectiveness
    print3AddressCodeAfterOptimizationToFile(instruction_count_before_optimization, instruction_count_after_optimization);

    // ===== PHASE 6: TARGET CODE GENERATION =====
    printf("⚙️  Generating target code (x86-64)...\n");
    phase_start = clock();
    generateRealTargetCode();
    phase_end = clock();
    phase_time_ms = ((double)(phase_end - phase_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("✓ Target code generation completed [took %.3f ms]\n\n", phase_time_ms);
    
    printRealTargetCodeToFile();

    // Close output file
    closeOutputFile();
    
    // Calculate total time
    end_time = clock();
    total_time_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  Total compilation time: %.3f ms\n", total_time_ms);
    printf("  Output saved to: %s\n", output_file_name);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("✅ Compilation completed successfully!\n");

    exit(0);
}

