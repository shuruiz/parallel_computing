#include <stdio.h>
#include "mpi.h"

int main(int argc, char * argv[]){
    int my_rank, p, n;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    printf("size: %d rank: %d\n", p, my_rank);

    MPI_Finalize();
}



// debug this program with 
// $HOME/opt/usr/local/bin/mpic++ -o test ./MPI_test.cpp
// $HOME/opt/usr/local/bin/mpirun -np 2 ./test
//