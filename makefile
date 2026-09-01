all: mandelbrot openMP pthreads1 pthreads2

mandelbrot:
	gcc src/mandelbrot.c -o mandelbrot

openMP:
	gcc src/openMP.c -o openMP -fopenmp

pthreads1:
	gcc src/pthreads1.c -o pthreads1 -pthread

pthreads2:
	gcc src/pthreads2.c -o pthreads2 -pthread

clean:
	rm -f mandelbrot openMP pthreads1 pthreads2
	rm -f *.pgm times.txt