/***@<parser.h>::***/

void mypass(void);
void declscope(void);
void stmblock(void);

void varlst(void);
void vartype(void);
void procdecl(void);
void stmlst(void);
void parmdef(void);

void match(token_t expected);

extern token_t lookahead;


