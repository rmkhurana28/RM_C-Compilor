// Test 08: Ultimate complexity test - maximum nesting and all features combined

int main() {
    // Declare all array types
    int matrix[30];
    double scores[20];
    bool states[15];
    char codes[10];
    
    // Multiple control variables
    int i;
    int j;
    int k;
    int m;
    int n;
    
    // Initialize matrix with complex pattern
    for (i = 0; i < 30; i++) {
        if (i < 10) {
            matrix[i] = i * i;
        } else {
            if (i < 20) {
                matrix[i] = i * 2;
            } else {
                matrix[i] = i + 100;
            }
        }
    }
    
    // Initialize scores with nested conditions
    for (j = 0; j < 20; j++) {
        if (j > 15) {
            scores[j] = j * 3.5;
        } else {
            if (j > 10) {
                scores[j] = j * 2.5;
            } else {
                if (j > 5) {
                    scores[j] = j * 1.5;
                } else {
                    scores[j] = j * 0.5;
                }
            }
        }
    }
    
    // Initialize states with alternating pattern
    for (k = 0; k < 15; k++) {
        if (k > 7) {
            states[k] = true;
        } else {
            states[k] = false;
        }
    }
    
    // Initialize codes
    for (m = 0; m < 10; m++) {
        codes[m] = 'A';
    }
    
    // LEVEL 1: Triple nested for loops with complex logic
    int globalsum = 0;
    int globalcount = 0;
    
    for (i = 0; i < 5; i++) {
        int outersum = 0;
        
        for (j = 0; j < 4; j++) {
            int middlesum = 0;
            
            for (k = 0; k < 3; k++) {
                int inneridx = i * 12 + j * 3 + k;
                
                if (inneridx < 30) {
                    if (matrix[inneridx] > 50) {
                        matrix[inneridx] = matrix[inneridx] - 10;
                        middlesum = middlesum + matrix[inneridx];
                    } else {
                        if (matrix[inneridx] > 25) {
                            matrix[inneridx] = matrix[inneridx] + 5;
                            middlesum = middlesum + matrix[inneridx] / 2;
                        } else {
                            matrix[inneridx] = matrix[inneridx] * 2;
                            middlesum = middlesum + matrix[inneridx];
                        }
                    }
                    globalcount++;
                }
            }
            
            outersum = outersum + middlesum;
            
            if (middlesum > 100) {
                for (m = 0; m < 5; m++) {
                    if (m < 15) {
                        states[m] = !states[m];
                    }
                }
            }
        }
        
        globalsum = globalsum + outersum;
    }
    
    // LEVEL 2: Nested while with complex conditions
    int whilecount = 0;
    while (whilecount < 10) {
        if (whilecount < 3) {
            for (i = 0; i < 5; i++) {
                if (i < 20) {
                    scores[i] = scores[i] + 1.0;
                }
            }
            whilecount++;
        } else {
            if (whilecount < 7) {
                int innerwhile = 0;
                while (innerwhile < 3) {
                    matrix[whilecount] = matrix[whilecount] + innerwhile;
                    innerwhile++;
                }
                whilecount++;
            } else {
                for (j = 0; j < 10; j++) {
                    if (j < 15) {
                        if (states[j]) {
                            matrix[j] = matrix[j] * 2;
                        } else {
                            matrix[j] = matrix[j] / 2;
                        }
                    }
                }
                whilecount++;
            }
        }
    }
    
    // LEVEL 3: Deeply nested if-else with loops inside
    if (globalsum > 500) {
        if (globalcount > 50) {
            for (i = 0; i < 20; i++) {
                if (scores[i] > 30.0) {
                    if (scores[i] < 60.0) {
                        scores[i] = scores[i] * 1.1;
                        
                        for (j = 0; j < 5; j++) {
                            if (j < 15) {
                                if (states[j]) {
                                    scores[i] = scores[i] + 0.5;
                                }
                            }
                        }
                    } else {
                        scores[i] = scores[i] - 5.0;
                    }
                } else {
                    scores[i] = scores[i] + 10.0;
                }
            }
        } else {
            int tempcount = 0;
            while (tempcount < globalcount) {
                if (tempcount < 30) {
                    matrix[tempcount] = matrix[tempcount] + 1;
                }
                tempcount++;
                
                if (tempcount > 20) {
                    tempcount = globalcount;
                }
            }
        }
    } else {
        if (globalsum > 250) {
            for (i = 0; i < 30; i++) {
                if (matrix[i] > 100) {
                    matrix[i] = matrix[i] - 50;
                } else {
                    if (matrix[i] > 50) {
                        matrix[i] = matrix[i] - 25;
                    } else {
                        matrix[i] = matrix[i] + 25;
                    }
                }
            }
        } else {
            for (j = 0; j < 15; j++) {
                states[j] = !states[j];
            }
        }
    }
    
    // LEVEL 4: For loop with nested while and if chains
    for (i = 0; i < 10; i++) {
        int loopcounter = 0;
        
        while (loopcounter < 5) {
            if (loopcounter < 2) {
                if (i < 20) {
                    scores[i] = scores[i] + loopcounter * 0.5;
                }
            } else {
                if (loopcounter < 4) {
                    if (i < 30) {
                        matrix[i] = matrix[i] + loopcounter;
                    }
                } else {
                    if (i < 15) {
                        states[i] = !states[i];
                    }
                }
            }
            loopcounter++;
        }
    }
    
    // LEVEL 5: Complex boolean calculations with nested structures
    bool complexflag = false;
    int boolcount = 0;
    
    for (i = 0; i < 15; i++) {
        if (states[i]) {
            boolcount++;
        }
    }
    
    if (boolcount > 7) {
        complexflag = true;
    }
    
    if (complexflag) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 4; j++) {
                for (k = 0; k < 3; k++) {
                    int calcidx = i * 12 + j * 3 + k;
                    if (calcidx < 30) {
                        if (matrix[calcidx] > 75) {
                            matrix[calcidx] = matrix[calcidx] - 25;
                        } else {
                            matrix[calcidx] = matrix[calcidx] + 25;
                        }
                    }
                }
            }
        }
    } else {
        int altcount = 0;
        while (altcount < 20) {
            if (altcount < 20) {
                scores[altcount] = scores[altcount] * 0.9;
            }
            altcount++;
        }
    }
    
    // LEVEL 6: Final massive computation with all operators
    double finalavg = 0.0;
    int finalsum = 0;
    
    for (i = 0; i < 20; i++) {
        finalavg = finalavg + scores[i];
        if (i < 30) {
            finalsum = finalsum + matrix[i];
        }
    }
    finalavg = finalavg / 20.0;
    
    // Complex arithmetic with nested conditions
    int resulta = 0;
    int resultb = 0;
    int resultc = 0;
    
    for (i = 0; i < 25; i++) {
        if (i < 10) {
            resulta = resulta + matrix[i];
        } else {
            if (i < 20) {
                resultb = resultb + matrix[i];
            } else {
                resultc = resultc + matrix[i];
            }
        }
    }
    
    int finalresult = ((resulta + resultb + resultc) / 3) * boolcount;
    
    // Unary operations in complex structure
    for (i = 0; i < 10; i++) {
        if (i < 5) {
            ++matrix[i];
            matrix[i]++;
        } else {
            --matrix[i];
            matrix[i]--;
        }
    }
    
    // Final nested structure with all features
    if (finalresult > 1000) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 4; j++) {
                if (i > j) {
                    int tempidx = i * 5 + j;
                    if (tempidx < 30) {
                        matrix[tempidx] = (matrix[tempidx] + finalresult) / 2;
                    }
                } else {
                    int tempidx = j * 5 + i;
                    if (tempidx < 30) {
                        matrix[tempidx] = (matrix[tempidx] - finalresult) / 2;
                    }
                }
            }
        }
    } else {
        int countdown = 30;
        while (countdown > 0) {
            countdown--;
            if (countdown < 30) {
                matrix[countdown] = matrix[countdown] + 1;
                
                if (matrix[countdown] > 200) {
                    matrix[countdown] = 200;
                }
            }
        }
    }
    
    // Ultra complex expression
    int ultra = ((resulta * 2 + resultb * 3 + resultc * 4) / 9) + boolcount * 10;
    double ultraavg = (finalavg * 2.0 + scores[0] + scores[19]) / 4.0;
    
    // Final boolean logic
    bool finala = (ultra > 500) && (ultraavg > 25.0);
    bool finalb = (boolcount > 10) || (finalsum < 2000);
    bool finalc = !complexflag && finala;
    bool finald = (finala || finalb) && (finalc || complexflag);
}
