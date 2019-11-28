documentation: txt_modification.c
	gcc `pkg-config --cflags gtk+-3.0` gui.c scheduler.c schedulability_tests.c -o scheduler `pkg-config --libs gtk+-3.0` -lm

clean:
	$(RM) scheduler
