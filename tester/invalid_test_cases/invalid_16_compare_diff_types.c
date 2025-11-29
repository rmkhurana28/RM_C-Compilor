// Invalid 16: Comparison different types

int main() {
    int x = 10;
    char c = 'A';
    bool result = x > c;  // Warning: comparing different types
}
