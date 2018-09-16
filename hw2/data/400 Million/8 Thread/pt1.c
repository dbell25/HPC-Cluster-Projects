#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

int global_sum = 0;
const int NUM_ITER = 400000000;

typedef struct {
	uint32_t virtualMem;
	uint32_t physicalMem;
} processMem_t;

int parseLine(char *line) {
	// This assumes that a digit will be found and the line ends in " Kb".
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

void work() {
	
	double st = 1.0/((double) NUM_ITER);
	int i;
	double local_sum = 0.0;
	double x = 0.0;
	
	for(i = 0; i < NUM_ITER; i++)
	{
	  x = (i + 0.25) * st;
	  global_sum += 4.0/(x*x+1);
	}
}

int main() {
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;

	gettimeofday(&t1, NULL);
	work();
	gettimeofday(&t2, NULL);
	
	processMem_t myMem;
	GetProcessMemory(&myMem);
	
	//printf("DEBUG: Global_Sum %lf", global_sum);
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f, ", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	printf("MEMORY, Virtual (KB), %u, Physical (KB), %u \n", myMem.virtualMem, myMem.physicalMem);
	return 0;
}
