#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

#define NUM_THREADS 16
pthread_mutex_t mutex_sum;
double global_sum = 0;
const int NUM_ITER = 200000000;

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

void *work(int *myID) {
	
	double st = 1.0/((double) NUM_ITER); 
	int startPos = ((int)myID) * (NUM_ITER / NUM_THREADS); 
	int endPos = startPos + (NUM_ITER / NUM_THREADS); 
	double local_sum = 0.0; 
	double x = 0.0;
	int i = 0; 
	
	printf("DEBUG: myID = %d, startPos = %d, endPos = %d \n", (int)myID, startPos, endPos);
	
	for(i = startPos; i < endPos; i++)
	{
	  x = (i + 0.25) * st;
	  local_sum += 4.0/(x*x+1);
	}
	
	pthread_mutex_lock (&mutex_sum);
	global_sum += local_sum;
	pthread_mutex_unlock (&mutex_sum);

    pthread_exit(NULL);
}

int main() {
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;

	gettimeofday(&t1, NULL);

	//pthread stuff
	
	int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	pthread_mutex_init(&mutex_sum, NULL);
	
		for (i = 0; i < NUM_THREADS; i++ ) {
	      rc = pthread_create(&threads[i], &attr, work, (void *)i);
	      if (rc) {
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	      }
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
	     rc = pthread_join(threads[i], &status);
	     if (rc) {
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}

	//end pthread stuff
	gettimeofday(&t2, NULL);
	
	processMem_t myMem;
	GetProcessMemory(&myMem);
	
	printf("DEBUG: Global_Sum %lf\n", global_sum);
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	printf("MEMORY, Virtual (KB), %u, Physical (KB), %u \n", myMem.virtualMem, myMem.physicalMem);
	pthread_mutex_destroy(&mutex_sum); // dispose of mutex
	pthread_exit(NULL); //kill threads
	return 0;
}
