all: deck.o display.o draw.o play.o main.o
	gcc -o tres deck.o display.o draw.o play.o main.o

main.o: main.c main.h play.h display.h draw.h deck.h headers.h
	gcc -c main.c

play.o: play.c play.h display.h draw.h deck.h headers.h
	gcc -c play.c

draw.o: draw.c draw.h display.h deck.h headers.h
	gcc -c draw.c

display.o: display.c display.h deck.h headers.h
	gcc -c display.c

deck.o: deck.c deck.h headers.h
	gcc -c deck.c

run:
	./tres

clean:
	rm *.o
	rm *~
