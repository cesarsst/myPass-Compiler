/***@<keywords.h>::***/

//Pegando as palavras reservadas do mypass
// Futuramente ser� passado um for de BEGIN ate END para verifica��o

enum {
	BEGIN = 16384,
	PROGRAM,
	VAR,
	INT,
	LONG,
	FLOAT,
	DOUBLE,
	BOOLEAN,
	CHAR,
	STRING,
	PROCEDURE,
	FUNCTION,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	END,
};

int iskeyworkd(char *name);
	




