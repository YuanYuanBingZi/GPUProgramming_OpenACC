#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char **argv){
    int N = 0;
    float tol = 0.0f;
    int max_iter = 0;

    if(strcmp(argv[1], "-n") != 0 || strcmp(argv[3], "-tol") != 0 || strcmp(argv[5], "-max_iter") != 0){
        fprintf(stderr, "Invalid Input Format, ./heat -n <N> -tol <t> -max_iter <iter>\n");
    }else{
        N = atoi(argv[2]);
        tol = atof(argv[4]);
        max_iter = atoi(argv[6]);
    }


    float *A = (float *)malloc(N * N * sizeof(float));
    float *Anew = (float *)malloc(N * N * sizeof(float));

    //Initialize the arrays
    for(int i = 0; i < N * N; i++){
            A[i] = 0.0f;
            Anew[i] = 0.0f;
    }

    for(int i = 0; i < N; i++){
        A[i] = 100.0f;
        Anew[i] = 100.0f;

    }


    int iter = 0;
    float err = 100.0f;

    #pragma acc data copy(A[0: N*N], Anew[0:N*N])
    {
        while(err > tol && iter < max_iter){
            err = 0.0f;
            #pragma acc parallel loop reduction(max:err)
            for(int i = 1; i < N - 1; i++){
                for(int j = 1; j < N - 1; j++){
                    Anew[i * N + j] = (A[(i + 1) * N + j] + A[(i - 1) * N + j] + A[i * N + j - 1] + A[i * N + j + 1])*0.25;
                    err = fmaxf(err, fabs(Anew[i * N + j] - A[i * N + j]));}
            }

            
            #pragma acc parallel loop
            for(int i = 1; i < N - 1; i++){
                for(int j = 1; j < N - 1; j++){
                    A[i * N + j] = Anew[i *N + j];
                }
            }



            if(iter % 1000 == 0){
                char filename[150];
                sprintf(filename, "heat_%d.csv", iter);
                FILE *fp = fopen(filename, "w");
                if(fp != NULL){
                    for(int i = 0; i < N; i++){
                        for(int j = 0; j < N; j++){
                            fprintf(fp, "%.2f", Anew[i * N + j]);
                            if(j < N - 1){
                                fprintf(fp, ",");
                            }
                        }
                        fprintf(fp, "\n");
                    }
                    fclose(fp);
                }else{
                    fprintf(stderr, "Failed to open the file '%s \n", filename);
                }
            }

            iter++;

        }
    }

    printf("iter: %d\n", iter);

    free(A);
    free(Anew);

    return 0;
}
