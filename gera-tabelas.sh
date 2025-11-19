#!/bin/bash

T1=$(awk '{sum += $2} END {print sum}' tempo1.txt)
T2=$(awk '{sum += $2} END {print sum}' tempo2.txt)
T3=$(awk '{sum += $2} END {print sum}' tempo3.txt)

S2=$(awk "BEGIN {print $T1 / $T2}")
S3=$(awk "BEGIN {print $T1 / $T3}")

echo "Experiencia,Tempo (s),Speedup" > resultados.csv
echo "1 processo 1 host,$T1,1.0" >> resultados.csv
echo "8 processos 1 host,$T2,$S2" >> resultados.csv
echo "8 processos 4 hosts,$T3,$S3" >> resultados.csv
