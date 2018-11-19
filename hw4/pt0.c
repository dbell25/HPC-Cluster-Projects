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
#define rectangle_size 0.00001
#define pi 3.14159265

int x_start, y_start, x_end, y_end;
int nodes, cores, threads;

int get_start(int rank) {
  int block_size, remainder, base_start, start;
  
  block_size = (x_end - x_start) / (nodes * threads); // Determines block size
  remainder = (x_end - x_start) % (nodes * threads); // Calculates remainder
  base_start = rank * block_size; // Default start with no remainders
  start = ((remainder == 0)
    ? (base_start) : ((rank < remainder) 
	? (base_start + rank) : (base_start + remainder))); // Offsets the start postion based on remainders   
  return start;
}

int get_end(int rank, int start) {
  int block_size, remainder, end;
  
  block_size = (x_end - x_start) / (nodes * threads); // Gets block size
  remainder = (x_end - x_start) % (nodes * threads); // Calculates remainder
  end = ((remainder == 0 || rank >= remainder)
    ? (start + block_size) : (start + block_size + 1)); // Calculates ending position from start postion and remainders
  return end;
}

double function(double x, double y)
{
  return -cos(x)*sin(y)*exp(-((x - pi)*(x - pi) + (y - pi)*(y - pi)));
}

int main(int argc, char *argv[]) {
  struct timeval t1, t2;
  double elapsed_time;
  gettimeofday(&t1, NULL); // Gets the start time
  
  int i, j, thread_id, thread_start, thread_end, rc, nnodes, rank;
  double size, local_volume, global_volume, *local_volumes;
	MPI_Status status;

  // MPI Start
  rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
  }

  // Gets MPI Values
  MPI_Comm_size(MPI_COMM_WORLD, &nnodes); 
  nodes = nnodes;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  cores = atoi(argv[1]); // Save number of cores
  threads = atoi(argv[2]); // Save number of threads
  omp_set_num_threads(threads);
  x_start = atoi(argv[3]); // Save starting x value
  y_start = atoi(argv[4]); // Save starting y value
  x_end = atoi(argv[5]); // Save ending x value
  y_end = atoi(argv[6]); // Save ending y value
  
  local_volumes = malloc(threads * sizeof(double)); // allocates memory for the array
  #pragma omp parallel private(i, j, thread_start, thread_end, local_volume)
  {
    thread_id = omp_get_thread_num();
    thread_start = get_start((rank * threads) + thread_id);
    thread_end = get_end((rank * threads) + thread_id, thread_start);
    
    for (i = thread_start; i < thread_end; i++)
    {
      for (j = y_start; j < y_end; j++)
        local_volume += rectangle_size * rectangle_size * function(x_start + i*rectangle_size, y_start + j*rectangle_size);
    }
    local_volumes[thread_id] = local_volume; // Saves volume to the volume array
  }
  
  // Collects the values from the threaded calculations
  local_volume = local_volumes[0];
  for (i = 1; i < threads; i++)
    local_volume += local_volumes[i];
 
  free(local_volumes); // frees local volume memory
  
  MPI_Reduce(&local_volume, &global_volume, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
  {
    gettimeofday(&t2, NULL); // Gets the final time
    elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("DATA, pt0, %d, %d, %d, %f, %E", nodes, cores, threads, elapsed_time, global_volume);
  }
  
  MPI_Finalize();
  return 0;
}