#include<stdlib.h>
#include<stdio.h>
#include "lexer.h"
#include "utils.h"

extern Token tokens[];
extern int tokenCount;

int main(){
    char *inbuf=loadFile("test/1.q");
    tokenize(inbuf);
    printf("Tokens: \n");
    showTokens();
    puts(inbuf);
    free(inbuf);
    return 0;
}