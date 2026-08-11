objects = machine.o simulation.o dynamic_tape.o

simulation:$(objects)
	gcc -o simulation $(objects)

simulation.o: simulation.c machine.h
	gcc -c simulation.c

machine.o: machine.c machine.h
	gcc -c machine.c

tape.o: dynamic_tape.c dynamic_tape.h
	gcc -c dynamic_tape.c


clean:
	rm -f *.o simulation