#include "database.h"

// Symbol-to-token lookup table
typedef struct {
    char *symbol;
    tokenType type;
} SymbolLookup;

SymbolLookup symbolLookup[] = {
    // ----- Keywords -----
    {"int", KEYWORD_INT},
    {"char", KEYWORD_CHAR},
    {"double", KEYWORD_DOUBLE},
    {"bool", KEYWORD_BOOL},
    {"if", KEYWORD_IF},
    {"else", KEYWORD_ELSE},
    {"while", KEYWORD_WHILE},
    {"for", KEYWORD_FOR},
    {"true", VAL_BOOL},
    {"false", VAL_BOOL},

    // ----- Operators -----
    {"==", OP_IS_EQUAL},
    {"!=", OP_NOT_EQUAL},
    {"<=", OP_LESS_EQUAL},
    {">=", OP_GREAT_EQUAL},
    {"&&", OP_AND_AND},
    {"||", OP_OR_OR},
    {"+", OP_PLUS},
    {"-", OP_MINUS},
    {"*", OP_MUL},
    {"/", OP_DIV},
    {"=", OP_EQUAL},
    {"<", OP_LESS_THAN},
    {">", OP_GREAT_THAN},
    {"!", OP_NOT},
    {"++", OP_PLUS_PLUS},
    {"--", OP_MINUS_MINUS},

    // ----- Delimiters / Punctuation -----
    {"." , FULL},
    {";", SEMI},
    {",", COMMA},
    {"(", L_PARAN},
    {")", R_PARAN},
    {"{", L_BRACES},
    {"}", R_BRACES},
    {"[", L_BRACK},
    {"]", R_BRACK},

    // ----- End marker -----
    {NULL, MISC_UNKNOWN}
};

typedef void (*StoreFunc)(tokenType, char*);


typedef struct {
    tokenType type;
    StoreFunc func;
} TokenStoreMap;

void storeInt(tokenType type , char* word);
void storeDouble(tokenType type , char* word);
void storeBool(tokenType type , char* word);
void storeString(tokenType type , char* word);

TokenStoreMap tokenStoreMap[] = {
    // Numeric & boolean literals
    {VAL_INT, storeInt},
    {VAL_DOUBLE, storeDouble},
    {VAL_CHAR , storeString},
    {VAL_STRING, storeString},
    {VAL_BOOL, storeBool},

    // Everything else (identifiers, keywords, symbols, etc.)
    {ID, storeString},
    {KEYWORD_INT, storeString},
    {KEYWORD_CHAR, storeString},
    {KEYWORD_DOUBLE, storeString},
    {KEYWORD_BOOL, storeString},
    {KEYWORD_IF, storeString},
    {KEYWORD_ELSE, storeString},
    {KEYWORD_WHILE, storeString},
    {KEYWORD_FOR, storeString},

    // Operators
    {OP_PLUS, storeString},
    {OP_MINUS, storeString},
    {OP_MUL, storeString},
    {OP_DIV, storeString},
    {OP_EQUAL, storeString},
    {OP_IS_EQUAL, storeString},
    {OP_NOT_EQUAL, storeString},
    {OP_LESS_THAN, storeString},
    {OP_GREAT_THAN, storeString},
    {OP_LESS_EQUAL, storeString},
    {OP_GREAT_EQUAL, storeString},
    {OP_AND_AND, storeString},
    {OP_OR_OR, storeString},
    {OP_NOT, storeString},
    {OP_PLUS_PLUS, storeString},
    {OP_MINUS_MINUS, storeString},

    // Punctuations
    {FULL , storeString},
    {SEMI, storeString},
    {COMMA, storeString},
    {L_PARAN, storeString},
    {R_PARAN, storeString},
    {L_BRACES, storeString},
    {R_BRACES, storeString},
    {L_BRACK, storeString},
    {R_BRACK, storeString},

    // Misc
    {MISC_EOF, storeString},
    {MISC_UNKNOWN, storeString}
};

// char lookup enum
typedef enum {
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_OPERATOR,
    CHAR_S_BRACKET,
    CHAR_M_BRACKET,
    CHAR_BRACKET,
    CHAR_SEMI,
    CHAR_COMMA,
    CHAR_FULL,       // maybe for '.' (period) if you need it)
    CHAR_WHITESPACE,
    CHAR_UNKNOWN
} CharType;

typedef struct {
    char c;      // single character
    CharType type;    // category
} CharLookup;

CharLookup charLookup[] = {
    // ----- Letters -----
    {'a', CHAR_LETTER}, {'b', CHAR_LETTER}, {'c', CHAR_LETTER}, {'d', CHAR_LETTER},
    {'e', CHAR_LETTER}, {'f', CHAR_LETTER}, {'g', CHAR_LETTER}, {'h', CHAR_LETTER},
    {'i', CHAR_LETTER}, {'j', CHAR_LETTER}, {'k', CHAR_LETTER}, {'l', CHAR_LETTER},
    {'m', CHAR_LETTER}, {'n', CHAR_LETTER}, {'o', CHAR_LETTER}, {'p', CHAR_LETTER},
    {'q', CHAR_LETTER}, {'r', CHAR_LETTER}, {'s', CHAR_LETTER}, {'t', CHAR_LETTER},
    {'u', CHAR_LETTER}, {'v', CHAR_LETTER}, {'w', CHAR_LETTER}, {'x', CHAR_LETTER},
    {'y', CHAR_LETTER}, {'z', CHAR_LETTER},

    {'A', CHAR_LETTER}, {'B', CHAR_LETTER}, {'C', CHAR_LETTER}, {'D', CHAR_LETTER},
    {'E', CHAR_LETTER}, {'F', CHAR_LETTER}, {'G', CHAR_LETTER}, {'H', CHAR_LETTER},
    {'I', CHAR_LETTER}, {'J', CHAR_LETTER}, {'K', CHAR_LETTER}, {'L', CHAR_LETTER},
    {'M', CHAR_LETTER}, {'N', CHAR_LETTER}, {'O', CHAR_LETTER}, {'P', CHAR_LETTER},
    {'Q', CHAR_LETTER}, {'R', CHAR_LETTER}, {'S', CHAR_LETTER}, {'T', CHAR_LETTER},
    {'U', CHAR_LETTER}, {'V', CHAR_LETTER}, {'W', CHAR_LETTER}, {'X', CHAR_LETTER},
    {'Y', CHAR_LETTER}, {'Z', CHAR_LETTER},

    {'_', CHAR_LETTER},  // underscore for identifiers

    // ----- Digits -----
    {'0', CHAR_DIGIT}, {'1', CHAR_DIGIT}, {'2', CHAR_DIGIT}, {'3', CHAR_DIGIT},
    {'4', CHAR_DIGIT}, {'5', CHAR_DIGIT}, {'6', CHAR_DIGIT}, {'7', CHAR_DIGIT},
    {'8', CHAR_DIGIT}, {'9', CHAR_DIGIT},

    // ----- Operators -----
    {'+', CHAR_OPERATOR}, {'-', CHAR_OPERATOR}, {'*', CHAR_OPERATOR},
    {'/', CHAR_OPERATOR}, {'=', CHAR_OPERATOR}, {'<', CHAR_OPERATOR},
    {'>', CHAR_OPERATOR}, {'!', CHAR_OPERATOR}, {'&', CHAR_OPERATOR}, {'|', CHAR_OPERATOR},
    {'%', CHAR_OPERATOR}, {'^', CHAR_OPERATOR}, {'~', CHAR_OPERATOR},

    // ----- Brackets -----
    {'(', CHAR_S_BRACKET}, {')', CHAR_S_BRACKET},
    {'{', CHAR_M_BRACKET}, {'}', CHAR_M_BRACKET},
    {'[', CHAR_BRACKET}, {']', CHAR_BRACKET},

    // ----- Punctuation -----
    {';', CHAR_SEMI},
    {',', CHAR_COMMA},
    {'.', CHAR_FULL},

    // ----- Whitespace -----
    {' ', CHAR_WHITESPACE},
    {'\t', CHAR_WHITESPACE},
    {'\n', CHAR_WHITESPACE},
    {'\r', CHAR_WHITESPACE},

    // ----- End marker -----
    {0, CHAR_UNKNOWN}
};

void skipTopLines(FILE* fptr);

char* readNext(FILE* fptr);

tokenType getTokenTypeOf(char* temp_name);
StoreFunc getStoreFunctionOf(char* temp_name , tokenType type);

CharType getCharTypeOf(char c);

void generateTokens(char* file_name){
    FILE* fptr = NULL; // generate file pointer to read data from
    
    fptr = fopen(file_name,"r"); // open file with read permissions

    if(!fptr){ // file name is invalid
        printf("File [%s] NOT found...\n", file_name);
        exit(1);
    }

    printf("File [%s] opened succesfully\n" , file_name); // file is opened 


    skipTopLines(fptr); // skip lines before entering into the main

    printf("current-pointor = %c\n", fgetc(fptr));

    char c = fgetc(fptr); // pointor to read file
    c = fgetc(fptr);

    char* temp_name; // pointer to store words from the file
    int temp_name_count;    

    bool shift = false;

    while(c != EOF){
        

        shift = false;
        while(c == ' ' || c == '\n' || c == '\t'){ // skipping white spaces
            shift = true;
            c = fgetc(fptr);
        }
        if(shift) ungetc(c,fptr);

        temp_name = readNext(fptr);
        if(temp_name == NULL){
            break;
        }
        c = fgetc(fptr);
        ungetc(c,fptr);


        tokenType temp_token_type = getTokenTypeOf(temp_name); // getting token_type of the words        

        StoreFunc temp_token_storeFunc = getStoreFunctionOf(temp_name,temp_token_type); // getting which function to call to generate token 
        temp_token_storeFunc(temp_token_type,temp_name); // generating token
        
    }
            
    fclose(fptr); // close the file
}

void skipTopLines(FILE* fptr) {
    if (!fptr) return;

    int c;
    while ((c = fgetc(fptr)) != EOF) {
        // Stop if we reach '{'
        if (c == '{') {
            ungetc(c, fptr);  // put it back, pointer now points at '{'
            break;
        }

        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\n') continue;

        // Preprocessor lines
        if (c == '#') {
            while ((c = fgetc(fptr)) != '\n' && c != EOF);
        } 
        // Comments
        else if (c == '/') {
            int next = fgetc(fptr);
            if (next == '/') {  // single-line comment
                while ((c = fgetc(fptr)) != '\n' && c != EOF);
            } else if (next == '*') {  // multi-line comment
                int star = 0;
                while ((c = fgetc(fptr)) != EOF) {
                    if (c == '*') star = 1;
                    else if (c == '/' && star) break;
                    else star = 0;
                }
            } else {
                // if next char is whitespace, continue
                if (next == ' ' || next == '\t' || next == '\n') continue;

                fprintf(stderr, "Typo in lines above main function...\n");
                exit(2);
            }
        } 
        // Other characters, just ignore and continue
        else {
            continue;
        }
    }
}

tokenType getTokenTypeOf(char* temp_name){
    // checking if it is int
    int dot_count = 0;
    int is_number = 1;

    for (int i = 0; temp_name[i]; i++) {
        if (temp_name[i] == '.') {
            dot_count++;
        } else if (!isdigit(temp_name[i])) {
            is_number = 0;
            break;
        }
    }

    if (is_number) {
        if (dot_count == 0)
            return VAL_INT;       // e.g. "5", "100"
        else if (dot_count == 1)
            return VAL_DOUBLE;    // e.g. "3.14", "0.5"
    }

    for(int i=0 ; symbolLookup[i].symbol ; i++){
        if(isStringSame(symbolLookup[i].symbol , temp_name)) return symbolLookup[i].type;
    }

    if(temp_name[0] == '\''){
        return VAL_CHAR;
    }

    if(temp_name[0] == '\"'){
        return VAL_STRING;
    }

    return ID; // default
}

StoreFunc getStoreFunctionOf(char* temp_name , tokenType type){
    
    StoreFunc storeFunc = storeString; // default
    
    for (int i = 0; i < sizeof(tokenStoreMap)/sizeof(tokenStoreMap[0]); i++) {
        if (tokenStoreMap[i].type == type) {
            storeFunc = tokenStoreMap[i].func;
            break;
        }
    }

    return storeFunc;
}

void storeInt(tokenType type , char* word){
    Token* temp = (Token*)malloc(sizeof(Token)); // allocate memory for new token

    // set the parameters
    temp->token_type = type;
    sscanf(word , "%d" , &temp->int_value);
    // temp->int_value = num;

    // check if tokens array is full
    if(token_count >= MAX){
        printf("NOT enough space to store tokens..\n");
        exit(3);
    }

    // add token to the array
    tokens[token_count++] = temp;

    return;
}

void storeDouble(tokenType type , char* word){ // allocate memory for new token
    Token* temp = (Token*)malloc(sizeof(Token));

    // set the parameters
    temp->token_type = type;
    sscanf(word , "%lf" , &temp->double_value);

    // check if tokens array is full
    if(token_count >= MAX){
        printf("NOT enough space to store tokens..\n");
        exit(3);
    }

    // add token to the array
    tokens[token_count++] = temp;

    return;
}

void storeBool(tokenType type , char* word){ // allocate memory for new token
    Token* temp = (Token*)malloc(sizeof(Token));

    // set the parameters
    temp->token_type = type;
    if(isStringSame(word,"true")){
        temp->bool_value = true;
    } else temp->bool_value = false;
    
    // check if tokens array is full
    if(token_count >= MAX){
        printf("NOT enough space to store tokens..\n");
        exit(3);
    }

    // add token to the array
    tokens[token_count++] = temp;

    return;
}

void storeString(tokenType type , char* word){ // allocate memory for new token
    Token* temp = (Token*)malloc(sizeof(Token));

    // set the parameters
    temp->token_type = type;
    strcpy(temp->var_name,word);

    // check if tokens array is full
    if(token_count >= MAX){
        printf("NOT enough space to store tokens..\n");
        exit(3);
    }

    // add token to the array
    tokens[token_count++] = temp;

    return;
}

CharType getCharTypeOf(char c){
    for(int i=0 ; charLookup[i].c != 0 ; i++){
        if(c == charLookup[i].c) return charLookup[i].type;
    }

    return CHAR_UNKNOWN;
}

char* readNext(FILE* fptr){
    char c = fgetc(fptr);

    while(c == ' ' || c == '\n' || c == '\t') c = fgetc(fptr); // skip initial

    static char temp_name[MAX_NAME];
    int temp_name_count = 0;

    CharType current_type = getCharTypeOf(c);

    temp_name[0] = c;
    temp_name[1] = '\0';
    temp_name_count = 1;
    
    if(c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'){
        printf("skipping ->%c<-\n", c);
        return temp_name;
    }

    

    if(c == '\''){ // found starting '
        // read untill next '
        c = fgetc(fptr);
        while(c != EOF && c != '\''){
            temp_name[temp_name_count++] = c;
            c = fgetc(fptr);
        }
        if(c == EOF){
            printf("syntax error..\n");
            exit(4);
        }
        temp_name[temp_name_count++] = c;
        temp_name[temp_name_count] = '\0';
        return temp_name;
    } else if(c == '\"'){ //found starting "
        // read untill next "
        c = fgetc(fptr);
        while(c != EOF && c != '\"'){
            temp_name[temp_name_count++] = c;
            c = fgetc(fptr);
        }
        if(c == EOF){
            printf("syntax error..\n");
            exit(4);
        }
        temp_name[temp_name_count++] = c;
        temp_name[temp_name_count] = '\0';
        
        return temp_name;
    }     
    else {}

    
    bool isEqual = false;

    while(c != EOF && c != '\n' && c != '\t'){
        isEqual = false;
        if(c == '=') isEqual = true;
        c = fgetc(fptr);
        if(current_type == getCharTypeOf(c)){
            if(isEqual && c != '='){
                temp_name[temp_name_count] = '\0';
                ungetc(c , fptr);
                return temp_name;
            }
            temp_name[temp_name_count++] = c;
            // add to word
        } else{
            if(c == '.'){
                temp_name[temp_name_count++] = c;
                continue;
            }
            temp_name[temp_name_count] = '\0';
            ungetc(c , fptr);
            return temp_name;
        }
    }

    return NULL;
}

