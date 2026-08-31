all: mandelbrot openMP

mandelbrot:
	gcc src/mandelbrot.c -o mandelbrot

openMP:
	gcc src/openMP.c -o openMP -fopenmp

clean:
	rm -f mandelbrot openMP