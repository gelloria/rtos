
pi: pi.c
	#gcc -g -Wall -o pi pi.c
	gcc -g -Wall `pkg-config gtk+-3.0 --cflags` Thread_LS_Gabriel.c gui.c -o pi `pkg-config gtk+-3.0 --libs` -lm

draft: draft.c
	gcc -g -Wall -o draft draft.c

draft2: draft2.c
	gcc -g -Wall -o draft2 draft2.c
clean:
	$(RM) pi
	$(RM) prueba
