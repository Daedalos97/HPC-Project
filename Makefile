EXENAME = percolation
CPP11 = g++ -std=c++11
CPPFLAGS = -Wall -Werror -pedantic
LIBFLAGS = -fopenmp
LIB_LAT = lattice
OBJ = $(LIB_LAT).o
RM = /bin/rm

main : $(OBJ)
	$(CPP11) $(CPPFLAGS) -o $(EXENAME) $(LIB_LAT).o main.cpp $(LIBFLAGS)

$(OBJ) : $(LIB_LAT).cpp $(LIB_LAT).h
	$(CPP11) $(CPPFLAGS) -c $(LIB_LAT).cpp $(LIBFLAGS)

clean:
	$(RM) -f $(EXENAME) *.o 
