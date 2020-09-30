CFLAGS=-Wall 
FICHEIROS=connect.c const.c disconnect.c filter.c node.c server.c spawn.c starter.c window.c

install: exemplo
	touch install

exemplo: exemplo
	gcc -o exemplo

exemplo.zip: $(FICHEIROS)
	zip -9 exemplo.zip $(FICHEIROS)
