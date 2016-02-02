all:
	bison -d parser.y
	flex tokenizer.lex
	gcc -g -std=gnu99 -c parser.tab.c lex.yy.c 
	gcc -g -std=gnu99 -c lisp/*.c
	gcc -g -std=gnu99 -o jmlisp *.o -lfl -ledit

clean:
	rm *.o
	rm parser.tab.h
	rm parser.tab.c
	rm jmlisp
	rm lex.yy.c
