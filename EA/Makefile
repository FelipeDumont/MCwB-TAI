PROJECT = EA
CFLAGS = -Wall -g -O3
OBJECTS = Main.o Greedy.o EvoAl.o Initialize.o

all: $(PROJECT)

.cpp.o:
	g++ -c $(CFLAGS) $<

$(PROJECT): $(OBJECTS)
	g++ $(CFLAGS) -o $(PROJECT) $(OBJECTS)
	
Main.o: Main.cpp
	g++ $(CFLAGS) -c Main.cpp
	
Greed.o: Greedy.cpp Greedy.h
	g++ $(CFLAGS) -c Greedy.cpp

GenAl.o: GenAl.cpp GenAl.h
	g++ $(CFLAGS) -c EvoAl.cpp
	
clean:
	rm $(OBJECTS) $(PROJECT)
