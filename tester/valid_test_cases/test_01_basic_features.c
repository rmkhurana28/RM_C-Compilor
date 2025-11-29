// Test 01: Basic features - int, double, char, bool, arithmetic, simple control flow

int main() {
    // Integer operations
    int x;
    x = 10;
    int a = 20;
    int b = 5;
    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    int quot = a / b;
    
    // Double operations
    double pi = 3.14;
    double y = 2.5;
    double dsum = pi + y;
    double dprod = pi * y;
    double dresult = pi / y;
    
    // Boolean operations
    bool flag = true;
    bool test = false;
    bool resulta = flag || test;
    bool resultb = flag && test;
    bool resultc = !flag;
    
    // Character operations
    char ca = 'A';
    char cb = 'B';
    
    // Simple if statement
    if (x > 5) {
        x = x + 1;
    }
    
    // Simple if-else
    if (flag) {
        x = 100;
    } else {
        x = 200;
    }
    
    // Relational operators
    bool ra = a > b;
    bool rb = a < b;
    bool rc = a == b;
    bool rd = a != b;
    bool re = a >= b;
    bool rf = a <= b;
    
    // Parentheses
    int parenresult = ((a + b) * x) - ((a - b) / b);
    
    // Unary operators
    ++x;
    --x;
    x++;
    x--;
}
