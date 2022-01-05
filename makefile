CC = gcc
DEPS = alexa.h
CFLAGS = -c -I.
DFLAGS = -lreadline

alexa: main.o alexa.o $(DEPS)
	$(CC) -o alexa main.o alexa.o $(DFLAGS)

main.o: main.c $(DEPS)
	$(CC) $(CFLAGS) main.c

alexa.o: alexa.c $(DEPS)
	$(CC) $(CFLAGS) alexa.c


.PHONY: clean
clean:
	rm -rf alexa *.o