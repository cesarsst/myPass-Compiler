/*
 * Módulo para implementação de métodos de varredura léxica
 */

/*
 * Bloco léxico irá ignorar espaços em branco e comentários, além de detectar os erros léxicos.
 * O processo léxico também pode ser visto como o ato de agrupar letras em palavras e determinar suas localizações no dicionário. Para isso quebra a cadeia de caracteres dada separando as palavras que nela estão representadas.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <keywords.h>
#include "tokens.h"
#include "lexer.h"
#define HIST_SIZE 10

//lexeme é parceiro de lookahead, parser vai usar lexeme para trabalhar com a tabela de símbolos
char lexeme[MAXIDLEN+1];

int linenumber = 1;

void skipspaces(FILE *tape){
    int head;
    
    while(isspace(head = getc(tape))){
    	if(head == '/n') linenumber++;	// Conta a quebra de linhas
	};
    
    ungetc(head,tape);
}

/*
 * ID = [A-Za-z][_A-Za-z0-9]*
 */

//versão usando head
/*
int isID(FILE *tape){
    int head;
    
    if(isalpha(head = getc(tape))){
        while(isalnum(head = getc(tape)));
        ungetc(head,tape);
        return ID;
    }
    
    ungetc(head,tape);
    return 0;
}
*/

/*
 //versão usando head[]
 int isID(FILE *tape){
 //array de bytes e nao deslocamento de 8 bytes como era com int
 //ou seja, um array de caracteres
 char head[MAXIDLEN+1];
 //int i = 0;
 
 if(isalpha(head[0] = getc(tape))){
 
 int i;
 for (i = 1;isalnum(head[i] = getc(tape)); (i < MAXIDLEN) && i++);
 //quando chegar no 31 continua lendo mas nao incrementa i
 //while(isalnum(head[i] = getc(tape)))  if (i < MAXIDLEN) && i++;
 //devolve o caractere
 ungetc(head[i],tape);
 //coloca zero o fim da string
 head[i] = 0;
 return ID;
 }
 
 //aqui i já é zero
 ungetc(head[0],tape);
 return 0;
 }
 */

int isID(FILE *tape){
    //agora lexeme é um array de bytes e não deslocamento de 8 bytes como era com int
    
    token_t
    if(isalpha(lexeme[0] = getc(tape))){
        
        int i;
        
        for (i = 1; isalnum(lexeme[i] = getc(tape)) || lexeme[i] == '_'; (i < MAXIDLEN) && i++);
        
        //devolve o caractere
        ungetc(lexeme[i],tape);
        
        //coloca zero no fim da string
        lexeme[i] = 0;
        if(token = iskeywords(lexeme)) return token;
        
        return ID;
    }
    
    ungetc(lexeme[0], tape);
    return 0;
}

int isNum(FILE *tape, int i){
    int token;
    lexeme[i] = getc(tape);
    if (isdigit(lexeme[i])){
        token = UINT;
        if(lexeme[i] == '0'){
            i++;
            lexeme[i] = getc(tape);
        }
        else {
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                i++;
            }
        }
        if (lexeme[i] == '.'){
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                i++;
            }
            ungetc(lexeme[i], tape);
            token = FLT;
            lexeme[i] = 0;
            return token;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return token;
    }else if (lexeme[i] == '.'){
        i++;
        lexeme[i] = getc(tape);
        if (isdigit(lexeme[i])){
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                i++;
            }
            token = FLT;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return token;
        }
        ungetc(lexeme[i], tape);
        ungetc('.', tape);
        return 0;
    }
    ungetc(lexeme[i], tape);
    return 0;
}

int isEE(FILE * tape, int i){
    i++;
    lexeme[i] = getc(tape);
    if (toupper(lexeme[i]) == 'E'){
        i++;
        lexeme[i] = getc(tape);
        if ((lexeme[i] == '+') || (lexeme[i] == '-')){
            i++;
        }else{
            ungetc(lexeme[i], tape);
        }
        lexeme[i] = getc(tape);
        if (isdigit(lexeme[i])){
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                i++;
            }
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return FLT;
        }
        for ( ; i >= 0 ; i--){
            ungetc(lexeme[i], tape);
        }
    }
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

int isFLOAT(FILE * tape){
    int num = 0, ee = 0, i = 0;
    num = isNum(tape, i);
    if (num == 0){
        //printf("NaN \n");
        return 0;
    }
    if((ee = isEE(tape, i)) > 0){
        //printf("EE \n");
        return ee;
    }
    //printf("UINT \n");
    return num;
}

int isASGN(FILE *tape){
    lexeme[0] = getc(tape);
    
    if (lexeme[0] == ':'){
        if ((lexeme[1] = getc(tape)) == '=' ){
            return ASGN;
        }
    }
    ungetc(lexeme[0], tape);
    return 0;
}

//Verificando sinais <= .... >=
token_t isRELOP(FILE *tape){
	
	lexeme[2] = lexeme[1] = 0;
	switch(lexeme[0] = getc(tape)){
		
		case '<':
			switch(lexeme[1] = getc(tape)){
				case '=': return LEQ;
				case '>': return NEQ;				
			}
			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
			
		case '>':
			if((lexeme[1] = getc(tape)) == '=') return GEQ;
			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
	}
		
	ungect(lexeme[0], tape);
	return 0;
	
	
	
}
/*
 * lexer to parser interface: @ gettoken::
 */

int gettoken(FILE * source){
    int token;
    
    /*
     * ignore left spaces
     */
    skipspaces(source);
    
    /*
     * lexical analysers are called hereafter:
     */
    
    if ((token = isID(source))){
        return token;
    }
    if ((token = isFLOAT(source))){
        return token;
    }
    if ((token = isASGN(source))){
        return token;
    }
    if ((token = isRELOP(source))){
        return token;
    }
    /*
     * return default token, say an ASCII value
     */
    
    return getc(source);
}
