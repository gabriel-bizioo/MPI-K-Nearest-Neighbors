#!/bin/bash
# Roda todos os experimentos e registra tabelas

sbatch ./1p1n.sh 10
sbatch ./8p1n.sh 10
sbatch ./8p4n.sh 10

$T1=$(cat "TEMPO_COMPUTA_KNN" tempo1.txt | awk '{print $2}')
$T2=$(cat "TEMPO_COMPUTA_KNN" tempo2.txt | awk '{print $2}')
$T3=$(cat "TEMPO_COMPUTA_KNN" tempo3.txt | awk '{print $2}')

$S2=$(awk "BEGIN print {print $T1 / $T2}")
$S3=$(awk "BEGIN print {print $T1 / $T3}")

echo "Experiencia,Tempo (s),Speedup" > resultados.csv
echo "1 processo 1 host,$T1,1.0" >> resultados.csv
echo "8 processos 1 host,$T2,$S2" >> resultados.csv
echo "8 processos 4 hosts,$T3,$S3" >> resultados.csv

