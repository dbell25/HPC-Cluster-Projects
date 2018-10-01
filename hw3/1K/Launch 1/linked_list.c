#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define max_number_elements 128
#define mem_size 25600
#define max_num_pools 1000

struct Node **_npools;
int _num_node_pools = 0;
int _current_node_count = 0;
int nodes_in_use = 0;
void _allocateNewPool();

struct Node
{
  int element_count;
  int arr[max_number_elements];
  struct Node *next;
};

struct Node* node_alloc()
{
  if(_current_node_count == mem_size) _allocateNewPool();
  nodes_in_use++;
  return &(_npools[_num_node_pools - 1][_current_node_count++]);
};

/*****************************************************************************/

void _allocateNewPool()
{
  _npools[_num_node_pools++] = malloc(mem_size * sizeof(struct Node));
  _current_node_count = 0;
}

/*****************************************************************************/

void allocateNodePools()
{
  _num_node_pools = 0;
  _current_node_count = 0;
  _npools = (struct Node **) malloc(max_num_pools * sizeof(struct Node *));
  _allocateNewPool();
}

/*****************************************************************************/

void destroyNodePools()
{
  int i;
  for(i = 0; i < _num_node_pools; i++)
  {
    free(_npools[i]);
    _npools[i] = NULL;
  }
  free(_npools);
  _npools = NULL;
  _num_node_pools = 0;
  _current_node_count = 0;
  nodes_in_use = 0;
}

/*****************************************************************************/

int getLength(struct Node *n)
{
  int len = 0;
  while(n != NULL)
  {
    len += n->element_count;
    n = n->next;
  }
  return len;
}

/*****************************************************************************/

void toArray(struct Node *n, int **arr, int *length)
{
  int pos = 0;
  *length = getLength(n);
  *arr =  (int *) malloc(*length * sizeof(int));

  while (n != NULL)
  {
    memcpy(*arr + pos, n->arr, n->element_count * sizeof(int));
    pos += n->element_count;
    n = n->next;
  }
}

/*****************************************************************************/

struct Node* add(struct Node *n, int x)
{
  if(n == NULL)
  {
    struct Node* new_head_node = NULL;
    new_head_node = node_alloc();
    return add(new_head_node, x);
  }

  if(n->element_count == max_number_elements)
  {
    struct Node* new_node = NULL;
    new_node = node_alloc();
    n->next = new_node;
    return add(new_node, x);
  }

  n->arr[n->element_count] = x;
  n->element_count += 1;
  return n;
}

/*****************************************************************************/