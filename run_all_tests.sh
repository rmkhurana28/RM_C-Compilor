#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
valid_passed=0
valid_failed=0
invalid_passed=0
invalid_failed=0

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  RM COMPILER - TEST SUITE RUNNER"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Function to update Main.c with test file
update_main_file() {
    local test_file=$1
    sed -i "s|strcpy(file_name, \".*\");|strcpy(file_name, \"$test_file\");|" Main.c
}

# Function to restore Main.c
restore_main_file() {
    sed -i 's|strcpy(file_name, ".*");|strcpy(file_name, "tester/test.c");|' Main.c
}

# Test valid test cases
echo -e "${BLUE}━━━ Testing VALID test cases (should compile successfully) ━━━${NC}"
echo ""

for test_file in tester/valid_test_cases/*.c; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file")
        printf "  Testing %-40s " "$test_name"
        
        # Update Main.c to use this test file
        update_main_file "$test_file"
        
        # Compile silently
        make -s > /dev/null 2>&1
        
        # Run the compiler and capture output
        ./Main > /tmp/test_output.txt 2>&1
        exit_code=$?
        
        # Check if compilation was successful
        if [ $exit_code -eq 0 ] && grep -q "Compilation completed successfully" /tmp/test_output.txt; then
            # Check if there are warnings
            if grep -qi "warning" /tmp/test_output.txt; then
                echo -e "${GREEN}✓ PASS${NC} ${YELLOW}(with warnings)${NC}"
            else
                echo -e "${GREEN}✓ PASS${NC}"
            fi
            ((valid_passed++))
        else
            echo -e "${RED}✗ FAIL${NC}"
            ((valid_failed++))
        fi
    fi
done

echo ""
echo -e "${BLUE}━━━ Testing INVALID test cases (should fail with errors) ━━━${NC}"
echo ""

for test_file in tester/invalid_test_cases/*.c; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file")
        printf "  Testing %-40s " "$test_name"
        
        # Update Main.c to use this test file
        update_main_file "$test_file"
        
        # Compile silently
        make -s > /dev/null 2>&1
        
        # Run the compiler and capture output (suppress bash segfault messages)
        { ./Main > /tmp/test_output.txt 2>&1; } 2>/dev/null
        exit_code=$?
        
        # Check if compilation failed with ERROR (not warnings)
        # Exclude lines with "warning" and only check for "error"
        if [ $exit_code -ne 0 ] || grep -v -i "warning" /tmp/test_output.txt | grep -qi "error"; then
            echo -e "${GREEN}✓ PASS${NC} (failed as expected)"
            ((invalid_passed++))
        else
            echo -e "${RED}✗ FAIL${NC} (should have failed)"
            ((invalid_failed++))
        fi
    fi
done

# Restore Main.c to original state
restore_main_file
make -s > /dev/null 2>&1

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  TEST RESULTS SUMMARY"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "  Valid Test Cases:"
echo -e "    ${GREEN}Passed: $valid_passed${NC}"
if [ $valid_failed -gt 0 ]; then
    echo -e "    ${RED}Failed: $valid_failed${NC}"
else
    echo -e "    ${GREEN}Failed: $valid_failed${NC}"
fi
echo ""
echo "  Invalid Test Cases:"
echo -e "    ${GREEN}Passed: $invalid_passed${NC}"
if [ $invalid_failed -gt 0 ]; then
    echo -e "    ${RED}Failed: $invalid_failed${NC}"
else
    echo -e "    ${GREEN}Failed: $invalid_failed${NC}"
fi
echo ""

total_passed=$((valid_passed + invalid_passed))
total_failed=$((valid_failed + invalid_failed))
total_tests=$((total_passed + total_failed))

echo "  Overall:"
echo "    Total tests: $total_tests"
echo -e "    ${GREEN}Passed: $total_passed${NC}"
if [ $total_failed -gt 0 ]; then
    echo -e "    ${RED}Failed: $total_failed${NC}"
else
    echo -e "    ${GREEN}Failed: $total_failed${NC}"
fi

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Final status message
if [ $total_failed -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED! Your compiler is working correctly.${NC}"
else
    echo -e "${RED}❌ SOME TESTS FAILED! Please review the failures above.${NC}"
fi

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Clean up
rm -f /tmp/test_output.txt

# Exit with appropriate code
if [ $total_failed -eq 0 ]; then
    exit 0
else
    exit 1
fi
