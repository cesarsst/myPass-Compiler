CFLAGS=-g I$(INCLUDE)

INCLUDE=./INCLUDE/

mypass: main.o lexer.o parser.o keywords.o
		$(CC) -o$@ $^
		
clean: $(RM) *.o

mostlyclean: clean
	$(RM) *~ 
