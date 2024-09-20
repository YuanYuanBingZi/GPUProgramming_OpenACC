#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openacc.h>
#include <sys/time.h>

#define SCALAR 666


int main(int argc, char *argv[]){
    if(argc != 3 || strcmp(argv[1], "-size") != 0){
        printf("Format: %s -size <n> \n", argv[1]);
        return 1;
    }

    int n = atoi(argv[2]);
    struct timeval start;
    struct timeval end;
    //part a: initialize the array and assign values
    float *a = (float *)malloc(sizeof(float) * n);
    float *b = (float *)malloc(sizeof(float) * n);
    float *c = (float *)malloc(sizeof(float) * n);

    #pragma acc parallel loop
    for(int i = 0; i < n; i++){
        a[i] = 1.0;
        b[i] = 2.0;
        c[i] = 3.0;
    }

    //part b: OUTPUT the total time and estimated bandwith, see below
    //part c: "COPY" benchmark
    gettimeofday(&start, NULL);
    #pragma acc data copyin(a[0:n]) copyout(c[0:n])
    {
        #pragma acc parallel loop
        for(int i = 0; i < n; i++){
            c[i] = a[i];
        }
    }
    gettimeofday(&end, NULL);
    double diff = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    double bandwidth = (n * sizeof(float) * 2)/diff; 
    printf("COPY: Estimated memeory bandwith is %f \n", bandwidth);

    //part d: "SCALE" benchmark
    gettimeofday(&start, NULL);
    #pragma acc data copyin(c[0:n]) copyout(b[0:n])
    {
        #pragma acc parallel loop
        for(int i = 0; i < n; i++){
            b[i] = SCALAR * c[i];
        }

    }
    gettimeofday(&end, NULL);
    diff = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    bandwidth = (n * sizeof(float) * 2)/diff; 
    printf("SCALE: Estimated memeory bandwith is %f \n", bandwidth);

    //part e: "ADD" benchmark
    gettimeofday(&start, NULL);
    #pragma acc data copyin(a[0:n], b[0:n]) copyout(c[0:n])
    {
        #pragma acc parallel loop
        for(int i = 0; i < n; i++){
        c[i] = a[i] + b[i];
        }
    }
    gettimeofday(&end, NULL);
    diff = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    bandwidth = (n * sizeof(float) * 3)/diff;
    printf("ADD: Estimated memeory bandwith is %f \n", bandwidth);

    //part f: "TRIAD" benchmark
    gettimeofday(&start, NULL);
    #pragma acc data copyin(b[0:n], c[0:n]) copyout(a[0:n])
    {
        #pragma acc parallel loop
        for(int i = 0; i < n; i++){
        a[i] = b[i] + SCALAR * c[i];
        }
    }

    gettimeofday(&end, NULL);
    diff = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    bandwidth = (n * sizeof(float) * 3)/diff; 
    printf("TRIAD: Estimated memeory bandwith is %f \n", bandwidth);


}



