#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type of token
typedef enum {
    TK_RESERVED, //Token for symbol
    TK_NUM, //Token for Natural number
    TK_EOF, //Token for end of input, probably '\0'
} TokenKind;

typedef struct Token Token;

// Token Structure
struct Token {
    TokenKind kind; //Type of token 
    Token *next; //Next token inputted
    int val; //If the TokenKind is TK_NUM is number
    char *str; //Input word
};

Token *token; //The current token

//Error function for debuging
void error(char *fmt, ...) {
    va_list ap; //handling variable number of argument
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

//If the token input is what was expected then return true, then return false
bool consume(char op) {
    if(token -> kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }
    token = token->next;
    return true;
}

//If the token is what was expected then goto the next token.
void expect(char op) {
    if(token -> kind != TK_RESERVED || token->str[0] != op) {
        error("it's not '%c'",op);
    }
    token = token -> next;
}

//Checks if the next token is a number or not.
int expect_number() {
    if(token -> kind != TK_NUM) {
        error("It's not a number");
    }

    int val = token->val;
    token = token -> next;

    return val;
}

//Checks if the Token is end of file.
bool at_eof() {
    return token -> kind == TK_EOF;
}

//Create new token and connect it to cur pointer
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok -> kind = kind;
    tok -> str = str;
    cur -> next = tok;
    
    return tok;
}

Token *tokenizer(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p) {
        if(isspace(*p)) {
            p++;
            continue;
        }
        else if(*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }
        else if(isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("Cannot tokenize");

    }

    new_token(TK_EOF, cur, p++);
    return head.next;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        error("Number of argument is not correct");
        return 1;
    }

    token = tokenizer(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("   mov rax, %d\n", expect_number());

    while(!at_eof()) {
        if(consume('+')) {
            printf("  add rax, %d\n", expect_number());
        }
        else if(consume('-')) {
            printf(" sub rax, %d\n", expect_number());
        }
    }

    printf("  ret\n");
    return 0;
}

