// rm 129
// Date: 2025-10-07 20:52


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// comment

#define MAX 128
#define RIDOM false
/*
    this is multi-line comment

    
*/

/* ALLOWED

    // ---------- INT ----------
    // Simple single variable declarations
    int a;
    int a = 5;    

    // Multiple variables
    int a,b,c;
    int a=1,b=2,c=3;
    int a=1,b,c=3;

    // Arrays
    int arr[3];
    int arr[3] = {1,2,3};

    // ---------- DOUBLE ----------
    double a;
    double a = 3.14;

    // Multiple variables
    double a, b, c;
    double a = 1.0, b = 2.5, c = 3.0;
    double a = 1.0, b, c = 3.0;

    // Arrays
    double arr[3];
    double arr[3] = {3.14, 2.71, 1.618};

    // ---------- BOOL ----------
    bool flag;
    bool flag = true;

    // Multiple variables
    bool flag1, flag2, flag3;
    bool flag1 = true, flag2 = false, flag3 = true;
    bool flag1 = true, flag2, flag3 = false;

    // Arrays
    bool arr[3];
    bool arr[3] = {true, false, true};

    // ---------- CHAR ----------
    char c;
    char c = 'A';

    // Multiple variables
    char c1, c2, c3;
    char c1 = 'X', c2 = 'Y', c3 = 'Z';
    char c1 = 'A', c2, c3 = 'B';

    // Arrays
    char arr[3];
    char arr[3] = {'a', 'b', 'c'};

    // ---------- STRING (char arrays) ----------
    char word[6];            // uninitialized
    char word[6] = "hello";  // initialized with string literal

    // Multiple string variables
    char w1[4], w2[5], w3[6];
    char w1[4] = "hey", w2[5] = "test", w3[6] = "hello";

    // ---------- if else ----------
    if (x > y) {
        x = x - 1;
    } else {
        x = y - 1;
    }

    // ---------- for ----------
    int k;
    for(k=0 ; k<5; k++)
        x = x + k;

    // ---------- WHILE ----------
    int i = 0;
    while (i<5)
        i = i + 1;


*/

/* NOT ALLOWED

    // variable in the length of array not allowed : 
    int list[MAX]; NOT ALLOWED

    // double brackets continously not allowed, must be space between them : 
    ans = ((a+b)*2) NOT allowed : MUST be atleast 1 space between brackets

    // integer declaration not allowed in condition
    for(int k=0 ; k<5; k++) : NOT ALLOWED

*/

/*
    int x;
    int y=5;
    int z = 10 ;
    char c='A';
    char d = 'B' ;
    double pi=3.14;
    double e = 2.71828 ;
    bool flag=true;
    bool check = false ;

    x=5;
    x =5;
    x= 5;
    x = 5 ;
    y=x+z;
    y = x + z ;
    z= x * y / 2 - 3;
    z = (x + y) * 2;

    pi = pi + 0.001;
    e=e*2.0;
    flag=!flag;
    check = (x > y) && (z <= 20) || false;

    if(x==y)
        x = x + 1;
    else if (x != y && y > 5 )
    {
        y = y - 1 ;
    }
    else
    {
        z = z + 2;
    }

    while(x<10){
        x = x + 1 ;
    }

    int i;
    
    for(i=0;i<5;i=i+1){
        z = z + i;
    }

    int arr[3];
    arr[0]=1;
    arr [ 1 ] = 2 ;
    arr[2] = arr[0] + arr[1];

    if (flag && check)
        y = y + 10;
    else
        y = y - 5;

    x = (y + z) * 2 - (x / 3);
    flag = (x == 10) && (y != 0) || !check;

    int result = 0;
    for (int i = 0; i < 3; i = i + 1) {
        result = result + i;
    }

    {
        int a = ( (5 + 2) * 3 );
        char letters[2] = {'a','b'};
        bool compare = (a > 10);
    }

    int a,b,c;
    a=1,b=2,c=3;
    x = a + b , y = c ;

    int f;
    int g;
    
    x = ( (a + b) * (c - d) ) / (e + (f * g) );
    int     spaced   =    10   ;

    y =
    x
    +
    z;


    bool done = true;
    if(done){ x = 0; }

    char word[5] = "hello";
    char letters[2] = {'a','b'};
    char c='A';
    
    for(i=0 ; i<5 ; --i );

    int a = 56;
    int b = 3.14;
    c = 7.8;
*/

int main() {        

    int alpha = 5;
    int beta = 10;
    int gamma;
    int delta[10];
    bool flag = true;
    int sum = 0;
    int counter = 0;

    if(++alpha > beta || flag){
        gamma = alpha++ * 2 + beta;

        for(int idx = 0; idx < 5; idx++){
            delta[idx] = ++gamma;

            if(delta[idx] > 20){
                sum = sum + delta[idx]++;
            } else {
                sum = sum + idx;
            }
        }

        while(counter < 3 && sum < 100){
            counter++;
            beta = beta++ + counter * 2;

            if(beta > 50){
                flag = false;
            }
        }
    } else {
        gamma = beta / 2;
    }

    int result = sum + ++counter + gamma++;
    bool status = (result > 100) && flag || (alpha == beta);
    
}

