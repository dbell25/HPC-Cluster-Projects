#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <omp.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

#define NUM_THREADS 16

double global_sum = 0;
const int NUM_ITER = 800000000;

typedef struct {
	uint32_t virtualMem;
	uint32_t physicalMem;
} processMem_t;

int parseLine(char *line) {
	int i = strlen(line);
	const char *p = line;
	while (*p < '0' || *p > '9') p++;
	line[i - 3] = '\0';
	i = atoi(p);
	return i;
}

void GetProcessMemory(processMem_t* processMem) {
	FILE *file = fopen("/proc/self/status", "r");
	char line[128];

	while (fgets(line, 128, file) != NULL) {
		//printf("%s", line);
		if (strncmp(line, "VmSize:", 7) == 0) {
			processMem->virtualMem = parseLine(line);
		}

		if (strncmp(line, "VmRSS:", 6) == 0) {
			processMem->physicalMem = parseLine(line);
		}
	}
	fclose(file);
}

void work(int myID) {
	
	double st = 1.0/((double) NUM_ITER);
	int startPos = myID * (NUM_ITER / NUM_THREADS); 
	int endPos = startPos + (NUM_ITER / NUM_THREADS);
	int i;
	double x = 0.0;
	double local_sum = 0.0;
	printf("DEBUG: myID = %d, startPos = %d, endPos = %d \n", myID, startPos, endPos);
	
	#pragma omp private( myID, local_sum, startPos, endPos, i, x, st)
	for(i = startPos; i < endPos; i++)
	{
	  x = (i + 0.25) * st;
	  local_sum += 4.0/(x*x+1);
	}
	
	#pragma omp critical
	{
		global_sum += local_sum;
	}
}

int main() {
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;
	
	omp_set_num_threads(NUM_THREADS);
	
	gettimeofday(&t1, NULL);
	#pragma omp parallel
	{
		work(omp_get_thread_num());
	}
	gettimeofday(&t2, NULL);
	
	processMem_t myMem;
	GetProcessMemory(&myMem);
	
	printf("DEBUG: Global_Sum %lf\n", global_sum);
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	printf("MEMORY, Virtual (KB), %u, Physical (KB), %u \n", myMem.virtualMem, myMem.physicalMem);
	return 0;
}
