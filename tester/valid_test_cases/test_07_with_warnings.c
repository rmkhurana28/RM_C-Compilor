// Test 07: Advanced features with warnings - non-bool conditions, complex scenarios

int main() {
    int x = 10;
    int y = 20;
    double d = 5.5;
    bool flag = true;
    
    // Warning: int in if condition
    if (x) {
        y = 100;
    }
    
    // Warning: double in while condition 
    while (d) {
        d = d - 1.0;
        if (d < 1.0) {
            d = 0.0;
        }
    }
    
    // Warning: int in for condition 
    int i;
    for (i = 0; i; i++) {
        if (i > 5) {
            i = 0;
        }
    }
    
    // Complex with uninitialized variables 
    int uninit;
    int result = uninit + 10;
    
    // Variable array size 
    int size = 5;
    int arr[10];
    for (i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    
    // Complex nested structures with warnings
    int counter = 5;
    if (counter) {
        while (counter) {
            counter--;
            if (counter) {
                y = y + counter;
            }
        }
    }
    
    // Mixed warnings and valid operations
    double dx = 3.14;
    if (dx) {
        dx = dx * 2.0;
        if (dx > 5.0) {
            dx = dx / 2.0;
        }
    }
    
    // Array operations with warnings
    int warr[8];
    int widx = 0;
    while (widx) {
        warr[widx] = widx;
        widx++;
        if (widx > 7) {
            widx = 0;
        }
    }
    
    // Complex boolean and non-boolean mix 
    bool xbool = x > 0;
    bool ybool = y > 0;
    if (xbool && ybool) {
        x = x + y;
    }
    
    bool xboolb = x > 0;
    if (flag || xboolb) {
        y = 200;
    }
}
