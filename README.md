# memcpyVsOMP
Studying the difference between memcpy and parallel copy using OMP


Use the attached makefile to compile. 

To run on a system with a slurm dispatcher use:

export KMP_AFFINITY=scatter,1,0,granularity=fine
export OMP_NUM_THREADS=32   // or whatever your number of physical cores are 

srun --hint=nomultithread --cpus-per-task=32 -n 1  ./benchmark


#######
Output example

OMP Threads 32
Array size (GiB), Array size (MiB), Elapsed time Memcpy (s), Elapsed time OMP (s), Memcpy throughput (GiB/s), OMP throughput (GiB/s)
0.000000003725, 0.0015, 0.0018
0.000000007451, 0.0013, 0.0015
0.000000014901, 0.0000, 0.0139
0.000000029802, 0.0000, 0.0192
0.000000059605, 0.0000, 0.0294
0.000000119209, 0.0000, 0.0588
0.000000238419, 0.0000, 0.1176
0.000000476837, 0.0635, 0.1905
0.000000953674, 0.0000, 0.5000
0.000001907349, 4.0000, 0.9412
0.000003814697, 8.0000, 1.5238
0.000007629395, 8.0000, 3.0476
0.000015258789, 6.0952, 4.9231
0.000030517578, 6.0952, 12.1905
0.000061035156, 7.2113, 20.4800
0.000122070312, 6.4403, 35.3103
0.000244140625, 8.0000, 44.5217
0.000488281250, 8.0789, 57.6901
0.000976562500, 8.1029, 70.0171
0.001953125000, 7.9073, 56.4966
0.003906250000, 8.1371, 77.2830
0.007812500000, 8.2674, 80.1174
0.015625000000, 8.3983, 89.2861
0.031250000000, 14.6131, 83.0095
0.062500000000, 14.6216, 89.4232
0.125000000000, 14.6027, 96.8214
0.250000000000, 14.6045, 99.3864
0.500000000000, 14.6148, 98.2020
1.000000000000, 14.6106, 97.4570
2.000000000000, 14.6752, 99.4182
4.000000000000, 14.8155, 101.5370
8.000000000000, 14.9117, 102.2854
16.000000000000, 14.9542, 102.3162
32.000000000000, 14.9893, 102.2913

