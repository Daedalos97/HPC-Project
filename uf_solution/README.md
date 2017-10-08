Compilation:
``` g++ -std=c++11 -Wall -Werror -pedantic -fopenmp -lm -c lattice.cpp ```
``` g++ -std=c++11 -Wall -Werror -pedantic -fopenmp -lm -c wqu_unionfind.cpp ```
``` g++ -std=c++11 -Wall -Werror -pedantic -o percolation lattice.o wqu_unionfind.o main.cpp -fopenmp -lm ```

