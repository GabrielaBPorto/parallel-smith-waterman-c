FLAGS = -g -Wall -fopenmp

CXX = gcc

Rm = rm -f

all:
	$(CXX) -o paralelo.exe src/paralelo_smith_waterman.c $(FLAGS)
	$(CXX) -o sequencial.exe src/sequencial_paralelo_smith_waterman.c $(FLAGS)

clean:
	$(RM) -f *.exe
