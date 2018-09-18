#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ARRAY_SIZE 2000000
#define STRING_SIZE 16

char char_array[ARRAY_SIZE][STRING_SIZE];
int char_counts[26];

char getRandomChar()
{
	int randNum = 0;
	char randChar = ' ';

	randNum = 26 * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
	randNum = randNum + 97;				// scale to 'a'
	randChar = (char) randNum;

	// printf("%c", randChar);
	return randChar;
}

void init_arrays()
{
  int i, j, randNum; 
  char randChar;

  for ( i = 0; i < ARRAY_SIZE; i++) {
	for ( j = 0; j < STRING_SIZE; j++ ) {
		 char_array[i][j] = getRandomChar();
	}
  }

  for ( i = 0; i < 26; i++ ) {
  	char_counts[i] = 0;
  }
}

void count_array()
{
  char theChar;
  int i, j, charLoc;

  for ( i = 0; i < ARRAY_SIZE; i+=8) {
	for ( j = 0; j < STRING_SIZE; j+=8 ) {
	     //iteration1
		 theChar = char_array[i][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration2
		 theChar = char_array[i+1][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+1][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration3
		 theChar = char_array[i+2][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+2][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;

		 //iteration4
		 theChar = char_array[i+3][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+3][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration5
		 theChar = char_array[i+4][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+4][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration6
		 theChar = char_array[i+5][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+5][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration7
		 theChar = char_array[i+6][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+6][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
		 
		 //iteration8
		 theChar = char_array[i+7][j];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+1];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+2];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+3];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+4];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+5];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+6];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	     theChar = char_array[i+7][j+7];
		 charLoc = ((int) theChar) - 97;
		 char_counts[charLoc]++;
	}
  }
}

void print_results()
{
  int i, total = 0;

  for ( i = 0; i < 26; i++ ) {
     total += char_counts[i];
     printf(" %c %d\n", (char) (i + 97), char_counts[i]);
  }
  printf("\nTotal characters:  %d\n", total);
}

main() {
	init_arrays();
	int i, j=0;
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;
	gettimeofday(&t1, NULL);
	count_array();
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	print_results();
}

