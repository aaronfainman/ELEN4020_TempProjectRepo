

/* TO Compile:
     mpicxx -g -Wall -o program main.cpp
To Run:
    mpiexec -n 4 ./program
*/

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int LOWER_BOUND = 1;
const int UPPER_BOUND = 10;
const double STEP_WIDTH = 0.000001;

int main(void)	{

    long double local_sum = 0;
    auto comm_sz = 0;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(comm_sz == 1) throw(std::invalid_argument("2 or more slots required"));

    double interval_size = (UPPER_BOUND - LOWER_BOUND)/(comm_sz - 1);
    double local_lower = LOWER_BOUND + (my_rank-1)*interval_size;
    double local_upper = local_lower + interval_size;


    if(my_rank!=0){
       for(double i=local_lower; i<local_upper; i+=STEP_WIDTH)  local_sum += sin(i)*sin(i)/(i*i);

        MPI_Send(&local_sum, 1, MPI_LONG_DOUBLE, 0,0, MPI_COMM_WORLD);
    }
    else {
        long double local_sum = double{0};
        long double total_sum = 0;
        
        for(int q=1; q<comm_sz; q++){
            MPI_Recv(&local_sum, 1, MPI_LONG_DOUBLE, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += local_sum;
        }

        cout << "total sum: " << total_sum << endl;

        cout << "integral: " << total_sum*STEP_WIDTH << endl;;
    }
    MPI_Finalize();
    return 0;
}