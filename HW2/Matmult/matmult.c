#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
    int N = 0;
    int M = 0;
    
    //get the input from command line
    if(strcmp(argv[1], "-n") != 0 || strcmp(argv[3], "-m") != 0){
        fprintf(stderr, "Invalid Input Format, ./matmult -n <N> -m <M>");
    }else{
        N = atoi(argv[2]);
        M = atoi(argv[4]);
    }

    //allocate memory for matrices A, B and C
    float *A = (float *)malloc(N * M * sizeof(float));
    float *B = (float *)malloc(M * N * sizeof(float));
    float *C = (float *)malloc(N * N * sizeof(float));

    //init A and B, fill both with 1.0f
    for(int i = 0; i < N*M; i++){
        A[i] = 1.0f;
        B[i] = 1.0f;
    }

    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);

    #pragma acc data copyin(A[0: N*M], B[0: M*N]) copyout(C[0: N*N])
    {
        #pragma acc parallel loop collapse(2)
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                float sum = 0.0f;
                #pragma acc loop reduction(+:sum)
                for(int k = 0; k < M; k++){
                    //sum = A[i][k] + B[k][j]
                    sum += A[i * M + k] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }
        }

    }

    gettimeofday(&end, NULL);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("Time for matrix multiply (b): %.02f\n", time);

    //check result
    int false = 0;
    #pragma acc data copy(C[0:N*N]) copyout(false)
    {
        #pragma acc parallel loop reduction(+:false)
        for(int i = 0; i < N * N; i++){
            false += (C[i] != M);
        }
        
    }
    
    if(false == 0) {
        printf("The Matrix Computation Result is correct.\n");
    }else{
        printf("Result is incorrect.\n");
    }

    gettimeofday(&start, NULL);
    #pragma acc data copyin(A[0: N*M], B[0: M*N]) copyout(C[0: N*N])
    {
        #pragma acc parallel loop collapse(2)
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                float sum = 0.0f;
                #pragma acc loop reduction(+:sum)
                for(int k = 0; k < M; k++){
                    //sum = A[i][k] + B[j][k]
                    sum += A[i * M + k] * B[j * N + k];
                }
                C[i * N + j] = sum;
            }
        }

    }

    gettimeofday(&end, NULL);
    time = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("Time for matrix multiply (d): %.02f\n", time);


    gettimeofday(&start, NULL);
    #pragma acc data copyin(A[0: N*M], B[0: M*N]) copyout(C[0: N*N])
    {
        #pragma acc parallel loop collapse(2)
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                float sum = 0.0f;
                #pragma acc loop reduction(+:sum)
                for(int k = 0; k < M; k++){
                    //sum = A[k][i] + B[k][j]
                    sum += A[k * M + i] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }
        }

    }
    gettimeofday(&end, NULL);
    time = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("Time for matrix multiply (e): %.02f\n", time);

    free(A);
    free(B);
    free(C);

    return 0;


}

