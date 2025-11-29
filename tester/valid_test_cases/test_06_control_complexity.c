// Test 06: Control flow complexity with all operators

int main() {
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int e = 50;
    
    double x = 1.5;
    double y = 2.5;
    double z = 3.5;
    
    bool p = true;
    bool q = false;
    bool r = true;
    
    // Deeply nested if-else chains
    if (a > 5) {
        if (b > 15) {
            if (c > 25) {
                if (d > 35) {
                    if (e > 45) {
                        a = 100;
                    } else {
                        a = 200;
                    }
                } else {
                    a = 300;
                }
            } else {
                a = 400;
            }
        } else {
            a = 500;
        }
    } else {
        a = 600;
    }
    
    // Complex while loops with nested conditions
    int counter = 0;
    while (counter < 20) {
        if (counter < 5) {
            counter = counter + 1;
        } else {
            if (counter < 10) {
                counter = counter + 2;
            } else {
                if (counter < 15) {
                    counter = counter + 3;
                } else {
                    counter = counter + 4;
                }
            }
        }
    }
    
    // For loops with complex conditions
    int i;
    int sum = 0;
    for (i = 0; i < 50; i++) {
        if (i > 40) {
            sum = sum + i * 3;
        } else {
            if (i > 30) {
                sum = sum + i * 2;
            } else {
                if (i > 20) {
                    sum = sum + i;
                } else {
                    if (i > 10) {
                        sum = sum + i / 2;
                    } else {
                        sum = sum + 1;
                    }
                }
            }
        }
    }
    
    // Mixed nested loops
    int outer;
    int inner;
    for (outer = 0; outer < 10; outer++) {
        for (inner = 0; inner < 10; inner++) {
            if (outer > inner) {
                sum = sum + outer;
            } else {
                sum = sum + inner;
            }
        }
    }
    
    // While with nested for
    int whilecounter = 0;
    while (whilecounter < 5) {
        for (i = 0; i < 5; i++) {
            if (i > whilecounter) {
                whilecounter++;
            }
        }
        whilecounter++;
    }
    
    // Complex boolean expressions
    bool complexa = (a > b) && (c < d);
    bool complexb = (e > a) || (b < c);
    bool complexc = !p && q;
    bool complexd = (p || q) && r;
    bool complexe = !(p && q) || (r && !q);
    
    // Relational operations
    bool rela = a == b;
    bool relb = c != d;
    bool relc = a < b;
    bool reld = c > d;
    bool rele = a <= b;
    bool relf = c >= d;
    
    // Unary operations chains
    ++a;
    ++a;
    ++a;
    a++;
    a++;
    --b;
    --b;
    b--;
    
    // Arithmetic expression chains
    int aritha = ((a + b) * c) - ((d - e) / a);
    int arithb = (a * b) + (c * d) - (e * a);
    int arithc = ((a + b + c + d + e) / 5) * 10;
    
    double daritha = ((x + y) * z) / x;
    double darithb = (x * y) - (y * z) + (z * x);
    double darithc = (x + y + z) / 3.0;
}
