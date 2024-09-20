#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openacc.h>

int main(int argc, char *argv[]){
    if(argc !=3 || strcmp(argv[1], "-numpoints") != 0){
        printf("Format: %s -numpoints <N>\n", argv[0]);
        return 1;
    }

    int total_points = atoi(argv[2]);
    int circle_points = 0;
    double pi;

    //rand number generator
    srand(time(NULL));

    #pragma acc data copy(circle_points)
    {
        #pragma acc parallel loop reduction(+: circle_points)
        for(int i = 0; i < total_points; i++){
            double rand_x = (double)rand() / RAND_MAX;
            double rand_y = (double)rand() / RAND_MAX;
            double dist = rand_x * rand_x + rand_y * rand_y;
            if (dist <= 1) {
                circle_points++;
            }
        }
    }

    pi = 4.0 * circle_points/total_points;
    printf("The Estimation of Pi = %.6f\n", pi);

}
