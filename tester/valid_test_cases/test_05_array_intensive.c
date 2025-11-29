// Test 05: Maximum array operations and complex expressions

int main() {
    int numbers[25];
    double decimals[15];
    bool conditions[8];
    char letters[10];
    
    // Initialize with patterns
    int idx;
    for (idx = 0; idx < 25; idx++) {
        numbers[idx] = idx * idx;
    }
    
    int didx;
    for (didx = 0; didx < 15; didx++) {
        decimals[didx] = didx * 2.5;
    }
    
    int cidx;
    for (cidx = 0; cidx < 8; cidx++) {
        if (cidx > 4) {
            conditions[cidx] = true;
        } else {
            conditions[cidx] = false;
        }
    }
    
    int lidx;
    for (lidx = 0; lidx < 10; lidx++) {
        letters[lidx] = 'A';
    }
    
    // Complex array computations
    int totalsum = 0;
    double totalavg = 0.0;
    
    for (idx = 0; idx < 25; idx++) {
        totalsum = totalsum + numbers[idx];
    }
    
    for (didx = 0; didx < 15; didx++) {
        totalavg = totalavg + decimals[didx];
    }
    totalavg = totalavg / 15.0;
    
    // Array transformations based on conditions
    for (idx = 0; idx < 8; idx++) {
        if (conditions[idx]) {
            numbers[idx] = numbers[idx] * 2;
            decimals[idx] = decimals[idx] + 10.0;
        } else {
            numbers[idx] = numbers[idx] / 2;
            decimals[idx] = decimals[idx] - 5.0;
        }
    }
    
    // Deep nested array access
    for (idx = 0; idx < 20; idx++) {
        if (idx > 5) {
            if (idx < 15) {
                int temp = numbers[idx];
                numbers[idx] = numbers[idx + 1];
                numbers[idx + 1] = temp;
            }
        }
    }
    
    // Complex boolean array logic
    bool allflags = true;
    for (cidx = 0; cidx < 8; cidx++) {
        if (!conditions[cidx]) {
            allflags = false;
        }
    }
    
    if (allflags) {
        for (idx = 0; idx < 25; idx++) {
            numbers[idx] = numbers[idx] + 100;
        }
    }
    
    // Arithmetic expression chains
    int resulta = numbers[0] + numbers[5] - numbers[10];
    int resultb = (numbers[1] * numbers[2]) / numbers[3];
    int resultc = ((numbers[0] + numbers[24]) * totalsum) / 10;
    
    double resultd = decimals[0] + decimals[14];
    double resulte = (decimals[5] * decimals[10]) / decimals[7];
    double resultf = totalavg * 2.0 - decimals[0];
}
