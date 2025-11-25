#!/bin/bash
#SBATCH --job-name=2-processos-8-threads
#SBATCH --output=teste1.log
#SBATCH --error=teste1.errors

# Carregar MPI se necessário
module load openmpi
echo "Rodando no host " `hostname`

echo "SLURM_JOB_NAME: " $SLURM_JOB_NAME
echo "SLURM_NODELIST: " $SLURM_NODELIST
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

PROGRAM="./knn-mpi"
NQ="-nq=128"
NP="-npp=400000"
D="-d=300"
K="-k=1024"
t="-t=8"
MPIARGS="-np 2"

NTIMES=$1
> teste1.txt
echo "Rodando Experiencia 3 (2 processos, 8 threads) $NTIMES vezes"
for i in $(seq 1 $NTIMES);
do
    echo "Execucao numero $i"
    mpirun $MPIARGS $PROGRAM $NQ $NP $D $K  >> tempo3.txt
done
