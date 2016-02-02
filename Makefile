all:
	bison -d parser.y
	flex tokenizer.lex
	gcc -g -c parser.tab.c lex.yy.c 
	gcc -g -c lisp/*.c
	gcc -g -o jmlisp *.o -lfl -ledit

clean:
	rm *.o
	rm parser.tab.h
	rm parser.tab.c
	rm jmlisp
	rm lex.yy.c
