/***@<parser.h>::***/
#pragma once
#define MAXIDLEN 256;

typedef int token_t;
extern char lexeme[MAXIDLEN + 1];
extern int isASGN(FILE *tape);

#include <stdio.h>
token_t gettoken(FILE *SOURCE)
