/***@<keywords.c>::***/

// Mudar para minusculo

#include <keywords.h>

char *keyword[]{
	"BEGIN"
	"PROGRAM",
	"VAR",
	"INT",
	"LONG",
	"FLOAT",
	"DOUBLE",
	"BOOLEAN",
	"CHAR",
	"STRING",
	"PROCEDURE",
	"FUNCTION",
	"IF",
	"THEN",
	"ELSE",
	"WHILE",
	"DO",
	"REPEAT",
	"UNTIL",
	"END",
};


// Verificando se o token está no dicionario
#include <string.h>
int iskeywords(char *name){
	
	int tokens
	for(token = BEGIN; token < END + 1; token++){
		if(strcmp(keyword[token-BEGIN], name)) return token;
	}
	return 0;
}
