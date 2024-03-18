CC=gcc
CPP=g++
OPTS=-g -std=c++20 -Werror

all: main.o predictor.o gshare.o tournament.o custom.o
	$(CPP) $(OPTS) -lm -o predictor main.o predictor.o gshare.o tournament.o custom.o

main.o: main.cpp predictor.h gshare.h tournament.h custom.h
	$(CPP) $(OPTS) -c main.cpp

predictor.o: predictor.cpp predictor.h
	$(CPP) $(OPTS) -c predictor.cpp

gshare.o: gshare.cpp gshare.h predictor.h
	$(CPP) $(OPTS) -c gshare.cpp

tournament.o: tournament.cpp tournament.h predictor.h
	$(CPP) $(OPTS) -c tournament.cpp

custom.o: custom.cpp custom.h predictor.h
	$(CPP) $(OPTS) -c custom.cpp

clean:
	rm -f *.o predictor;
