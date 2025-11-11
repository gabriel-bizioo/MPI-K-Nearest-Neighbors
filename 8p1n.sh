#!/bin/bash
#SBATCH --job-name=8-processos-1-host
#SBATCH --output=knn-8p1n.log
#SBATCH --error=knn-8p1n.errors
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=8
#SBATCH --exclusive

# Carregar MPI se necessário
module load openmpi
echo "Rodando no host " `hostname`

echo "SLURM_JOB_NAME: " $SLURM_JOB_NAME 
echo "SLURM_NODELIST: " $SLURM_NODELIST 
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

PROGRAM="./knn-mpi"
NQ="nq=128"
NP="npp=400000"
D="d=300"
K="k=1024"

NTIMES=$1
> tempo2.txt
echo "Rodando Experiencia 2 (8 processos, 1 host) $NTIMES vezes"
for i in $(seq 1 $NTIMES);
do
    echo "Execucao numero $i"
    mpirun $PROGRAM $NQ $NP $D $K  >> tempo2.txt
done

