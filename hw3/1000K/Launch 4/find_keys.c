#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include "linked_list.c"

#define MAX_WORD_LENGTH 10
#define MAX_LINE_LENGTH 2001
#define WIKI_FILE "/homes/danieljbell/cis625/hw3/wiki_dump.txt"
#define KEYWORD_FILE "/homes/danieljbell/cis625/hw3/keywords.txt"
#define OUTPUT_FILE "/homes/danieljbell/cis625/hw3/wiki.out"

FILE *fd;
int nwords, nlines, maxwords = 50000, maxlines = 1000000;
int i, k, n, err, start, end, *count;
double nchars = 0, tstart, ttotal, tlast;
char *word_memory, **word, *line_memory, **line, *temp_memory;
struct Node** chunk_start;
struct Node** chunk_end;
double myclock();
int compare(const void* a, const void* b);

// function prototypes
void filIO(int rank);
void start_clock();
void print_wiki();
void free_memory();

int main(int argc, char * argv[])
{
  int numtasks, rank, len, rc;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Number of tasks= %d, My rank= %d\n", numtasks, rank);
	
  start_clock(); // Sets up timer

  // Allocates space for the word list and lines
  count = (int *) calloc( maxwords, sizeof( int ) );

  // Allocate node pools
  allocateNodePools();

  // Contiguous memory allocation for words
  word_memory = malloc(maxwords * MAX_WORD_LENGTH * sizeof(char));
  word = (char **) malloc( maxwords * sizeof( char * ) );
  for(i = 0; i < maxwords; i++) word[i] = word_memory + i * MAX_WORD_LENGTH;

  // Allocate arrays for the beginning and end of the lists
  chunk_start = (struct Node**) malloc( maxwords * sizeof(struct Node *) );
  chunk_end = (struct Node**) malloc( maxwords * sizeof(struct Node *) );
  for( i = 0; i < maxwords; i++ ) chunk_start[i] = chunk_end[i] = node_alloc();

  // Contiguous memory
  line_memory = malloc(maxlines * MAX_LINE_LENGTH * sizeof(char));
  line = (char **) malloc( maxlines * sizeof( char * ) );
  for( i = 0; i < maxlines; i++ ) line[i] = line_memory + i * MAX_LINE_LENGTH;

  filIO(rank);
  
  // Broadcast data
  MPI_Bcast(&nwords, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&nlines, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(word_memory, nwords * MAX_WORD_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(line_memory, nlines * MAX_LINE_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD);

  tlast = myclock();

  // Division of work
  start = rank * (nwords/numtasks);
  end = (rank + 1) * (nwords/numtasks);
  if(rank == numtasks - 1) end = nwords;

  printf("\nChunk Number: %d, Start: %d, End: %d\n", rank, start, end);

  // Loop over the word list
  for( k = 0; k < nlines; k++ ) {
    for( i = start; i < end; i++ ) {
      if( strstr( line[k], word[i] ) != NULL ) {
        count[i]++;
        chunk_end[i] = add(chunk_end[i], k);
      }
    }
  }

  printf("\nRank %d Runtime: %lf seconds", rank, myclock() - tlast); 
  fflush(stdout);
  print_wiki();

  MPI_Barrier(MPI_COMM_WORLD);

  if(rank == 0)
  {
    ttotal = myclock() - tstart;
	printf("\n\nDATA: Total Runtime: %.4f", ttotal);
  }

  free_memory();
  MPI_Finalize();
  return 0;
}

/**************************************************************************/

void filIO(int rank){

// Read in the keywords and sorts them alphabetically

  if(rank == 0)
  {
    fd = fopen( KEYWORD_FILE, "r" );
    nwords = -1;
    do {
      err = fscanf( fd, "%[^\n]\n", word[++nwords] );
    } while( err != EOF && nwords < maxwords );
    fclose( fd );

    printf( "Read in %d words\n", nwords);

    qsort(word, nwords, sizeof(char *), compare);
    temp_memory = malloc(maxwords * MAX_WORD_LENGTH * sizeof(char));
    for(i = 0; i < maxwords; i++){
      for(k = 0; k < MAX_WORD_LENGTH; k++){
        *(temp_memory + i * MAX_WORD_LENGTH + k) = word[i][k];
      }
      word[i] = word_memory + i * MAX_WORD_LENGTH;
    }
    memcpy(word_memory, temp_memory, maxwords * MAX_WORD_LENGTH);
    free(temp_memory); temp_memory = NULL;
  }
  
// Read in the lines from the data file

  if(rank == 0)
  {
    char *input_file = (char*)malloc(500 * sizeof(char));
	sprintf(input_file, WIKI_FILE);
    fd = fopen( input_file, "r" );
    nlines = -1;
    do {
      err = fscanf( fd, "%[^\n]\n", line[++nlines] );
      if( line[nlines] != NULL ) nchars += (double) strlen( line[nlines] );
    } while( err != EOF && nlines < maxlines);
    fclose( fd );
    free(input_file); input_file = NULL;

    printf( "Read in %d lines averaging %.0lf chars/line\n", nlines, nchars / nlines);
  }
}

/**************************************************************************/

void print_wiki(){
  // Prints to the wiki.out file

  char *output_file = (char*) malloc(500 * sizeof(char));
  //sprintf(output_file, OUTPUT_FILE, argv[1], rank);
  sprintf(output_file, OUTPUT_FILE);
  fd = fopen( output_file, "w" );
    for( i = start; i < end; i++ ) {
      if(count[i] != 0){
        fprintf( fd, "%s: ", word[i] );
        int *line_numbers;
        int len;
        // this function mallocs for line_numbers
        toArray(chunk_start[i], &line_numbers, &len);

            for (k = 0; k < len - 1; k++) {
              fprintf( fd, "%d, ", line_numbers[k]);
            }
            fprintf( fd, "%d\n", line_numbers[len - 1]);

        // so we free it
        free(line_numbers);  line_numbers = NULL;
      }
    }
  fclose( fd );
  free(output_file);  output_file = NULL;	  
}

/**************************************************************************/

void start_clock(){
  tstart = myclock();  // Set the zero time
  tstart = myclock();  // Start the clock
  tlast = tstart;		
}

/**************************************************************************/

int compare(const void* a, const void* b){
  const char **ia = (const char **)a;
  const char **ib = (const char **)b;
  return strcmp(*ia, *ib);
}

/**************************************************************************/

double myclock() {
  static time_t t_start = 0;  
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  if( t_start == 0 ) t_start = ts.tv_sec;

  return (double) (ts.tv_sec - t_start) + ts.tv_nsec * 1.0e-9;
}

/**************************************************************************/

void free_memory(){
  destroyNodePools();
  free(chunk_start); chunk_start = NULL;
  free(chunk_end); chunk_end = NULL;
  free(word); word = NULL;
  free(word_memory); word_memory = NULL;
  free(line); line = NULL;
  free(line_memory); line_memory = NULL;
}

/**************************************************************************/
