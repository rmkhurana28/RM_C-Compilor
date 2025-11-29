// Test 03: Complex control flow - nested if, if-else chains, loops with conditions

int main() {
    int x = 10;
    int y = 20;
    int z = 30;
    bool flag = true;
    
    // Nested if statements
    if (x > 5) {
        if (y > 15) {
            if (z > 25) {
                x = x + y + z;
            }
        }
    }
    
    // If-else chain
    if (x < 10) {
        y = 1;
    } else {
        if (x < 20) {
            y = 2;
        } else {
            if (x < 30) {
                y = 3;
            } else {
                y = 4;
            }
        }
    }
    
    // While with if
    while (x < 100) {
        if (x > 50) {
            x = x + 5;
        } else {
            x = x + 10;
        }
    }
    
    // For with if-else
    int i;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        if (i > 5) {
            sum = sum + i * 2;
        } else {
            sum = sum + i;
        }
    }
    
    // Complex boolean expressions with control flow
    if ((x > 10) && (y < 50) || (z == 30)) {
        if (!flag || (x != y)) {
            x = 0;
        }
    }
}
