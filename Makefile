
pi: pi.c
	gcc -g -Wall -o pi pi.c

draft: draft.c
	gcc -g -Wall -o draft draft.c

draft2: draft2.c
	gcc -g -Wall -o draft2 draft2.c
clean:
	$(RM) pi
	$(RM) prueba
