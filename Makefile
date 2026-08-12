objects = machine.o simulation.o dynamic_tape.o csv_parser.o

simulation:$(objects)
	gcc -o simulation $(objects)

simulation.o: simulation.c machine.h
	gcc -c simulation.c

machine.o: machine.c machine.h
	gcc -c machine.c

tape.o: dynamic_tape.c dynamic_tape.h
	gcc -c dynamic_tape.c

csv_parser.o: csv_parser.c csv_parser.h
	gcc -c csv_parser.c
	
clean:
	rm -f *.o simulation