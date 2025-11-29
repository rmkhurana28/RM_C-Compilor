// Invalid 11: Logical NOT on non-bool

int main() {
    int x = 10;
    bool y = !x;  // Error: ! requires bool operand
}
