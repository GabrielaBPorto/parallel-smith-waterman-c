#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Aqui temos um algoritmo que realiza um gerador de sequências ruim.

int main(int argc, char const *argv[]) {
  if(argc < 3){
    printf("Foi colocada quantidade %d de argumentos, sendo ela errada para o funcionamento do código. Qualquer dúvida leia o README\n", argc);
    exit(0);
  }
  //Strings over the Alphabet Sigma
  char *sequenciaA, *sequenciaB;
  //Definição das pontuações
  int matchScore = 5;
  int missmatchScore = -3;
  int gapScore = -4;
  //Definição do tamanho das seqencias
  long long int sizeFirstSequence, sizeSecondSequence;
  sizeFirstSequence = strtoll(argv[1], NULL, 10);
  sizeSecondSequence = strtoll(argv[2], NULL, 10);

  //Alocação de espaço para as duas sequências de entrada
  sequenciaA = malloc(sizeFirstSequence * sizeof(char)); // Para a Linha
  sequenciaB = malloc(sizeSecondSequence * sizeof(char)); // Para a Coluna. No entanto vetores que inicializam com valores vazios

  //Aqui começa a geração aleatória de nucleotideos para a sequênciaA e para a sequênciaB
  #pragma omp parallel
  {
    #pragma omp for
    for(int i=0;i<sizeFirstSequence;i++){
        int aux=rand()%4;
        if(aux==0)
            sequenciaA[i]='A';
        else if(aux==2)
            sequenciaA[i]='C';
        else if(aux==3)
            sequenciaA[i]='G';
        else
            sequenciaA[i]='T';
    }

    #pragma omp for
    for(int i=0;i<sizeSecondSequence;i++){
        int aux=rand()%4;
        if(aux==0)
            sequenciaB[i]='A';
        else if(aux==2)
            sequenciaB[i]='C';
        else if(aux==3)
            sequenciaB[i]='G';
        else
            sequenciaB[i]='T';
    }
  }

  fprintf(stdout, "%lld %lld\n", sizeFirstSequence, sizeSecondSequence);
  for (size_t i = 0; i < sizeFirstSequence; i++) {
    fprintf(stdout, "%c", sequenciaA[i]);
  }
  for (size_t i = 0; i < sizeSecondSequence; i++) {
    fprintf(stdout, "%c", sequenciaB[i]);
  }

  return 0;
}
