#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.c"

#define MAX_SIZE 1000000
#define MAX_BINS 1000
#define MAX_STARS 50

double array[MAX_SIZE];
int bin_counts[MAX_BINS];
double array_copy[MAX_SIZE];

int cmpfunc(const void *a, const void *b) {
    double diff = (*(double *)a - *(double *)b);
    return (diff > 0) - (diff < 0);
}

int main() {
    // start
    printf("Commands:\n  set     - Set array size, seed, and distribution (uniform or gaussian)\n  min     - Print minimum value\n  max     - Print maximum value\n  mean    - Print mean value\n  stddev  - Print standard deviation\n  hist    - Plot histogram\n  summary - Print min, max, mean, stddev\n  help    - Show this help message\n  exit    - Exit the program");
    
    // all declarations
    double sum, mean, max, min, std_dev;
    int size, bins;
    unsigned int seed;

    // infin loop start
    while(1) {
        char s[10];
        printf("\n\nEnter command: ");
        scanf("%s", &s);
        
        // help
        if(strcmp(s, "help") == 0) {
            printf("Commands:\n  set     - Set array size, seed, and distribution (uniform or gaussian)\n  min     - Print minimum value\n  max     - Print maximum value\n  mean    - Print mean value\n  stddev  - Print standard deviation\n  hist    - Plot histogram\n  summary - Print min, max, mean, stddev\n  help    - Show this help message\n  exit    - Exit the program");
            continue;
        }        

        // set
        else if(strcmp(s, "set") == 0) {
            // set command temp declarations
            double given_max, given_min, given_mean, given_std_dev;
            char distribution[10];
            printf("Enter number of elements (<= 100000): "); scanf("%d", &size);
            printf("Enter seed: "); scanf("%d", &seed);
            here:
            printf("Distribution? (uniform/gaussian): "); scanf("%s", &distribution);
            if(strcmp(distribution, "uniform") != 0 && strcmp(distribution, "gaussian") != 0) {
                printf("Invalid distribution\n");
                goto here;
            }
            
            if(strcmp(distribution, "uniform") == 0) {
                printf("Enter min and max: "); scanf("%lf %lf", &given_min, &given_max);
                populate_array_uniform(array, size, given_min, given_max, seed);
                printf("Array initialized with uniform distribution.\n");
            }

            else {
                printf("Enter mean and stddev: "); scanf("%lf %lf", &given_mean, &given_std_dev);
                populate_array_gaussian(array, size, given_mean, given_std_dev, seed);
                printf("Array initialized with gaussian distribution.");
            }
            
            // min
            min = array[0];
            for(int i = 1; i < size; i++) {
                if(array[i] < min) min = array[i];
            }
            
            // max
            max = array[0];
            for(int i = 1; i < size; i++) {
                if(array[i] > max) max = array[i];
            }
            
            // mean
            sum = 0, mean = 0;
            for(int i = 0; i < size; i++) {
                sum += array[i];
            }
            mean = sum/size;
            
            // std deviation
            double numerator_sqr[size];
            double std_dev_num_sum = 0;
            for(int i = 0; i < size; i++) {
                numerator_sqr[i] = (array[i] - mean)*(array[i] - mean);
                std_dev_num_sum += numerator_sqr[i];
            }
            std_dev = sqrt(std_dev_num_sum/(size));
            
            //printing values
            // for(int i = 0; i < size; i++) {
            //     printf("%lf\n", array[i]);
            // }
            continue;
        }
        
        // invalid cmd check
        else if(strcmp(s, "median") != 0 && strcmp(s, "set") != 0 && strcmp(s, "min") != 0 && strcmp(s, "max") != 0 && strcmp(s, "mean") != 0 && strcmp(s, "stddev") != 0 && strcmp(s, "exit") != 0 && strcmp(s, "hist") != 0 && strcmp(s, "summary") != 0 && strcmp(s, "help") != 0) {
            printf("Unknown command. Type 'help' for list of commands");
        }
        
        // initialization check
        else if((array[0] == 0 && array[1] == 0) && (strcmp(s, "median") != 0 || strcmp(s, "set") == 0 || strcmp(s, "min") == 0 || strcmp(s, "max") == 0 || strcmp(s, "mean") == 0 || strcmp(s, "stddev") == 0 || strcmp(s, "hist") == 0 || strcmp(s, "summary") == 0 || strcmp(s, "help") == 0)) {
            printf("Array not initialized. Use 'set' command first.");
            continue;
        }
        
        // min print
        else if(strcmp(s, "min") == 0) {
            printf("%-10s", "Min"); printf(":"); printf("%10.4lf", min);
            continue;
        }
        
        // max print
        else if(strcmp(s, "max") == 0) {
            printf("%-10s", "Max"); printf(":"); printf("%10.4lf", max);
        }
        
        // mean print
        else if(strcmp(s, "mean") == 0) {
            printf("%-10s", "Mean"); printf(":"); printf("%10.4lf", mean);
        }
        
        // std dev print
        else if(strcmp(s, "stddev") == 0) {
            printf("%-10s", "Std Dev"); printf(":"); printf("%10.4lf", std_dev);
        }
        
        // summary print
        else if(strcmp(s, "summary") == 0) {
            printf("%-10s", "Min"); printf(":"); printf("%10.4lf\n", min);
            printf("%-10s", "Max"); printf(":"); printf("%10.4lf\n", max);
            printf("%-10s", "Mean"); printf(":"); printf("%10.4lf\n", mean);
            printf("%-10s", "Std Dev"); printf(":"); printf("%10.4lf", std_dev);
        }

        // hist
        else if(strcmp(s, "hist") == 0) {
            int bin_counts[MAX_BINS] = {0};
            printf("Enter number of bins: "); scanf("%d", &bins);
            double bin_width = (max-min)/bins;

            // star count calculation
            for(int i = 0; i < size; i++) {
                int bin_index = ((int)(array[i] - min) / bin_width);
                if (bin_index == bins) bin_index--;
                bin_counts[bin_index]++;
            }

            // max_frequency indentification
            int max_frequency = bin_counts[0];
            for(int i = 1; i < bins; i++) {
                if(bin_counts[i] > max_frequency) max_frequency = bin_counts[i];
            }

            // rescaling
            if(max_frequency > MAX_STARS) {
                for(int i = 0; i < bins; i++) {
                    bin_counts[i] = (bin_counts[i]*MAX_STARS)/max_frequency;
                }
            }

            // histogram plot
            double low_interval = min, high_interval = min + bin_width;
            for(int j = 0; j < bins; j++) {
                printf("[%7.2lf - %7.2lf]: ", low_interval, high_interval);
                low_interval += bin_width, high_interval += bin_width;

                // star plot
                for(int i = 0; i < bin_counts[j]; i++) {
                    printf("*");
                }
                printf("\n");
            }
        }

        else if(strcmp(s, "median") == 0) {
            // Copy original array
            for(int i = 0; i < size; i++) {
                array_copy[i] = array[i];
            }

            // Sort using qsort
            qsort(array_copy, size, sizeof(double), cmpfunc);

            // median calc
            double median;
            if(size % 2 == 0) {
                median = (array_copy[size/2 - 1] + array_copy[size/2]) / 2.0;
            } else {
            median = array_copy[size/2];
            }

            // median printf
            printf("%-10s", "Median"); printf(":"); printf("%10.4lf", median);
        }

        // exit
        else if(strcmp(s, "exit") == 0) {
            break;
        }
    }
    return 0;
}