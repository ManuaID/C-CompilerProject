#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Length of the argument is correct");
        return 1;
    }

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:");
    printf("   mov rax, %d\n", atoi(argv[1]));
    printf("   ret\n");

    return 0;
}