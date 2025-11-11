# Calculo de K-Nearest-Neighbors em Cluster MPI

# Objetivo
Usar o problema dos 'K nearest neighbors' para testar o resultado do Cluster
XEON com diferentes nodos usando MPI

## K-Nearest-Neighbors
Dado um Vetor Q de 128 e outro vetor P de 400000 pontos de 300 dimensoes,
devemos retornar, para cada ponto de em Q, os 1024 pontos mas próximos no vetor P
(vizinhos mais proximos -> nearest neighbors)

## Max-Heap
Para calcular os vizinhos, foi usado uma Heap de valor máximo, de forma que
valores menores dos previamente obtidos são inseridos na heap, e está sempre
os n vizinhos mais próximos, sendo n o tamanho da heap (no caso do nosso problema, 1024)

# MPI
O prograrma utiliza o protocolo MPI afim de implementar o algoritmo de forma
a rodar paralelamente em multiplas máquinas de um cluster computacional.
Foram usadas as operações MPI_BCast para o envio do vetor P e MPI_SCatter
para o envio de faixa do vetor Q para as diversas threads.

## Paralelismo
Cada thread processa uma parte do vetor Q, iterando por todos os
elementos de P. Para 4 threads, por exemplo, cada thread processa 32
elementos de Q, e para cada elemento de Q esta deve iterar por todos os
400000 de P para se assegurar que obteve os 1024 elementos mais próximos
de cada um dos 32 pontos.
Nao sao permitidos numeros de threads que gerem tamanhos de faixas 
desequilibrados, ou seja, para t = numero threads, se 128 % t > 0 -> erro.

# Testes
Os resultados foram obtidos através de cluster intel Xeon, de forma que partes
do processo puderam ser ecxecutadas em amquinas diferentes de forma paralela.
Foram feitas 10 rodadas de testes para cada uma das configurações:

1) Processo (thread), rodando em apenas uma host (maquina). Esse teste serve
como base para medir a aceleração ganha ao paralelizar as tarefas

2) processos, rodando em apenas um host. E esperado que rodar em apenas um
host adicione um overhead devido ao compartilhamento de recursos entre as 
threads do processador

3) 8 processos, rodando em 4 hosts, 2 processos por host. Aqui, como os hosts
estao menos saturados, se espera que a performance seja melhor, mesmo que 
exista um overhead de sincronização

# Resultados

## Teste 1:

22.247589
22.250900
22.252647
22.268508
22.230591
22.272807
22.220403
22.265369
22.244418
22.259823

### Total: 222.51
### Media: 22.25

## Teste 2:

11.553767
11.568632
11.529738
12.073475
11.618497
11.535967
11.549467
11.543729
11.591065
11.963668

### Total: 116.528
### Media: 11.65
## Teste 2:

9.639977
9.675023
9.674482
9.663414
9.658094
9.615736
9.654807
9.604667
9.696062
9.645677

### Total: 96.5279
### Media: 9.65


# Conclusoes
Ao analisarmos os resultados obtidos, confirmamos os comportamentos previstos:
vemos uma ganho de 1.9x entre o teste 1 e 2, e 2.3x entre o teste 1 e 3. O 
teste 2 ganha velocidade em relação ao 1 por executar operações em paralelo,
apesar do ganho não ser diretamente proporcional ao numero de threads. O 
teste 3, ao usar apenas 2 processos por host e evitar o compartilhamento de
recursos, ganha um pouco mais de margem em relação ao teste 1 quando comparado
ao teste 2.

# Otimizações
A função inline 'distanciaEuclidiana' foi implementada usando flags do openmp para garantir que o compilador (gcc)
utilize as otimizações de unroll e AVX de forma correta no loop de calculos de distância, o que, junto com as flags de otimização
presentes no 'makefile' acrescentou consideravelmente para o aceler o tempo de execução dos experimentos

# Arquitetura

## Saída de lstopo
Machine (63GB total)
  NUMANode L#0 (P#0 63GB)
  Package L#0
    L2 L#0 (6144KB)
      L1d L#0 (32KB) + L1i L#0 (32KB) + Core L#0 + PU L#0 (P#0)
      L1d L#1 (32KB) + L1i L#1 (32KB) + Core L#1 + PU L#1 (P#2)
    L2 L#1 (6144KB)
      L1d L#2 (32KB) + L1i L#2 (32KB) + Core L#2 + PU L#2 (P#4)
      L1d L#3 (32KB) + L1i L#3 (32KB) + Core L#3 + PU L#3 (P#6)
  Package L#1
    L2 L#2 (6144KB)
      L1d L#4 (32KB) + L1i L#4 (32KB) + Core L#4 + PU L#4 (P#1)
      L1d L#5 (32KB) + L1i L#5 (32KB) + Core L#5 + PU L#5 (P#3)
    L2 L#3 (6144KB)
      L1d L#6 (32KB) + L1i L#6 (32KB) + Core L#6 + PU L#6 (P#5)
      L1d L#7 (32KB) + L1i L#7 (32KB) + Core L#7 + PU L#7 (P#7)
  HostBridge
    PCIBridge
      2 x { PCI 08:00.0-1 (Ethernet) }
    PCIBridge
      PCI 09:00.0 (RAID)
    PCIBridge
      PCI 0a:01.0 (VGA)
    PCI 00:1f.1 (IDE)
      Block "sr0"
    PCI 00:1f.2 (RAID)
      Block "sda"
  Block "sdb"
  Block "sdc"
