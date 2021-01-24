all: program1 program2

program1: program1.c header.c header.h
	gcc -g program1.c header.c -o program1
	
program2: program2.c header.c header.h
	gcc -g program2.c header.c -o program2
	
clean:
	rm -f *.o a.out program1 program2 core
