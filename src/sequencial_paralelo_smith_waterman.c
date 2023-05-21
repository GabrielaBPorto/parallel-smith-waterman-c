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
void printMatrix(int* matrix, int sizeFirstSequence, int sizeSecondSequence);
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

int main(int argc, char* argv[]) {
    // if(argc < 3){
    //   printf("Foi colocada quantidade %d de argumentos, sendo ela errada para o funcionamento do código. Qualquer dúvida leia o README\n", argc);
    //   exit(0);
    // }

    //Definição das pontuações
    int matchScore = 5;
    int missmatchScore = -3;
    int gapScore = -4;
    //Definição do tamanho das seqencias
    long long int sizeFirstSequence, sizeSecondSequence;
    //sizeFirstSequence = strtoll(argv[1], NULL, 10);
    //sizeSecondSequence = strtoll(argv[2], NULL, 10);
    fscanf(stdin, "%lld\n", &sizeFirstSequence);
    fscanf(stdin, "%lld\n", &sizeSecondSequence);
    // long l ong int i,j;

    int totalDiagonais = sizeFirstSequence + sizeSecondSequence - 1;

    #ifdef DEBUG
    printf("Matrix[%lld][%lld]\n", sizeFirstSequence, sizeSecondSequence);
    #endif

    //Alocação de espaço para as duas sequências de entrada
    sequenciaA = malloc(sizeFirstSequence * sizeof(char)); // Para a Linha
    sequenciaB = malloc(sizeSecondSequence * sizeof(char)); // Para a Coluna. No entanto vetores que inicializam com valores vazios

    for (long long int k = 0; k < sizeFirstSequence; k++) {
      char c;
      fscanf(stdin, "%c\n", &c );
      if(c != '\n'){
        sequenciaA[k] = c;
      }
      else {
        k--;
      }
    }

    for (long long int k = 0; k < sizeSecondSequence; k++) {
      char c;
      fscanf(stdin, "%c\n", &c );
      if(c != '\n'){
        sequenciaB[k] = c;
      }
      else {
        k--;
      }
    }

    //É adicionado no tamanho de cada sequencia pois [0][j] e [i][0] vem 0
    sizeFirstSequence++;
    sizeSecondSequence++;

    //Alocação da matriz de Similaridade. Pontuação gerada entre as similaridades entre sequenciaA e sequenciaB
    int *H;
    H = calloc(sizeFirstSequence * sizeSecondSequence, sizeof(int));
    //Alocação da matriz que através da sequência de pontuações encontra a pntuação mais alta.
    int *P;
    P = calloc(sizeFirstSequence * sizeSecondSequence, sizeof(int));

    //Posição inicial para iniciar o backtracking(ou seja, a verificação da pontuação)
    long long int maxPos = 0;
    //Indicies para a matriz de similaridade
    double initialTime = omp_get_wtime();
    //Na primeira linha e na primeira coluna possui 0s, atributo do algoritmo.
    for (long long int temp = 1; temp < totalDiagonais+1; temp++) {
      for (long long int i = temp; i >= 1; i--) {
          long long int j = temp-i +1;
          if((j >= sizeFirstSequence) || (i >= sizeSecondSequence)){
            continue;
          }
          long long int indice = (i*sizeFirstSequence) + j;
          int up, left, diagonal;
          up = H[indice - sizeFirstSequence] + gapScore;
          left = H[indice-1] + gapScore;
          diagonal = sequenciaA[j-1] == sequenciaB[i-1] ? H[indice-sizeFirstSequence - 1] + matchScore : H[indice - sizeFirstSequence -1] + missmatchScore;
          int max = max3(up, left, diagonal);
          max = max < NONE ? NONE : max;
          int pred = (max == up) ? UP : (max == left) ? LEFT : (max == diagonal) ? DIAGONAL : NONE;
          H[indice] = max;
          #ifdef DEBUG
          // printf("seqA:%c;seqB:%c\n", sequenciaA[j-1], sequenciaB[i-1]);
          // printf("H[%d][%d]\n", i,j);
          // printf("[%d][%d]up:%d;[%d][%d]left:%d;[%d][%d]diagonal:%d;max:%d;pred:%d\n",i-1,j,up, i, j-1, left, i-1, j-1, diagonal, max, pred );
          #endif
          P[indice] = pred;
          if (max > H[maxPos]) {
            maxPos = indice;
          }
      }
    }


    //Término da criação da matriz de similaridade.
    //Frees input arrays
    //Começa a verificar qual melhores sequências
    long long int predPos;
    int pop=0;
    //backtrack from maxPos to startPos = 0
    do {
        if(P[maxPos] == DIAGONAL)
            predPos = maxPos - sizeFirstSequence - 1;
        else if(P[maxPos] == UP)
            predPos = maxPos - sizeFirstSequence;
        else if(P[maxPos] == LEFT)
            predPos = maxPos - 1;
        P[maxPos]*=PATH;
        maxPos = predPos;
        //printf("rodada %d\n", pop );
        pop++;
    } while(P[maxPos] != NONE);

    //Gets final time
    double finalTime = omp_get_wtime();

    //Impressão das matrizes em DEBUG
    #ifdef DEBUG
    printf("\nElapsed time: %f\n\n", finalTime - initialTime);

    printMatrix(H, sizeFirstSequence, sizeSecondSequence);
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
void printMatrix(int* matrix, int sizeFirstSequence, int sizeSecondSequence) {
    long long int i, j;
    for (i = 0; i < sizeFirstSequence; i++) { //Lines
        for (j = 0; j < sizeSecondSequence; j++) {
          // printf("[%d]", sizeFirstSequence * i + j );
          // printf("%d\n", matrix[sizeFirstSequence * i + j]);
            printf("%d\t", matrix[sizeFirstSequence * i + j]);
        }
        printf("\n");
    }

}  /* End of printMatrix */
