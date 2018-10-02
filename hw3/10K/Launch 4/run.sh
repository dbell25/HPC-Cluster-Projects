#!/bin/bash -l
module load OpenMPI
mpicc -Wall -Wextra -O0 find_keys.c -o find_keys
mpirun /homes/danieljbell/cis625/hw3/find_keys
