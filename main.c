// Tariq Alturkestani
// memcpy vs. omp
// 26-03-2019
// In this code I try to find the throughtput difference between memcopy and OMP parallel for loops
// memcpy uses only one kernel thread (1 CPU core) and with OMP we can use all available cores

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>


int main ()
{
     MPI_Init(NULL, NULL);
    
    const int64_t _1_gib = 1073741824;
    const int64_t _1_mib = 1048576;
    const int testingUpTo = 32;
    float *src, *dest;  
    
    int pageSize=getpagesize();
    const int maxNumThreads = omp_get_max_threads();
    
         
    
    double start, end;
    double ** memcpyThroghputs;
    double ** ompThroghputs;
    double * memcpyThroghputsAVG;
    double * ompThroghputsAVG;
    double ** elapsedTimeMemcpy;
    double ** elapsedTimeOMP;
    double * arraySizesInGib;
    int trials = 10; // skipping the first one
    int numOfTests =0;
    int testNum = 0;
    double dummy;
    
    for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
    {
        numOfTests++;
    }
    
    
    arraySizesInGib = (double * ) malloc(sizeof(double) * numOfTests);
    memcpyThroghputsAVG = (double * ) malloc(sizeof(double) * numOfTests);
    ompThroghputsAVG = (double * ) malloc(sizeof(double) * numOfTests);
    
    for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
    {
        arraySizesInGib[testNum++] = i/(double)_1_gib;
    }
    
    testNum = 0;
    
    // allocation stats arrays
    memcpyThroghputs = (double ** ) malloc(sizeof(double) * trials);
    ompThroghputs = (double ** ) malloc(sizeof(double) * trials);
    
    elapsedTimeMemcpy = (double ** ) malloc(sizeof(double) * trials);
    elapsedTimeOMP = (double ** ) malloc(sizeof(double) * trials);
    
    for ( int i =0; i< trials; i++)
    {
        memcpyThroghputs[i] = (double * ) malloc(sizeof(double) * numOfTests);
        ompThroghputs[i] = (double * ) malloc(sizeof(double) * numOfTests);
        
        elapsedTimeMemcpy[i] = (double * ) malloc(sizeof(double) * numOfTests);
        elapsedTimeOMP[i] = (double * ) malloc(sizeof(double) * numOfTests);
    }
    
    
    // testing the throughput difference using different array sizes.
    // copying 4 bytes is way different that copying 32 GiB! 
    
    for (int k = 0; k < trials ; k++)
    {
        for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
        {
            src = (float*) malloc(i);
            dest = (float*) malloc(i);
                        
            // first touch
            #pragma omp parallel for 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                src[j] = 3.14*j/(float)i; // some random number 
            }
            // first touch
            #pragma omp parallel for 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                dest[j] = 0.0;
            }
            
            
            // memcpy test
            start=omp_get_wtime();
            memcpy(dest, src, i);
            dummy=dest[i/sizeof(float)-1];
            end=omp_get_wtime();
            
            if (end-start > 0.000000001)
            {
                memcpyThroghputs[k][testNum] = 2.0*arraySizesInGib[testNum]/(end-start);
            }
            else
            {
                memcpyThroghputs[k][testNum] = 0.0f;
            }

            elapsedTimeMemcpy[k][testNum] = (end-start);
            
            
            // summing the array and outputing it to avoid any funny compiler business 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                dummy += dest[j];
            }
        
            fprintf(stderr,"IGNORE %f Test %d out of %d\n", dummy, testNum+1, numOfTests); 
        
            free(src);
            free(dest);
            testNum++;
        }
        testNum = 0;
    }
    
    testNum = 0;
    
    for (int k = 0; k < trials ; k++)
    {
        for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
        {     
            src = (float*) malloc(i);
            dest = (float*) malloc(i);
                    
            // first touch
            #pragma omp parallel for 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                src[j] = 3.14*j/(float)i; // some random number 
            }
            // first touch
            #pragma omp parallel for 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                dest[j] = 0.0;
            }
            
            // omp test
            start=omp_get_wtime();
            #pragma omp parallel for 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                dest[j] = src[j];               
            }
            dummy=dest[i/sizeof(float)-1];
            end=omp_get_wtime();
            
            if (end-start > 0.000000001)
            {
                ompThroghputs[k][testNum] = 2.0*arraySizesInGib[testNum]/(end-start);
            }
            else
            {
                ompThroghputs[k][testNum] = 0.0f;
            }
            
            elapsedTimeOMP[k][testNum] = (end-start);
            
            // summing the array and outputing it to avoid any funny compiler business 
            for (int64_t j = 0; j < i/sizeof(float) ; j++)
            {
                dummy += dest[j];
            }
            
            fprintf(stderr,"IGNORE. %f Test %d out of %d\n", dummy, testNum+1, numOfTests); 
            
            free(src);
            free(dest);
            testNum++;
        }
        testNum = 0;
    }
    
    printf("#OMP Threads %d, trials %d \n", maxNumThreads, trials-1);
    printf("Array size (GiB), Memcpy throughput AVG (GiB/s), OMP throughput AVG (GiB/s)\n");
    for ( int i =0 ; i < numOfTests; i++)
    {
        double memCpyTemp=0;
        double ompTemp=0;
        for ( int j = 1; j < trials ; j++)
        {
            memCpyTemp+=memcpyThroghputs[j][i];
            ompTemp+=ompThroghputs[j][i];
        }
        
        memcpyThroghputsAVG[i]= memCpyTemp/(trials-1.0);
        ompThroghputsAVG[i]= ompTemp/(trials-1.0);
        
        printf("%.12f, %.4f, %.4f\n", arraySizesInGib[i], memcpyThroghputsAVG[i],  ompThroghputsAVG[i] );
    }

    MPI_Finalize();
    return 0;
}
