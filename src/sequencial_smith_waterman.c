#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/*--------------------------------------------------------------------
 * Text Tweaks
 */
#define RESET   "\033[0m"
#define BOLDRED "\033[1m\033[31m"      /* Bold Red */
/* End of text tweaks */

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
void printPredecessorMatrix(int* matrix, int sizeFirstSequence, int sizeSecondSequence);
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


    #ifdef DEBUG
    printf("Matrix[%lld][%lld]\n", sizeFirstSequence, sizeSecondSequence);
    #endif

    //Alocação de espaço para as duas sequências de entrada
    sequenciaA = malloc(sizeFirstSequence * sizeof(char)); // Para a Linha
    sequenciaB = malloc(sizeSecondSequence * sizeof(char)); // Para a Coluna. No entanto vetores que inicializam com valores vazios

    for (int k = 0; k < sizeFirstSequence; k++) {
      char c;
      fscanf(stdin, "%c\n", &c );
      if(c != '\n'){
        sequenciaA[k] = c;
      }
      else {
        k--;
      }
    }

    for (int k = 0; k < sizeSecondSequence; k++) {
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
    H = malloc(sizeFirstSequence * sizeSecondSequence * sizeof(int));
    //Alocação da matriz que através da sequência de pontuações encontra a pntuação mais alta.
    int *P;
    P = malloc(sizeFirstSequence * sizeSecondSequence * sizeof(int));
    long long int i,j;




    //Generates the values of b

    //Posição inicial para iniciar o backtracking(ou seja, a verificação da pontuação)
    long long int maxPos = 0;
    //Indicies para a matriz de similaridade
    double initialTime = omp_get_wtime();
    //Na primeira linha e na primeira coluna possui 0s, atributo do algoritmo.
    for (i = 1; i < sizeSecondSequence; i++) { //Lines
        for (j = 1; j < sizeFirstSequence; j++) { //Columns
          int up, left, diag;
          //Stores index of element
          long long int indice = sizeFirstSequence * i + j;
          //Get element above
          up = H[indice - sizeFirstSequence] + gapScore;
          //Get element on the left
          left = H[indice - 1] + gapScore;
          //Get element on the diagonal
          if (sequenciaA[j-1] == sequenciaB[i-1]){
              diag = H[indice - sizeFirstSequence - 1] + matchScore;
          }
          else
              diag = H[indice - sizeFirstSequence - 1] + missmatchScore;
          //Calculates the maximum
          int max1 = max3(up, left, diag);
          max1 = max1 < NONE ? NONE : max1;
          int max = NONE;
          int pred = NONE;
          if (diag > max) { //same letter ↖
              max = diag;
              pred = DIAGONAL;
          }
          else if (up > max) { //remove letter ↑
              max = up;
              pred = UP;
            }
          else if (left > max) { //insert letter ←
              max = left;
              pred = LEFT;
          }
          //Inserts the value in the similarity and predecessor matrixes
          H[indice] = max;
          P[indice] = pred;
          printf("up: %d, left: %d, diagonal: %d\n", up, left, diag );
          printf("H[%d][%d]: %d ou %d\n", i,j, max1, max );

          //Updates maximum score to be used as seed on backtrack
          if (max > H[maxPos]) {
              maxPos = indice;
          }
        }
    }
    //Término da criação da matriz de similaridade.
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
    printf("\nElapsed time: %f\n\n", finalTime - initialTime);

    //Impressão das matrizes em DEBUG
    #ifdef DEBUG
    printf("\nSimilarity Matrix:\n");
    printMatrix(H, sizeFirstSequence, sizeSecondSequence);

    // printf("\nPredecessor Matrix:\n");
    // printMatrix(P, sizeFirstSequence, sizeSecondSequence);
    #endif

    //Frees similarity matrixes
    free(H);
    free(P);

    //Frees input arrays
    free(sequenciaA);
    free(sequenciaB);

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
            printf("%d\t", matrix[sizeFirstSequence * i + j]);
        }
        printf("\n");
    }

}  /* End of printMatrix */

/*--------------------------------------------------------------------
 * Function:    printPredecessorMatrix
 * Purpose:     Print predecessor matrix
 */
void printPredecessorMatrix(int* matrix, int sizeFirstSequence, int sizeSecondSequence) {
    long long int i, j, index;
    for (i = 0; i < sizeFirstSequence; i++) { //Lines
        for (j = 0; j < sizeSecondSequence; j++) {
            index = sizeFirstSequence * i + j;
            if(matrix[index] < 0) {
                printf(BOLDRED);
                if (matrix[index] == -UP)
                    printf("↑ ");
                else if (matrix[index] == -LEFT)
                    printf("← ");
                else if (matrix[index] == -DIAGONAL)
                    printf("↖ ");
                else
                    printf("- ");
                printf(RESET);
            } else {
                if (matrix[index] == UP)
                    printf("↑ ");
                else if (matrix[index] == LEFT)
                    printf("← ");
                else if (matrix[index] == DIAGONAL)
                    printf("↖ ");
                else
                    printf("- ");
            }
        }
        printf("\n");
    }

}  /* End of printPredecessorMatrix */
