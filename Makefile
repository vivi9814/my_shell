my_shell : my_shell.o
	gcc -o my_shell my_shell.o
	
my_shell.o : my_shell.c
	gcc -c -o my_shell.o my_shell.c

clean :
	rm -rf my_shell.o my_shell
