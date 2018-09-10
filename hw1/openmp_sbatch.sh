#!/bin/bash -l
##$ -l h_rt=0:01:00

echo "Hostname: `srun hostname`"
/homes/danieljbell/cis625/hw1/pt0
/homes/danieljbell/cis625/hw1/pt0-tiled
/homes/danieljbell/cis625/hw1/pt0-unrolled