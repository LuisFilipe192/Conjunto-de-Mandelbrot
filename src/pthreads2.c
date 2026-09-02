#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

typedef struct dadosthread {
    unsigned char *imagem;
    int largura;
    int altura;
    int inicio;
    int fim;
    int contagem;
} dadosthread;

void *calcular(void *arg) {
    dadosthread *dados = (dadosthread *)arg;

    dados->contagem = 0;

    for(int y=0;y < dados->altura; y++){
        for(int x=0;x < dados->largura;x++){
            int intensidade = dados->imagem[y* dados->largura + x];

            if(intensidade >= dados->inicio &&intensidade <= dados->fim){
                dados->contagem++;
            }
        }
    }

    return NULL;
}

void registrar_erro(char *mensagem){
    FILE *arquivo = fopen("erros.txt","a");

    if(arquivo != NULL){
        fprintf(arquivo,"%s\n", mensagem);
        fclose(arquivo);
    }
}

int main(int argc, char *argv[]){

    long valor;

    if(argc != 5){
        registrar_erro("Erro: argumentos invalidos.");
        return 1;
    }

    errno = 0;
    char *end;

    valor = strtol(argv[1],&end,10);

    if(*end != '\0'){
        registrar_erro("Erro: argumento da largura invalido.");
        return 1;
    }

    if(errno == ERANGE){
        registrar_erro("Erro: numero fora do limite permitido.");
        return 1;
    }

    if(valor > INT_MAX){
        registrar_erro("Erro: largura passou o limite maximo.");
        return 1;
    }

    if(valor < INT_MIN){
        registrar_erro("Erro: largura passou o limite minimo.");
        return 1;
    }

    if(valor <= 1){
        registrar_erro("Erro: largura deve ser maior que um.");
        return 1;
    }

    int largura = (int)valor;

    errno = 0;

    valor = strtol(argv[2],&end,10);

    if(*end != '\0'){
        registrar_erro("Erro: argumento da altura invalido.");
        return 1;
    }

    if(errno == ERANGE){
        registrar_erro("Erro: numero fora do limite permitido.");
        return 1;
    }

    if(valor > INT_MAX){
       registrar_erro("Erro: altura passou o limite maximo.");
        return 1;
    }

    if(valor < INT_MIN){
        registrar_erro("Erro: altura passou o limite minimo.");
        return 1;
    }

    if(valor <= 1){
        registrar_erro("Erro: altura deve ser maior que um.");
        return 1;
    }

    int altura = (int)valor;

    errno = 0;

    valor = strtol(argv[3],&end,10);

    if(*end != '\0'){
        registrar_erro("Erro: argumento de max_iteracoes invalido.");
        return 1;
    }

    if(errno == ERANGE){
        registrar_erro("Erro: numero fora do limite permitido.");
        return 1;
    }

    if(valor > INT_MAX){
        registrar_erro("Erro: max_iteracoes passou o limite maximo.");
        return 1;
    }

    if(valor < INT_MIN){
        registrar_erro("Erro: max_iteracoes passou o limite minimo.");
        return 1;
    }

    if(valor <= 0){
        registrar_erro("Erro: max_iteracoes deve ser maior que zero.");
        return 1;
    }

    int max_iteracoes = (int)valor;

    errno = 0;

    valor = strtol(argv[4],&end,10);

    if(*end != '\0'){
        registrar_erro("Erro: argumento de num_threads invalido.");
        return 1;
    }

    if(errno == ERANGE){
        registrar_erro("Erro: numero fora do limite permitido.");
        return 1;
    }

    if(valor > INT_MAX){
        registrar_erro("Erro: num_threads passou o limite maximo.");
        return 1;
    }

    if(valor < INT_MIN){
        registrar_erro("Erro: num_threads passou o limite minimo.");
        return 1;
    }

    if(valor <= 0){
        registrar_erro("Erro: num_threads deve ser maior que zero.");
        return 1;
    }

    int num_threads = (int)valor;

    unsigned char *imagem = malloc(largura*altura * sizeof(unsigned char));

    if(imagem == NULL){
        registrar_erro("Erro: falha na alocacao de memoria.");
        return 1;
    }

    struct timespec inicio;
    struct timespec fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for(int y = 0; y < altura; y++){
        for(int x = 0; x < largura; x++){

            double real = -2.0 + x * 3.0/(largura-1);
            double imag = -1.5 + y * 3.0/(altura-1);

            double z_real = 0.0;
            double z_imag = 0.0;

            int iteracoes = 0;

            while(iteracoes < max_iteracoes){
                double novo_real =z_real * z_real - z_imag * z_imag +real;

                double novo_imag =2.0 * z_real * z_imag +imag;

                z_real = novo_real;
                z_imag = novo_imag;

                iteracoes++;

                if(z_real * z_real +z_imag * z_imag > 4.0){
                    break;
                }
            }

            int intensidade =(int)((iteracoes * 255.0)/max_iteracoes);

            imagem[y*largura + x] = intensidade;
        }
    }


    pthread_t threads[num_threads];
    dadosthread dados[num_threads];

    int tamanho_faixa = 256/num_threads;

    for(int i = 0; i < num_threads; i++){

        dados[i].imagem = imagem;
        dados[i].largura = largura;
        dados[i].altura = altura;

        dados[i].inicio = i * tamanho_faixa;

        if (i == num_threads - 1) {
            dados[i].fim = 255;
        } else {
            dados[i].fim =
                (i + 1) * tamanho_faixa - 1;
        }

        dados[i].contagem = 0;

        if(pthread_create(&threads[i],NULL,calcular,&dados[i]) != 0){
            registrar_erro("Erro: falha na criacao da thread.");
            free(imagem);
            return 1;
        }
    }

    for (int i=0;i < num_threads;i++){
        if(pthread_join(threads[i], NULL) != 0){
            registrar_erro("Erro: falha ao esperar pela thread.");

            free(imagem);
            return 1;
        }
    }

    clock_gettime(CLOCK_MONOTONIC,&fim);

    double tempo =(fim.tv_sec - inicio.tv_sec) +(fim.tv_nsec - inicio.tv_nsec)/1000000000.0;

    FILE *times = fopen("times.txt","a");

    if(times == NULL){
        registrar_erro("Erro: falha na criacao do arquivo times.txt.");

        free(imagem);
        return 1;
    }

    fprintf(times,"Pthreads2: %.6f segundos\n",tempo);

    fclose(times);

    FILE *arquivo =fopen("mandelbrot_lfass_pthreads2.pgm","w");

    if(arquivo == NULL){
        registrar_erro("Erro: falha na criacao do arquivo de saida.");

        free(imagem);
        return 1;
    }

    for(int y=0;y < altura;y++){
        for(int x=0;x < largura;x++){
            fprintf(arquivo,"%d",imagem[y*largura + x]);

            if (x < largura-1) {
                fprintf(arquivo," ");
            }
        }

        fprintf(arquivo,"\n");
    }

    fclose(arquivo);
    free(imagem);

    return 0;
}