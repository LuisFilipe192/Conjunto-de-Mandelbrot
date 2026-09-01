#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

typedef struct dadosthread{
    unsigned char *imagem;
    int largura;
    int altura;
    int max_iteracoes;
    int id;
    int num_threads;
}dadosthread;

void *calcular(void *arg){
    dadosthread *dados = (dadosthread *)arg;

    for(int y=dados->id;y < dados->altura;y+= dados->num_threads){
        for(int x=0;x < dados->largura;x++){

            double real =-2.0 + x * 3.0/(dados->largura-1);

            double imag =-1.5 + y * 3.0/(dados->altura-1);

            double z_real =0.0;

            double z_imag =0.0;

            int iteracoes =0;

            while(iteracoes < dados->max_iteracoes){

                double novo_real = z_real*z_real - z_imag*z_imag + real;

                double novo_imag = 2.0*z_real*z_imag + imag;

                z_real = novo_real;

                z_imag = novo_imag;

                iteracoes++;

                if(z_real*z_real + z_imag*z_imag > 4.0){
                    break;
                }
            }

            int intensidade = (int)((iteracoes*255.0)/dados->max_iteracoes);

            dados->imagem[y*dados->largura + x] = intensidade;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){

    long valor;

    if(argc != 5){
        fprintf(stderr, "Erro: argumentos invalidos\n");
        return 1;
    }

    errno = 0;

    char *end;

    valor = strtol(argv[1],&end,10);

    if(*end != '\0'){
        fprintf(stderr, "Erro: argumento da largura invalido.\n");
        return 1;
    }

    if(errno == ERANGE){
        fprintf(stderr, "Erro: numero fora do limite permitido.\n");
        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: largura passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: largura passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 1){
        fprintf(stderr, "Erro: largura deve ser maior que um.\n");
        return 1;
    }

    int largura = (int)valor;

    errno = 0;

    valor = strtol(argv[2],&end,10);

    if(*end != '\0'){
        fprintf(stderr, "Erro: argumento da altura invalido.\n");
        return 1;
    }

    if(errno == ERANGE){
        fprintf(stderr, "Erro: numero fora do limite permitido.\n");
        return 1;
    }

    if(valor > INT_MAX){
        fprintf(stderr, "Erro: altura passou o limite maximo.\n");
        return 1;
    }

    if(valor < INT_MIN){
        fprintf(stderr, "Erro: altura passou o limite minimo.\n");
        return 1;
    }

    if(valor <= 1){
        fprintf(stderr, "Erro: altura deve ser maior que um.\n");
        return 1;
    }

    int altura = (int)valor;

    errno = 0;

    valor = strtol(argv[3],&end,10);

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

    errno = 0;

    valor = strtol(argv[4],&end,10);

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

    unsigned char *imagem = malloc(largura*altura * sizeof(unsigned char));

    if(imagem == NULL){
        fprintf(stderr, "Erro: falha na alocacao de memoria.\n");
        return 1;
    }


    pthread_t threads[num_threads];

    dadosthread dados[num_threads];

    struct timespec inicio;

    struct timespec fim;


    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for(int i = 0;i < num_threads;i++){

        dados[i].imagem = imagem;
        dados[i].largura = largura;
        dados[i].altura = altura;
        dados[i].max_iteracoes = max_iteracoes;
        dados[i].id = i;
        dados[i].num_threads = num_threads;

        if(pthread_create(&threads[i], NULL, calcular,&dados[i])!=0){
            fprintf(stderr,"Erro: falha na criacao da thread.\n");
            free(imagem);
            return 1;
        }
    }

    for(int i=0;i < num_threads;i++){

        if(pthread_join(threads[i],NULL)!= 0){
            fprintf(stderr, "Erro: falha ao esperar pela thread.\n");
            free(imagem);

            return 1;
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec)+ (fim.tv_nsec -inicio.tv_nsec)/1000000000.0;

    FILE *times = fopen("times.txt","a");

    if(times == NULL){
        fprintf(stderr,"Erro: falha na criacao do arquivo times.txt.\n");
        free(imagem);

        return 1;
    }

    fprintf(times,"Pthreads2: %.6f segundos\n",tempo);

    fclose(times);

    FILE *arquivo = fopen("mandelbrot_lfass_pthreads2.pgm","w");

    if(arquivo == NULL){
        fprintf(stderr,"Erro: falha na criacao do arquivo de saida.\n");
        free(imagem);

        return 1;
    }

    for(int y=0;y < altura;y++){
        for(int x=0;x < largura;x++){
            fprintf(arquivo,"%d",imagem[y*largura + x]);

            if(x < largura - 1){
                fprintf(arquivo, " ");
            }
        }

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    free(imagem);

    return 0;
}