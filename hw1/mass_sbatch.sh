#!/bin/bash
#$SBATCH --partition=ksu-gen-reserved.q,batch.q

for i in 1 2 4 8 16
do
	 sbatch --constraint=elves --ntasks-per-node=$i --nodes=1 openmp_sbatch.sh
done
