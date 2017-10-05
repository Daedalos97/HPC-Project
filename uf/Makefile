# A Makefile to build the percolation project.
# author: Abrar Amin (21518928@student.uwa.edu.au)
# author: Samuel Heath (2.....@student.uwa.edu.au)

#-------------------------------------------------
EXE = percolation
CPP11 = g++ -std=c++11
CPPFLAGS = -Wall -Werror -pedantic
LIBFLAGS = -fopenmp -lm
LIB_LAT = lattice
LIB_ALG = wqu_unionfind
OBJ = $(LIB_LAT).o $(LIB_ALG).o
RM = /bin/rm
#-------------------------------------------------

main : $(OBJ)
	$(CPP11) $(CPPFLAGS) -o $(EXE) $(OBJ) main.cpp $(LIBFLAGS)

$(LIB_ALG).o : $(LIB_ALG).cpp $(LIB_ALG).h
	$(CPP11) $(CPPFLAGS) $(LIBFLAGS) -c $(LIB_ALG).cpp

$(LIB_LAT).o : $(LIB_LAT).cpp $(LIB_LAT).h
	$(CPP11) $(CPPFLAGS) $(LIBFLAGS) -c $(LIB_LAT).cpp

clean:
	$(RM) -f $(EXE) $(OBJ)

