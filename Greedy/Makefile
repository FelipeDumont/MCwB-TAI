PROJECT = Greedy
CFLAGS = -Wall -g -O3
OBJECTS = Main.o Greedy.o Initialize.o

all: $(PROJECT)

.cpp.o:
	g++ -c $(CFLAGS) $<

$(PROJECT): $(OBJECTS)
	g++ $(CFLAGS) -o $(PROJECT) $(OBJECTS)
	
Main.o: Main.cpp
	g++ $(CFLAGS) -c Main.cpp
	
Greed.o: Greedy.cpp Greedy.h
	g++ $(CFLAGS) -c Greedy.cpp
	
clean:
	rm $(OBJECTS) $(PROJECT)
