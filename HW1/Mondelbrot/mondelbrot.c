#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITERATION 1000

int check_mandelbrot(double x0, double y0){
    double x = 0.0, y = 0.0;
    int iteration = 0;
    while(x*x + y*y <= 4 && iteration < MAX_ITERATION){
        double xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
    }
    return iteration;
}

int main(int argc, char *argv[]){
    int numx, numy;
    double x_min = -2.0, x_max = 1.0, y_min = -2.0, y_max = 2.0;

    if(argc != 5 || strcmp(argv[1], "-numx") != 0 || strcmp(argv[3], "-numy") != 0){
        printf("Format: %s -numx <n>  -numy <m> \n", argv[0]);
        return 1;
    }else{
        numx = atoi(argv[2]);
        numy = atoi(argv[4]);
    }

    double interval_x = (x_max - x_min) / numx;
    double interval_y = (y_max - y_min) / numy;


    FILE *fp = fopen("mandelbrot.csv", "w");
    if(fp == NULL){
        perror("Error open the csv file\n");
        return 1;
    }

    #pragma acc data
    {
        #pragma acc kernels
        for(int j = 0; j < numy; j++){
            for(int i = 0; i < numx; i++){
                double x0 = x_min + i * interval_x;
                double y0 = y_min + j * interval_y;
                int iteration = check_mandelbrot(x0, y0);
                fprintf(fp, "%d,", iteration);
            }
            fprintf(fp, "\n");
        }
    }

    return 0;

}