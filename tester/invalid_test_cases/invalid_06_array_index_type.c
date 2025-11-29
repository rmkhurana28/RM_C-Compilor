// Invalid 06: Array index not int

int main() {
    int arr[5];
    double idx = 2.5;
    arr[idx] = 10;  // Error: array index must be int
}
