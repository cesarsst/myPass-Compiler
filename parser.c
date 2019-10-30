/***@<parser.c>::***/

/*******************************************************************************
 * Author: Prof. Eraldo Pereira Marinho, Ph.D.
 * Local standard date: ter out 29 09:38:03 -03 2019
 ******************************************************************************/

/*******************************************************************************
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID (input ',' output) ';' ] declscope stmblock '.'
 input -> ID
 output -> ID
 input, output: standard buffer handlers



// Pode ser definido no lexer


RELOP = '<' | NEQ | LEQ | '=' | GEQ | '>' "
NEQ = "<>"
LEQ = "<="
GEQ = ">="

smpexpr -> ['+'|'-'] term { OPLUS term }
OPLUS = " + | - " | OR
term -> fact { OTIMES fact }
OTIMES = " * | / " | DIV | MOD | AND
NUM = UINT | FLT
CHR = \'[0x00-0xFF]\' (ASCII)
STR = \"CHR*\"
ID = [A-Za-z][A-Za-z0-9_]*

Notes:: 
 1. low case variable means nonterminals whereas upper case means tokens, or
 terminal symbols. Of course, explicit ASCII characters are standard terminals;
 2. this is not Pascal, neither C.
*******************************************************************************/

/**** Atividade:
 * 1. Com base na sintaxe acima exposta, implemente um analisador sintático (parser)
 * para cada uma das produções. Observe que os tokens serão tratados no nível léxico
 * mas esses podem ser, em alguns casos, tratados diretamente no parser, na forma
 * de funções predicado.
 *
 * 2. Aumente o analisador léxico para os tokens que sejam mais bem definidos
 * na parte léxica do pré-compilador.
 ****/

/*
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID (input ',' output) ';' ] declscope stmblock '.'
 input -> ID
 output -> ID
 input, output: standard buffer handlers
 */

#include <tokens.h>
#include <lexer.h>
#include <keywords.h>
#include <parser.h>


token_t lookahead;
char lexeme[MAXIDLEN + 1];
 
void mypass(void){	
 	
	 if (lookahead == PROGRAM) {
		match(PROGRAM); match(ID);
		match('('); match(ID); match(';'); match(ID); match(')');
		match(';');
	 }
	 
	declscope();
	stmblock();
 }


/****************************************************************
declscope -> { VAR varlst ':' vartype ';' } procdecl
***************************************************************/
void declscope(void){
	
	while(lookahead == VAR){
		match(VAR);
		varlst();
		match(':');
		vartype();
		match(';');
	}
	procdecl();
	
};

/****************************************************************
varlst -> ID { ',' ID }
***************************************************************/
void varlst(void){
	
	_varlst:
		match(ID);
		if(lookahead == ','){
			match(','); goto _varlst
		}
	
};


/****************************************************************
vartype -> INT | LONG | FLOAT | DOUBLE | BOOLEAN | CHAR | STRING
***************************************************************/
void vartype(void){
	
	switch(lookahead){
		case INT:
			match(INT);
			break;
		case LONG:
			match(LONG);
			break;
		case FLOAT:
			match(FLOAT);
			break;
		case DOUBLE:
			match(DOUBLE);
			break;
		case BOOLEAN:
			match(BOOLEAN);
			break;
		case CHAR:
			match(CHAR);
			break;
		default:
			match(STRING);
	}
	
};

/****************************************************************
parmdef -> [ '(' [VAR] varlst ':" vartype {';' [VAR] varlst ':" vartype } ')']
***************************************************************/
void parmdef(void){
	
	if(lookahead == '('){
		match('(');
		
		_parmdef:
		if(lookahead == VAR){
			match(var);
		}
		varlst();
		match(':');
		vartype();
		if(lookahead == ';'){
			match(';'); goto _parmdef;
		}
		mtach(')');
	}
	
};

/****************************************************************
procdecl -> { PROCEDURE ID parmdef ';' declscope stmblock |
              FUNCTION  ID parmdef ':' vartype ';' declscope stmblock }
***************************************************************/
void procdecl(void){
	
	int isfunc;
	while(lookahead == PROCEDURE || (isfunc = lookahead == FUNCTION)){

		match(lookahead);
		match(ID);
		pardef();
		if(isfunc){
			match(':'); vartype();			
		}
		match(';');
		declscope();
		stmblock();
		
	}
	
};

/****************************************************************
stmblock -> BEGIN stmtlst END
***************************************************************/
void stmblock(void){
	
	match(BEGIN);
	stmtlst();
	match(END);
	
};

/****************************************************************
stmlst -> stmt { ';' stmt }
***************************************************************/
void stmlst(void){
	
	_stmt:
		stmt();
		if(lookahead == ';'){
			match(';'); goto _stmt;
		}
};

/*******************************************************************************************
stmt -> stmblock | ifstm | whilestm | repstm | fact
*******************************************************************************************/
void stmt(void){
	switch(lookahead){
		case BEGIN:
			stmblock();
			break;
		case IF:
			ifstm();
			break
		case WHILE:
			whilestm();
			break;
		case REPEAT:
			repstm();
			break;
		default:
			fact();
	}
}

/*******************************************************************************************
ifstm -> IF expr THEN stmt [ ELSE stmt ]
*******************************************************************************************/
void ifstm(void){
	match(IF);
	expr();
	match(THEN);
	stmt()
	if(lookahead == ELSE){
		stmt();
	}
}

/*******************************************************************************************
whilestm -> WHILE expr DO stmt
*******************************************************************************************/
void whilestm(void){
	match(WHILE);
	expr();
	match(DO);
	stmt();
}


/*******************************************************************************************
repstm -> REPEAT stmlst UNTIL expr
*******************************************************************************************/
void repstm(void){
	match(REPEAT);
	stmlst();
	match(UNTIL);
	expr();	
}


/*******************************************************************************************
fact ->   '(' expr ')'
	| NUM
	| CHR
	| STR
	| TRUE
	| FALSE
	| ID [ ":=" expr ]
*******************************************************************************************/
void fact(void){
	
	switch(lookahead){
		case '(':
			match('('); expr(); match(')');
		case NUM:
			match(NUM);
		case CHR:
			match(CHR);
		case STR:
			match(STR);
		case TRUE:
			match(TRUE);
		case FALSE:
			match(FALSE);
		default:
			match(ID);
			if(lookahead == ':'){
				match(':'); match('='); expr();
			}	
	}
	
	
}


/*******************************************************************************************
expr -> smpexpr [ RELOP smpexpr ]
*******************************************************************************************/
void expr(void){
	smpexpr();
	
	token_t retToken = isRELOP(source);
	if(retToken){
		match(retToken);
		smpexpr();
	}
	
}

/*******************************************************************************************
MATCH
*******************************************************************************************/
void match(int expected){
    if (lookahead == expected){
        lookahead = gettoken(source);
    }else{
        fprintf(stderr,
                "%d seen while %d was expected\n", lookahead, expected);
        exit(-2);
    }
}
