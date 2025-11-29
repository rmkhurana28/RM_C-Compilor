// Test 02: Arrays and loops - all array types, for, while, nested loops

int main() {
    // Array declarations
    int arr[5] = {1, 2, 3, 4, 5};
    double darr[3] = {1.1, 2.2, 3.3};
    bool barr[2] = {true, false};
    char carr[4] = {'a', 'b', 'c', 'd'};
    
    int i;
    int sum = 0;
    
    // For loop with array access
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + arr[i];
    }
    
    // While loop
    int j = 0;
    while (j < 3) {
        darr[j] = darr[j] * 2.0;
        j++;
    }
    
    // Nested loops
    int k;
    int m;
    for (k = 0; k < 3; k++) {
        for (m = 0; m < 2; m++) {
            arr[k] = arr[k] + m;
        }
    }
    
    // Array assignment and expressions
    arr[0] = arr[1] + arr[2];
    darr[1] = darr[0] / darr[2];
    barr[0] = barr[1] || true;
}
