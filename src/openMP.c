#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

#include <omp.h>

int main(int argc, char *argv[]){
    
    long valor;

    if(argc != 5){
        fprintf(stderr,"Erro: argumentos invalidos\n");
        return 1;
    }

// largura
    errno = 0;

    char *end;
    valor = strtol(argv[1], &end, 10);

    if(*end != '\0'){
        fprintf(stderr, "Erro: argumento da largura invalido.\n");
        return 1;
    }

    if(errno == ERANGE){
        fprintf(stderr, "Erro: numero fora do limite permitido.\n");
        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: valor passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: valor passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 0){
        fprintf(stderr, "Erro: largura deve ser maior que zero.\n");
        return 1;
    }

    int largura = (int)valor;

// altura
    errno = 0;
    valor = strtol(argv[2], &end, 10);

    if(*end != '\0'){
        fprintf(stderr, "Erro: argumento da altura invalido.\n");
        return 1;
    }

    if(errno == ERANGE){
        fprintf(stderr, "Erro: numero fora do limite permitido.\n");
        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: valor passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: valor passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 0){
        fprintf(stderr, "Erro: altura deve ser maior que zero.\n");
        return 1;
    }

    int altura = (int)valor;

    // max_iteracoes

    errno = 0;

    valor = strtol(argv[3], &end, 10);

    if(*end != '\0'){

        fprintf(stderr, "Erro: argumento de max_iteracoes invalido.\n");

        return 1;
    }

    if(errno == ERANGE){

        fprintf(stderr, "Erro: numero fora do limite permitido.\n");

        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: max_iteracoes passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: max_iteracoes passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 0){
        fprintf(stderr, "Erro: max_iteracoes deve ser maior que zero.\n");
        return 1;
    }

    int max_iteracoes = (int)valor;


    //num_threads
    errno = 0;
    valor = strtol(argv[4], &end, 10);

    if(*end != '\0'){
        fprintf(stderr, "Erro: argumento de num_threads invalido.\n");
        return 1;
    }

    if(errno == ERANGE){
        fprintf(stderr, "Erro: numero fora do limite permitido.\n");
        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: num_threads passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: num_threads passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 0){
        fprintf(stderr, "Erro: num_threads deve ser maior que zero.\n");
        return 1;
    }

    int num_threads = (int)valor;

    struct timespec inicio;
    struct timespec fim;

    unsigned char *imagem =malloc(largura * altura * sizeof(unsigned char));

    if(imagem == NULL){

        fprintf(stderr,"Erro: falha na alocacao de memoria.\n");

        return 1;
    }

    omp_set_num_threads(num_threads);

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    #pragma omp parallel for

    for(int y=0;y<altura;y++){
        for(int x=0;x<largura;x++){

            double real = -2.0 + x * 3.0/(largura-1);
            double imag = -1.5 + y * 3.0/(altura-1);

            double z_real = 0.0;
            double z_imag = 0.0;

            int iteracoes =0;

            while(iteracoes<max_iteracoes){

                double novo_real = z_real*z_real-z_imag*z_imag+real;
                double novo_imag = 2.0*z_real*z_imag+imag;

                z_real = novo_real;
                z_imag = novo_imag;

                iteracoes++;

                if(z_real*z_real + z_imag*z_imag > 4.0){
                    break;
                }
            }

            int intensidade = (int)((iteracoes*255.0)/max_iteracoes);

            imagem[y*largura + x] =intensidade;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec)+(fim.tv_nsec - inicio.tv_nsec)/1000000000.0;

    FILE *times = fopen("times.txt", "a");

    if(times==NULL){
        fprintf(stderr,"Erro: falha na criacao do arquivo times.txt.\n");
        free(imagem);
        return 1;
    }

    fprintf(times, "openMP: %.6f segundos\n", tempo);
    fclose(times);

    FILE *arquivo = fopen("mandelbrot_lfass_openmp.pgm","w");

    if(arquivo == NULL){
        fprintf(stderr, "Erro: falha na criacao do arquivo de saida.\n");
        free(imagem);

        return 1;
    }

    for(int y=0;y<altura;y++){
        for(int x=0;x<largura;x++){

            fprintf(arquivo,"%d",imagem[y*largura + x]);

            if(x < largura-1){
                fprintf(arquivo," ");
            }
        }

        fprintf(arquivo,"\n");
    }

    fclose(arquivo);


    return 0;
}