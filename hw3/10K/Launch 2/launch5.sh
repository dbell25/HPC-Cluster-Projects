#!/bin/bash -l
##$ -l h_rt=0:02:00

for j in 1 2 3 4 5
  do
     sbatch --constraint=elves --partition=ksu-gen-reserved.q,batch.q --ntasks-per-node=16 --nodes=$j run.sh
done
