all: p1 p2

program1: p1.c header.c header.h
	gcc -g p1.c header.c -o p1
	
program2: p2.c header.c header.h
	gcc -g p2.c header.c -o p2
	
clean:
	rm -f *.o a.out p1 p2 core
