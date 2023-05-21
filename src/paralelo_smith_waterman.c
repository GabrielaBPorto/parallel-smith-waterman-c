#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


/*--------------------------------------------------------------------
 * Constants
 */
#define PATH -1
#define NONE 0
#define UP 1
#define LEFT 2
#define DIAGONAL 3

// #define DEBUG

/* End of constants */


/*--------------------------------------------------------------------
 * Functions Prototypes
 */
void printMatrix(int* matrix, int block_num, int sizeFirstSequence, int sizeSecondSequence);
/* End of prototypes */


/*--------------------------------------------------------------------
 * Global Variables
 */
//Strings over the Alphabet Sigma
char *sequenciaA, *sequenciaB;

#define max(a,b) \
   ({ typeof (a) _a = (a); \
       typeof (b) _b = (b); \
     _a > _b ? _a : _b; })

#define max3(a,b,c) max(a,max(b,c))

void le_sequencias(int n, int m){
  for (long long int k = 0; k < n; k++) {
    char c;
    fscanf(stdin, "%c\n", &c );
    if(c != '\n'){
      sequenciaA[k] = c;
    }
    else {
      k--;
    }
  }

  for (long long int k = 0; k < m; k++) {
    char c;
    fscanf(stdin, "%c\n", &c );
    if(c != '\n'){
      sequenciaB[k] = c;
    }
    else {
      k--;
    }
  }
}

int main(int argc, char* argv[]) {
    if(argc < 2){
      printf("Argumentos insuficientes\n");
      exit(0);
    }
    int num_threads = strtoll(argv[1], NULL, 10);
    long long int block_size = strtoll(argv[2], NULL, 10);

    //Definição das pontuações
    int matchScore = 5;
    int missmatchScore = -3;
    int gapScore = -4;
    //Definição do tamanho das sequencias
    long long int sizeFirstSequence, sizeSecondSequence;
    fscanf(stdin, "%lld\n", &sizeFirstSequence);
    fscanf(stdin, "%lld\n", &sizeSecondSequence);

    //Definições de variaveis utilizadas para percorrer a matriz em blocos.
    long long int numblocks = sizeSecondSequence / block_size;
    long long int line_size = sizeSecondSequence / numblocks;
    int diagonaisBloco = sizeFirstSequence + line_size - 1;
    //Posição inicial para iniciar o backtracking(ou seja, a verificação da pontuação)
    long long int maxPos = 0;

    #ifdef DEBUG
    printf("Matrix[%lld][%lld]\n", sizeFirstSequence, sizeSecondSequence);
    #endif

    //Alocação de espaço para as duas sequências de entrada
    sequenciaA = malloc(sizeFirstSequence * sizeof(char)); // Para a Linha
    sequenciaB = malloc(sizeSecondSequence * sizeof(char)); // Para a Coluna. No entanto vetores que inicializam com valores vazios
    le_sequencias(sizeFirstSequence, sizeSecondSequence);

    //É adicionado no tamanho de cada sequencia pois [0][j] e [i][0] vem 0
    sizeFirstSequence++;
    sizeSecondSequence++;

    //Alocação das matrizes utilizadas
    int *H;
    H = calloc(sizeFirstSequence *  (line_size + 1), sizeof(int));
    int *P;
    P = calloc(sizeFirstSequence * (line_size + 1), sizeof(int));

    double initialTime = omp_get_wtime();

    int controle = 0;
    //Percorre todos os blocos
    for (size_t b = 1; b < numblocks+1; b++) {
      int limiarDiagonais = diagonaisBloco/2;
      int line = b * line_size;
      // printf("line=%d\n",line );
      //Aqui somente uma thread irá trabalhar, pois será perda de desempenho direcionar mais de uma thread responsável para pouco processamento.
      for (long long int temp = 1; temp < limiarDiagonais; temp++) {
        for (long long int i = temp; i >= 1; i--) {
          long long int j = temp-i +1;
          if((j >= sizeFirstSequence) || (i+controle > line)){
            // printf("teste i=%d; j=%d; line=%d; fi=%d\n",i,j,line,sizeFirstSequence );
            continue;
          }
          long long int indice = (i*sizeFirstSequence) + j;
          int up, left, diagonal;
          //Faz os cálculos para as 3 pontuações
          up = H[indice - sizeFirstSequence] + gapScore;
          left = H[indice-1] + gapScore;
          diagonal = sequenciaA[j-1] == sequenciaB[(i+controle)-1] ? H[indice-sizeFirstSequence - 1] + matchScore : H[indice - sizeFirstSequence -1] + missmatchScore;
          //Verificar qual a maior das 3 pontuações
          int max = max3(up, left, diagonal);
          max = max < NONE ? NONE : max;
          int pred = (max == up) ? UP : (max == left) ? LEFT : (max == diagonal) ? DIAGONAL : NONE;
          #ifdef DEBUG
          printf("seqA:%c;seqB:%c\n", sequenciaA[j-1], sequenciaB[(i+controle)-1]);
          printf("H[%d][%d]\n", i+controle,j);
          printf("H[%d]=%d\n", indice, max );
          printf("[%d][%d]up:%d;[%d][%d]left:%d;[%d][%d]diagonal:%d;max:%d;pred:%d\n",(i+controle)-1,j,up, (i+controle), j-1, left, (i+controle)-1, j-1, diagonal, max, pred );
          #endif
          H[indice] = max;
          P[indice] = pred;
          //Caso tenha uma posição que seja maior que a posição max anterior, pega essa.
          if (max > H[maxPos]) {
            maxPos = indice;
          }
        }
      }
      //Aqui permanece a lógica de trabalhar com as diagonais, no entanto cada *diagonal* é divida entre n threads
      for (long long int temp = limiarDiagonais; temp < diagonaisBloco+1; temp++) {
        #pragma omp parallel for num_threads(num_threads)
        for (long long int i = temp; i >= 1; i--) {
          long long int j = temp-i +1;
          if((j >= sizeFirstSequence) || (i+controle > line)){
            continue;
          }
          long long int indice = (i*sizeFirstSequence) + j;
          int up, left, diagonal;
          up = H[indice - sizeFirstSequence] + gapScore;
          left = H[indice-1] + gapScore;
          diagonal = sequenciaA[j-1] == sequenciaB[(i+controle)-1] ? H[indice-sizeFirstSequence - 1] + matchScore : H[indice - sizeFirstSequence -1] + missmatchScore;
          int max = max3(up, left, diagonal);
          max = max < NONE ? NONE : max;
          int pred = (max == up) ? UP : (max == left) ? LEFT : (max == diagonal) ? DIAGONAL : NONE;
          #ifdef DEBUG
          printf("seqA:%c;seqB:%c\n", sequenciaA[j-1], sequenciaB[(i+controle)-1]);
          printf("H[%d]=%d\n", indice, max );
          printf("H[%d][%d]\n", i+controle,j);
          printf("[%d][%d]up:%d;[%d][%d]left:%d;[%d][%d]diagonal:%d;max:%d;pred:%d\n",(i+controle)-1,j,up, (i+controle), j-1, left, (i+controle)-1, j-1, diagonal, max, pred );
          #endif
          H[indice] = max;
          P[indice] = pred;
          if (max > H[maxPos]) {
            #pragma omp critical
            {
              maxPos = indice;
            }
          }
        }
      }
      controle += line_size;
      long long int predPos;
      //backtrack from maxPos to startPos = 0
      do {
        //TODO enquanto vai gravando melhor caminho na matriz, guarda a sequencia que será devolvido para o usuário
        if(P[maxPos] == DIAGONAL)
        predPos = maxPos - sizeFirstSequence - 1;
        else if(P[maxPos] == UP)
        predPos = maxPos - sizeFirstSequence;
        else if(P[maxPos] == LEFT)
        predPos = maxPos - 1;
        P[maxPos]*=PATH;
        maxPos = predPos;
      } while(P[maxPos] != NONE);

      #ifdef DEBUG
      printMatrix(H,b, line_size, sizeFirstSequence);
      #endif

      for (size_t x = 0; x < sizeFirstSequence; x++) {
        H[x] = H[(line*sizeFirstSequence) + x];
        P[x] = P[(line*sizeFirstSequence) + x];
      }

    }

    //Gets final time
    double finalTime = omp_get_wtime();

    //Impressão das matrizes em DEBUG
    #ifdef DEBUG
    printf("\nElapsed time: %f\n\n", finalTime - initialTime);

    #endif

    //Frees similarity matrixes
    // free(sequenciaA);
    // free(sequenciaB);
    // free(H);
    // free(P);


    return 0;
}  /* End of main */


/*--------------------------------------------------------------------
 * Function:    printMatrix
 * Purpose:     Print Matrix
 */
void printMatrix(int* matrix, int block_num, int n, int m) {
  long long int i, j;
  if(block_num < 2){
    for (i = 0; i <= n; i++) { //Lines
      for (j = 0; j < m; j++) {
        printf("%d\t", matrix[m * i + j]);
      }
      printf("\n");
    }
  }
  else {
    for (i = 1; i <= n; i++) { //Lines
      for (j = 0; j < m; j++) {
        printf("%d\t", matrix[m * i + j]);
      }
      printf("\n");
    }
  }
}
