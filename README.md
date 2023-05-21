CI316 – Programação Paralela - Trabalho Prático 1
Prof. Marco Antonio Zanata Alves – UFPR – Departamento de Informática
Os trabalhos podem ser feitos:
Grupos de 2~3 pessoas para alunos de Graduação (BCC e/ou IBM).
Escolha um dos seguintes algoritmos clássicos abaixo (final do arquivo), ou discuta
com o professor um outro algoritmo que seja interessante (o professor também
pode ter alguma outra ideia).
Faça a implementação sequencial otimizada em linguagem C do algoritmo
escolhido.
Com base nessa implementação sequencial, faça:
• Profile e análise da sua implementação
• Cálculo da complexidade sequencial
• Modelagem PRAM/BSP para estudar formas de paralelizar o código
Utilizando as primitivas Pthreads, OpenMP ou MPI, faça a paralelização do
código escolhido.
Cuidados para efetuar os experimentos:
• Para assegurar a corretude da implementação paralela, deve-se verificar se os
resultados paralelos batem com os sequênciais.
• Crie pelo menos 3 tamanhos de entrada (N, 2x N, 4x N), criando também duas
variações por tamanho (ou seja, 6 entradas no total). Os tamanhos distintos
irão ajudar nos testes de escalabilidade. As duas variações de entrada por
tamanho irão ajudar em testes de algoritmos heurísticos.
• A menor entrada deverá demorar pelo menos 8 segundos para atingir a
solução.
◦ Não crie entradas aleatórias no início do programa, para conseguir testar a
mesma entrada com diferentes números de threads e também evitar
eventuais comparações entre melhor vs. pior caso.
• Execute pelo menos 20x cada versão para obter uma média minimamente
significativa. Ou seja, todo teste, onde mudamos o número de processos ou
tamanho de entrada, devemos executar 20x e obter uma média.
◦ O speedup deverá ser calculado encima da média das execuções.
• Cuidar para não executar em servidores virtualizados ou que contenham
outros usuários (processos ativos) utilizando a mesma máquina. Evite rodar
em servidores do DINF, pois a maioria dessas máquinas estão virtualizadas e
os testes de speedup não serão satisfatórios.
◦ Sugiro utilizar as máquinas dos laboratórios para os testes. Escolhendo um
modelo de máquina e sempre utilizando o mesmo modelo até o final do
trabalho.
• Teste de escalabilidade forte: Manter um tamanho de entrada N qualquer,
e aumentar gradativamente o número de processos. Sugere-se que escolhase um N tal que o tempo de execução seja maior ou igual a 10 segundos.
• Teste de escalabilidade fraca: Aumentar o tamanho da entrada
proporcionalmente com o número de processos. Exemplo: N, 2N, 4N, 8N, 16N.
Atenção, escalar N com o número de PROCESSOS (não de máquinas)
• Popular/preencher as máquinas sempre com o número de threads/processos
igual ao número de processadores físicos (não virtuais).
Com base na implementação sequencial e paralela, prepare uma apresentação de
no máximo 10 minutos (tempo extra poderá ser descontado) com as
seguintes informações:
• Apresentação detalhada do algoritmo, funcionalidades e implementação. (3
slides)
• Estratégia de paralelização, particionamento do código adotado, pontos de
sincronização e concorrência que você solucionou. (1 slide)
• Complexidade sequencial e paralela para N processadores. (1 slide)
• Avaliação de desempenho do código, utilizando as métricas clássicas
(overhead, speedup, eficiência, tempo médio, desvio padrão).
• Cálculo do speedup para 1, 2, 4, 8, 16 e infitos processadores, utilizando as
leis de Amdahl e Gustafson Barsis. A análise de código e debug pode ser feita
utilizando os softwares likwid, perf, gprof, time ou Vtune (intel).
• O speedup teórico e prático devem ser apresentados em formato de gráficos,
para 1, 2, …, N processadores usados. (2 slides, 1 por entrada distinta)
• O gráfico de tempo de execução com desvio padrão podem ser apresentados
em formato gráfico ou tabela. (1 slide)
• O overhead e eficiência apresentados em tabelas separadas (1 por métrica).
(2 slides)
• Análise com visão crítica do código e os resultados obtidos. Indique as fontes
de ganho e queda de desempenho. (1 slide)
O trabalho será avaliado seguindo os seguintes critérios:
• Domínio do tema
(0-sem domínio, 10-muito domínio)
• Clareza na apresentação
(0-obscuro, 10-muito claro)
• Objetivos alcançados quanto a implementação sequencial e paralela.
(0-nenhum objetivo alcançado, 5-apenas sequencial, 10-sequencial e
paralelizado)
• Complexidade sequencial e paralela
(0-sem análise, 5-análise superficial, 10-análise profunda)
• Métricas apresentadas, overhead, speedup, eficiência, tempo médio, desvio
padrão.
(2 pontos por métrica)
• Cálculo do speedup teórico para Amdahl e Gustafson Barsis
(5 pontos por cada métrica)
• Confiabilidade nos resultados
(0-nada confiável/injusto, 10-muito confiáveis/muito justo)
• Tempo de apresentação.
(0- >= 20 minutos, 10- ==10 minutos. Ou seja, -1 por minuto extra)
• Pontualidade na entrega do trabalho
(-0,3 na nota final, por dia de atraso)
Regras Gerais:
A entrega será feita pelo Moodle dividida em três arquivos:
- Relatório em PDF contendo o nome dos participantes do grupo
- Códigos fonte sequencial
- Códigos fonte paralelo (Pthread/OpenMP/MPI)
Casos não tratados no enunciado deverão ser discutidos com o professor.
 Atencão , a cópia do trabalho (plágio), acarretará em nota igual a Zero para
todos os envolvidos.
Smith-Waterman: performs local sequence alignment; that is, for determining similar regions between
two strings of nucleic acid sequences or protein sequences. Instead of looking at the entire sequence,
the Smith–Waterman algorithm compares segments of all possible lengths and optimizes the similarity
measure.
