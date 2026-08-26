#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

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

    return 0;
}