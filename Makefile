# Tariq Alturkestani


CC = CC 
             

CFLAGS =-O3 --std=c++11  -restrict  -fstrict-aliasing -qopt-streaming-cache-evict=0 -qopt-streaming-stores=always -qopt-prefetch-distance=64,8 -qopt-report=5 -qopt-report-phase=vec -qopenmp 
#CFLAGS =-O3 --std=c++11  -qopenmp 

benchmark: main.c
	$(CC) $(CFLAGS)  $(OBJS) $(LIBS) main.c -o $@ 

clean:
	rm -f  benchmark a.out *.o *.out *.err  *.optrpt *~
