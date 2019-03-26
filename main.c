// Tariq Alturkestani
// memcpy vs. omp
// 26-03-2019
// In this code I try to find the throught difference between memcopy and OMP parallel for loops
// memcpy uses only one kernel thread (1 CPU core) and with OMP we can use all avaiable cores

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>


int main ()
{
    
    const int64_t _1_gib = 1073741824;
    const int64_t _1_mib = 1048576;
    const int testingUpTo = 32;
    float *src, *dest;  
    
    int pageSize=getpagesize();
    const int maxNumThreads = omp_get_max_threads();
    
         
    
    double start, end;
    double * memcpyThroghputs;
    double * ompThroghputs;
    double * arraySizesInGib;
    int numOfTests =1;
    int testNum = 0;
    int64_t dummy;
    
    for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
    {
        numOfTests++;
    }
    
    // allocation stats arrays
    memcpyThroghputs = (double * ) malloc(sizeof(double) * numOfTests);
    ompThroghputs = (double * ) malloc(sizeof(double) * numOfTests);
    arraySizesInGib = (double * ) malloc(sizeof(double) * numOfTests);
    
    
    // testting the throughput difference using different array sizes.
    // copying 4 bytes is way different that copying 32 GiB! 
    
    for (int64_t i = 4; i <= testingUpTo * _1_gib; i*=2)
    {
        src = (float*) malloc(i);
        dest = (float*) malloc(i);
        
        arraySizesInGib[testNum] = i/(double)_1_gib;
        
        // first touch
        #pragma omp parallel for 
        for (int64_t j = 0; j < i/sizeof(float) ; j++)
        {
            src[j] = 1.0;
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
        end=omp_get_wtime();
        
        memcpyThroghputs[testNum] = arraySizesInGib[testNum]/(end-start);
        
        dummy=0;
        // resetting dest array
        for (int64_t j = 0; j < i/sizeof(float) ; j++)
        {
            dummy += dest[j];
            dest[j] = 0.0;
        }
        
        fprintf(stderr,"IGNORE: this output is here to prevent the compiler from deleting the copy operation. %ld\n", dummy); 
        
        
        // omp test
        start=omp_get_wtime();
        #pragma omp parallel for 
        for (int64_t j = 0; j < i/sizeof(float) ; j++)
        {
            dest[j] = src[j];               
        }
        end=omp_get_wtime();
        
        ompThroghputs[testNum] = arraySizesInGib[testNum]/(end-start);
        
        dummy=0;
        // resetting dest array
        for (int64_t j = 0; j < i/sizeof(float) ; j++)
        {
            dummy += dest[j];
            dest[j] = 0.0;
        }
        
        fprintf(stderr, "IGNORE: this output is here to prevent the compiler from deleting the copy operation. %ld\n", dummy); 
        
        free(src);
        free(dest);
        testNum++;
    }
    
    printf("OMP Threads %d\n", maxNumThreads);
    printf("Array size (Gib), Memcopy throughput, OMP throughput\n");
    for ( int i =0 ; i < testNum; i++)
    {
        printf("%.8f, %.4f, %.4f\n", arraySizesInGib[i], memcpyThroghputs[i], ompThroghputs[i] );
    }


    return 0;
}
