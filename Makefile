PROJECTNAME = percolation
CPP11 = g++ -std=c++11
CPPFLAGS = -Wall -Werror -pedantic
OMP_FLAGS = -fopenmp
LIB_LAT = lattice
OBJ = $(LIB_LAT).o
RM = /bin/rm

main : $(OBJ)
	$(CPP11) $(CPPFLAGS) -o $(PROJECTNAME) $(LIB_LAT).o main.cpp $(OMP_FLAGS)

$(OBJ) : $(LIB_LAT).cpp $(LIB_LAT).h
	$(CPP11) $(CPPFLAGS) -c $(LIB_LAT).cpp $(OMP_FLAGS)

clean:
	$(RM) -f $(PROJECTNAME) *.o 

rmexe:
	$(RM) -f $(PROJECTNAME)

clobber: clean rmexe
