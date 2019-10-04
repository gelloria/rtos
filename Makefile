
pi: pi.c
	gcc -g -Wall -o pi pi.c

clean:
	$(RM) pi
	$(RM) prueba
