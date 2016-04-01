CC = g++
DEBUG = -g -pg
FLAGS = -Wall -std=c++11 $(DEBUG) -fopenmp -O3
INC = -I ./lib/boost_1_60_0
OBJS = Combination.o ColorSet.o

all: ramsey

ramsey: $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

%.o: %.cpp
	$(CC) -c $(INC) $(FLAGS) $< -o $@

# dependencies
ColorSet.o: ColorSet.h Combination.o
Combination.o: Combination.h

clean:
	rm -f ramsey
	rm -f *.o