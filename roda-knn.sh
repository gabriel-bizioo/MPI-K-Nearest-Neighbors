#!/bin/bash
#SBATCH --job-name=experimentos-knn-mpi
#SBATCH --output=resultados-knn.log
#SBATCH --error=erros-knn.log
#SBATCH --hodes=4
#SBATCH --ntasks-per-node=2
#SBATCH --exclusive

# Carregar MPI se necessário
module load openmpi
echo "Rodando no host " `hostname`

echo "SLURM_JOB_NAME: "	$SLURM_JOB_NAME	
echo "SLURM_NODELIST: " $SLURM_NODELIST 
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

PROGRAM="./knn-mpi"
NQ="nq=128"
NP="npp=400000"
D="d=300"
K="k=1024"

echo "Experiência 1 (1 processo)"
mpirun -np 1 $PROGRAM $NQ $NP $D $K  > tempo1.txt
T1=$(grep "TEMPO_COMPUTA_KNN" tempo1.txt | awk '{print $2}')

echo "Experiência 2 (8 processos, mesmo host)"
mpirun --oversubscribe --usehwthread-cpus -np 8 -N 8 $PROGRAM $NQ $NP $D $K > tempo2.txt
T2=$(grep "TEMPO_COMPUTA_KNN" tempo2.txt | awk '{print $2}')

echo "Experiência 3 (8 processos em 4 hosts, 2 por host)"
mpirun -np 8 --map-by ppr:2:node $PROGRAM $NQ $NP $D $K > tempo3.txt
T3=$(grep "TEMPO_COMPUTA_KNN" tempo3.txt | awk '{print $2}')

S2=$(awk "BEGIN {print $T1 / $T2}")
S3=$(awk "BEGIN {print $T1 / $T3}")

echo "Experiencia,Tempo (s),Speedup" > resultados.csv
echo "1 processo 1 host,$T1,1.0" >> resultados.csv
echo "8 processos 1 host,$T2,$S2" >> resultados.csv
echo "8 processos 4 hosts,$T3,$S3" >> resultados.csv

echo "O tempo total foi de " $SECONDS " segundos"
