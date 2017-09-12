#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void reg_matriz(int *, int);
void mult_matriz(int *, int *, int *, int);
void print_matriz(int *, int);
void strassen(int *, int *, int *, int);
void sum(int *, int *, int *, int);
void sub(int *, int *, int *, int);


int main(int argc, char *argv[])
{
    int tam = 4096;
    int *matriz_A;
    int *matriz_B;
    int *matriz_res;
    clock_t timer;
    srand(time(NULL));

    matriz_A = malloc(tam*tam*sizeof(int));
    matriz_B = malloc(tam*tam*sizeof(int));
    matriz_res = malloc(tam*tam*sizeof(int));

    if(!matriz_A && !matriz_B && !matriz_res){
        puts("Erro de alocacao.\n");
        exit(1);
    }

    reg_matriz(matriz_A,tam);
    reg_matriz(matriz_B,tam);

    timer = clock();
    mult_matriz(matriz_A, matriz_B, matriz_res, tam);
    timer = clock() - timer;
    printf("O tempo de execucao para o metodo burro foi de %f segundos.\n", ((float)timer)/CLOCKS_PER_SEC);

//    print_matriz(matriz_A, tam);
//    printf("\n");
//    print_matriz(matriz_B, tam);
//    printf("\n");
//    print_matriz(matriz_res, tam);
//    printf("\n");

    free(matriz_A);
    free(matriz_B);
    free(matriz_res);


    matriz_A = malloc(tam*tam*sizeof(int));
    matriz_B = malloc(tam*tam*sizeof(int));
    matriz_res = malloc(tam*tam*sizeof(int));

    if(!matriz_A && !matriz_B && !matriz_res){
        puts("Erro de alocacao.\n");
        exit(1);
    }

    reg_matriz(matriz_A,tam);
    reg_matriz(matriz_B,tam);

    timer = clock();
    strassen(matriz_A, matriz_B, matriz_res, tam);
    timer = clock() - timer;
    printf("O tempo de execucao para o metodo de strassen foi de %f segundos.\n", ((float)timer)/CLOCKS_PER_SEC);



//    print_matriz(matriz_A, tam);
//    printf("\n");
//    print_matriz(matriz_B, tam);
//    printf("\n");
//    print_matriz(matriz_res, tam);
//    printf("\n");

    free(matriz_A);
    free(matriz_B);
    free(matriz_res);

    return 0;
}


void reg_matriz(int *matriz, int tam){
    int i, r;
    for(i = 0; i < tam*tam; i++){
            r = rand()%100;
            matriz[i]= r;
    }
}

void mult_matriz(int *matriz_A, int *matriz_B, int *res, int tam){
    int i, j, k, temp;

    for(i = 0; i < tam; i++){
        for(j = 0; j < tam; j++){
            temp = 0;
            for(k = 0; k < tam; k++){
                temp += matriz_A[i*tam+k] * matriz_B[k*tam+j];
            }
            res[i*tam+j] = temp;
        }
    }
}

void print_matriz(int *matriz, int tam){
    int i, j;

    for (i = 0; i < tam; i++)
    {
        for (j = 0; j < tam; j++)
            printf("%d  ",matriz[i*tam+j]);
        printf("\n");
    }
    printf("\n");
}

void strassen(int *A, int *B, int *C, int tam){
    int i, j, lin = 0, col = 0;
    int n = tam/2;

    if (tam <= 32)
    {
       mult_matriz(A, B, C, tam);
       return;
    }

    if (tam == 1){
        C[0] = A[0] * B[0];
        return;
    }

    int *A11 = malloc(n*n*sizeof(int));
    int *A12 = malloc(n*n*sizeof(int));
    int *A21 = malloc(n*n*sizeof(int));
    int *A22 = malloc(n*n*sizeof(int));

    int *B11 = malloc(n*n*sizeof(int));
    int *B12 = malloc(n*n*sizeof(int));
    int *B21 = malloc(n*n*sizeof(int));
    int *B22 = malloc(n*n*sizeof(int));

    int *P1 = malloc(n*n*sizeof(int));
    int *P2 = malloc(n*n*sizeof(int));
    int *P3 = malloc(n*n*sizeof(int));
    int *P4 = malloc(n*n*sizeof(int));
    int *P5 = malloc(n*n*sizeof(int));
    int *P6 = malloc(n*n*sizeof(int));
    int *P7 = malloc(n*n*sizeof(int));

    int *C11 = malloc(n*n*sizeof(int));
    int *C12 = malloc(n*n*sizeof(int));
    int *C21 = malloc(n*n*sizeof(int));
    int *C22 = malloc(n*n*sizeof(int));

    int *S1 = malloc(n*n*sizeof(int));
    int *S2 = malloc(n*n*sizeof(int));

    if(!A11 && !A12 && !A21 && !A22 && !B11 && !B12 && !B21 && !B22 && !P1 && !P2 &&
            !P3 && !P4 && !P5 && !P6 && !P7 && !C11 && !C12 && !C21 && !C22 && S1 && S2){
        puts("Erro de alocacao.\n");
        exit(1);
    }


    for (lin = 0, i = 0; lin < n; lin++, i++)
    {
        for (col = 0, j = 0; col < n; col++, j++)
        {
            A11[i*n+j] = A[lin*tam+col];
            B11[i*n+j] = B[lin*tam+col];
        }
        for (col = n, j = 0; col < tam; col++, j++)
        {
            A12[i*n+j] = A[lin*tam+col];
            B12[i*n+j] = B[lin*tam+col];
        }
    }

    for (lin = n, i = 0; lin < tam; lin++, i++)
    {
        for (col = 0, j = 0; col < n; col++, j++)
        {
            A21[i*n+j] = A[lin*tam+col];
            B21[i*n+j] = B[lin*tam+col];
        }
        for (col = n, j = 0; col < tam; col++, j++)
        {
            A22[i*n+j] = A[lin*tam+col];
            B22[i*n+j] = B[lin*tam+col];
        }
    }

    //P1
    sum(A11, A22, S1, n);
    sum(B11, B22, S2, n);
    strassen(S1, S2, P1, n);

    //P2
    sum(A21, A22, S1, n);
    strassen(S1, B11, P2, n);

    //P3
    sub(B12, B22, S2, n);
    strassen(A11, S2, P3, n);

    //P4
    sub(B21, B11, S2, n);
    strassen(A22, S2, P4, n);

    //P5
    sum(A11, A12, S1, n);
    strassen(S1, B22, P5, n);

    //P6
    sub(A21, A11, S1, n);
    sum(B11, B12, S2, n);
    strassen(S1, S2, P6, n);

    //P7
    sub(A12, A22, S1, n);
    sum(B21, B22, S2, n);
    strassen(S2, S1, P7, n);

    //C11
    sum(P1, P4, C11, n);
    sub(C11, P5, C11, n);
    sum(C11, P7, C11, n);

    //C12
    sum(P3, P5, C12, n);

    //C21
    sum(P2, P4, C21, n);

    //C22
    sub(P1, P2, C22, n);
    sum(C22, P3, C22, n);
    sum(C22, P6, C22, n);

    for (lin = 0, i = 0; lin < n; lin++, i++)
    {
        for (col = 0, j = 0; col < n; col++, j++)
            C[lin*tam+col] = C11[i*n+j];
        for (col = n, j = 0; col < tam; col++, j++)
            C[lin*tam+col] = C12[i*n+j];
    }
    for (lin = n, i = 0; lin < tam; lin++, i++)
    {
        for (col = 0, j = 0; col < n; col++, j++)
            C[lin*tam+col] = C21[i*n+j];
        for (col = n, j = 0; col < tam; col++, j++)
            C[lin*tam+col] = C22[i*n+j];
    }

    free(S1);
    free(S2);
    free(A11);
    free(A12);
    free(A21);
    free(A22);
    free(B11);
    free(B12);
    free(B21);
    free(B22);
    free(P1);
    free(P2);
    free(P3);
    free(P4);
    free(P5);
    free(P6);
    free(P7);
    free(C11);
    free(C12);
    free(C21);
    free(C22);

}


void sum(int *matriz_A, int *matriz_B, int *matriz_res, int tam){
    int i, j;

    for (i = 0; i < tam; i++){
        for (j = 0; j < tam; j++){
            matriz_res[i*tam+j] = matriz_A[i*tam+j] + matriz_B[i*tam+j];
        }
    }
}


void sub(int *matriz_A, int *matriz_B, int *matriz_res, int tam){
    int i, j;

    for (i = 0; i < tam; i++){
        for (j = 0; j < tam; j++){
            matriz_res[i*tam+j] = matriz_A[i*tam+j] - matriz_B[i*tam+j];
        }
    }
}
