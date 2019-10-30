#include <stdio.h>

#include "parser.h"
void mypass(void);

FILE *source, *object;

int main (int argc, char *argv[]){
    source = stdin;
    object = stdout;
    
    lookahead = gettoken(source);
    
    //printf("\nThe returns is %d.\n", isFLOAT(source));
    //fprintf(object, "pattern classified as %d\n", isNum(source));
    //fprintf(object, "the token number is %i\n", token);
    //fprintf(object, "next token number is %i\n", gettoken(source));
    
    mybc();
    //expr();
    
    fprintf(object, "\n");
    
    return 0;
    
}
