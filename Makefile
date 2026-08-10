objects = machine.o simulation.o 

simulation:$(objects)
	gcc -o simulation $(objects)

simulation.o: simulation.c machine.h
	gcc -c simulation.c

machine.o: machine.c machine.h
	gcc -c machine.c

clean:
	rm -f *.o simulation