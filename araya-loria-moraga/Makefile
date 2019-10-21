
pi: pi.c
	#gcc -g -Wall -o pi pi.c
	gcc -g -Wall `pkg-config gtk+-3.0 --cflags` pi.c gui.c -o pi `pkg-config gtk+-3.0 --libs` -lm

run: pi
	./pi -f config.txt

clean:
	$(RM) pi
