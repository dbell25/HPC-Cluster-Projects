#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "mpi.h"
#include <omp.h>
#include <math.h>

int x_start, y_start, nodes, cores, threads;

int get_start(int id) {
  int block_size, remainder, base_start, start;
  
  block_size = (y_start - x_start) / (nodes * threads); // Gets the block size
  remainder = (y_start - x_start) % (nodes * threads); // Calculates the remainder
  base_start = id * block_size; // Starting value without remainders
  start = ((remainder == 0)
    ? (base_start) : ((id < remainder) 
	? (base_start + id) : (base_start + remainder))); // Offsets the starting postion based on remainders
  return start;
}

int get_end(int id, int start) {
  int block_size, remainder, end;
  
  block_size = (y_start - x_start) / (nodes * threads); // Calculates block size
  remainder = (y_start - x_start) % (nodes * threads); // Calculates the remainder
  end = ((remainder == 0 || id >= remainder)
    ? (start + block_size) : (start + block_size + 1)); 
  return end;
}

double function(double x)
{
  return cos(x)+(pow(fabs(6.0-x), 2.0/15.0))+2*(pow(fabs(5.0-x), 4.0/35.0));
}

int main(int argc, char *argv[]) {
  struct timeval t1, t2;
  double elapsed_time;
  gettimeofday(&t1, NULL); // Gets the start time
  
  int j, thread_id, thread_start, thread_end, *positions, rc, nnodes, rank;
  double i, result, local_min, global_min, *local_mins;
  MPI_Status status;

  // MPI Start
  rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS) {
	  printf ("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  // Gets the MPI values
  MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
  nodes = nnodes;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  cores = atoi(argv[1]); // Save number of cores
  threads = atoi(argv[2]); // Save number of threads
  omp_set_num_threads(threads); // Set number of threads
  if (rank == 0)
  {
    x_start = atoi(argv[3]); // Save starting x value
    y_start = atoi(argv[4]); // Save ending x value
  }
  
  // Broadcasts start and end x */
  positions = malloc(2 * sizeof(int));
  if (rank == 0)
  {
    positions[0] = x_start;
    positions[1] = y_start;
  }
  MPI_Bcast(&positions[0], 2, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0)
  {
    x_start = positions[0];
    y_start = positions[1];
  }
  
  // Creates thread calculations
  local_mins = malloc(threads * sizeof(double));
  #pragma omp parallel private(i, thread_id, thread_start, thread_end, local_min, result)
  {
    thread_id = omp_get_thread_num();
    thread_start = get_start((rank * threads) + thread_id);
    thread_end = get_end((rank * threads) + thread_id, thread_start);
    
    for (i = 0; i < (thread_end - thread_start); i+=.01)
    {
      result = function(thread_start + i);
      if (i == 0) local_min = result;
      else
      {
        if (result < local_min) local_min = result;
      }
    }
    local_mins[thread_id] = local_min;
  }
  
  // Collects thread calculations
  local_min = local_mins[0];
  for (j = 1; j < threads; j++)
  {
    if (local_mins[j] < local_min) local_min = local_mins[j];
  }
  MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
  {
    gettimeofday(&t2, NULL); // Gets the final time
    elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("DATA, pt1, %d, %d, %d, %f, %E", nodes, cores, threads, elapsed_time, global_min);
  }

  MPI_Finalize();
  return 0;
}