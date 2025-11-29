// Invalid 08: Bool with arithmetic operation

int main() {
    bool x = true;
    bool y = x + 5;  // Error: can't add int to bool
}
