// Invalid 15: Logical AND with non-bool

int main() {
    int x = 10;
    int y = 20;
    bool result = x && y;  // Error: && requires bool operands
}
