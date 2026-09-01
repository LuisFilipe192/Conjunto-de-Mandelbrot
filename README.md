# Conjunto de Mandelbrot

## Descrição do projeto

O Conjunto de Mandelbrot é um programa desenvolvido em C. Seu objetivo é gerar uma imagem do conjunto de Mandelbrot a partir de pontos do plano complexo.

Para cada ponto da imagem, o programa aplica repetidamente a fórmula z_novo = z_atual² + c, começando com z = 0, até que o número de interações chegue no limite definido ou o ponto escape do conjunto.

O programa possui quatro implementações diferentes para realizar o cálculo: uma implementação Serial, uma implementação com OpenMP e duas implementações utilizando Pthreads, com estratégias diferentes de divisão do trabalho.

Também foi implementado o registro do tempo de execução de cada implementação em um arquivo chamado times.txt.

As imagens geradas pelas quatro implementações utilizam os mesmos parâmetros e elas tem que ser idênticas entre elas.

O projeto foi desenvolvido em C para ambiente Linux.

## Estrutura dos arquivos

O projeto está organizado dessa forma:

- `src/mandelbrot.c` — responsável pela implementação Serial do conjunto de Mandelbrot.
- `src/openMP.c` — responsável pela implementação paralela utilizando OpenMP.
- `src/pthreads1.c` — responsável pela primeira implementação utilizando Pthreads, com divisão das linhas da imagem em blocos.
- `src/pthreads2.c` — responsável pela segunda implementação utilizando Pthreads, com divisão das linhas de forma intercalada.
- `Makefile` — utilizado para compilar e limpar os arquivos compilados.
- `times.txt` — arquivo utilizado para registrar os tempos de execução das implementações.
- `evidencias.log` — arquivo utilizado para registrar os testes realizados durante o desenvolvimento.
- `mandelbrot_lfass_serial.pgm` — imagem gerada pela implementação Serial.
- `mandelbrot_lfass_openMP.pgm` — imagem gerada pela implementação OpenMP.
- `mandelbrot_lfass_pthreads1.pgm` — imagem gerada pela primeira implementação Pthreads.
- `mandelbrot_lfass_pthreads2.pgm` — imagem gerada pela segunda implementação Pthreads.

## Como compilar

Para compilar o projeto, basta executar o comando:

```
make
```

## Como executar

Depois de compilar o projeto, o programa pode ser executado utilizando o seguinte formato:

```
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Para executar as outras implementações:

```
./openMP 800 600 1000 4
./pthreads1 800 600 1000 4
./pthreads2 800 600 1000 4
```

## Implementações

### Serial

A implementação Serial realiza o cálculo de cada pixel da imagem utilizando apenas uma execução sequencial.

O resultado é armazenado no arquivo:

```
mandelbrot_lfass_serial.pgm
```
### OpenMP

A implementação OpenMP utiliza múltiplas threads para dividir o cálculo dos pixels da imagem.

A quantidade de threads utilizada é definida pelo quarto argumento da execução.

O resultado é armazenado no arquivo:

```
mandelbrot_lfass_openMP.pgm
```
### Pthreads 1

A primeira implementação utilizando Pthreads divide a imagem em blocos de linhas.

Cada thread recebe uma parte contínua das linhas da imagem para realizar o cálculo.

O resultado é armazenado no arquivo:

```
mandelbrot_lfass_pthreads1.pgm
```
### Pthreads 2

A segunda implementação utilizando Pthreads utiliza uma estratégia diferente de divisão do trabalho.

As linhas são distribuídas de forma intercalada entre as threads. Por exemplo, utilizando quatro threads:

Thread 0 → linhas 0, 4, 8, 12...
Thread 1 → linhas 1, 5, 9, 13...
Thread 2 → linhas 2, 6, 10, 14...
Thread 3 → linhas 3, 7, 11, 15...

O resultado é armazenado no arquivo:

```
mandelbrot_lfass_pthreads2.pgm
```

## Registro dos tempos

Durante a execução, o programa registra o tempo de cálculo de cada implementação no arquivo:

```
times.txt
```

O arquivo contém os tempos no seguinte formato:
```
Serial: 0.631558 segundos
openMP: 0.419833 segundos
Pthreads1: 0.519666 segundos
Pthreads2: 0.308649 segundos
```

Os tempos podem variar de acordo com o computador e com os parâmetros utilizados na execução.

## Formato da imagem

As imagens são geradas no formato de arquivo .pgm, contendo apenas os valores de intensidade dos pixels, sem cabeçalho.

Cada valor representa a intensidade do pixel entre 0 e 255.

Os valores são normalizados de acordo com o número de iterações realizadas para cada ponto.

As quatro implementações devem produzir exatamente a mesma imagem.

A comparação entre os arquivos pode ser realizada utilizando os comandos:
```
cmp mandelbrot_lfass_serial.pgm mandelbrot_lfass_openMP.pgm
cmp mandelbrot_lfass_serial.pgm mandelbrot_lfass_pthreads1.pgm
cmp mandelbrot_lfass_serial.pgm mandelbrot_lfass_pthreads2.pgm
```

Quando os comandos não apresentam nenhuma mensagem, significa que os arquivos são idênticos.

Tratamento de erros

O programa realiza a validação dos argumentos antes de iniciar a execução.

São tratados casos como:

quantidade incorreta de argumentos;
largura inválida;
altura inválida;
número máximo de iterações inválido;
número de threads inválido;
número fora dos limites permitidos;
falha na alocação de memória;
falha na criação do arquivo de saída;
falha na criação das threads;
falha ao esperar pela execução das threads.

As mensagens de erro são exibidas utilizando stderr, enquanto a execução normal não apresenta mensagens na saída padrão.

Sistema operacional utilizado

O projeto foi desenvolvido e testado no seguinte ambiente:

Sistema operacional: Linux (Ubuntu)
Linguagem: C
Compilador: GCC
OpenMP