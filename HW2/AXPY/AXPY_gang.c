#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define a 5


int main(int argc, char *argv[]){
    int N;
    if(argc !=3 || strcmp(argv[1], "-n") != 0){
        printf("Format: %s -n <N> \n", argv[0]);
        return 1;
    }

    N = atoi(argv[2]);
    struct timeval start;
    struct timeval end;


    float *X = (float *)malloc(N*N * sizeof(float));
    float *Y = (float *)malloc(N*N * sizeof(float));

    gettimeofday(&start, NULL);
   
    #pragma acc enter data create(X[0:N*N], Y[0:N*N])
    {
        #pragma acc parallel loop gang worker
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                X[i * N + j] = 1.0;
                Y[i * N + j] = 2.0;
            }
        }

        

        #pragma acc parallel loop gang worker
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                Y[i * N + j] = a*X[i * N + j] + Y[i * N + j];
            }
        }

    }
    #pragma acc exit data copyout(X[0:N*N],Y[0:N*N])

    gettimeofday(&end, NULL);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("%.02f\n",time);
    
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(Y[i * N + j] != a*X[i * N + j] + 2.0){
                printf("Error in %d row %d col, error val: %f, correct val: %f\n", i, j, a*X[i * N + j] + 2.0, Y[i * N + j]);
                break;
            }
        }
    }
    
    free(X);
    free(Y);
    
    return 0;

}