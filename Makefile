all:
	gcc -Wall -g main.c -o readme

install: all
	cp readme ~/.local/bin/

