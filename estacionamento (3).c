#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_CARS 100 // total de carros


int totalVagasDisponiveis; // espacos livres no estacionamento
int totalVagas;
int* parking;
FILE *file;
int carroId=0;
sem_t mutex;

int* criarEstacionamento(int totalVagas) {
    parking = (int*)malloc(totalVagas*sizeof(int));

    for (int i = 0; i < totalVagas; i++) {
        parking[i] = 0;
    }

    return parking;
}

int procurarVaga(int* parking, int total) {
    for (int j = 0; j < total; j++) {
        if (parking[j] == 0) {
            return j;
        }
    }
}

bool ocuparVaga(int vaga, int* parking) {
    if (parking[vaga] == 1){
        printf("Vaga já ocupada. ERRO!!!\n");
        return false;
    }
    else {
        parking[vaga] = 1;
        totalVagasDisponiveis++;
        return true;
    }
}

bool desocuparVaga(int vaga, int* parking) {
    if (parking[vaga] == 0){
        printf("Vaga já deocupada. ERRO!!!\n");
        return false;
    }
    else {
        parking[vaga] = 0;
        totalVagasDisponiveis--;
        return true;
    }
}

int contarVagasDisponiveis(int* parking, int total) {
    int cont = 0;
    for (int i = 0; i<total; i++) {
        if (parking[i] == 0) cont++;
    }

    return cont;
}

void* assignSpace(void* threadid){

	// bloqueia o mutex pra que outras threads nao usem
    sem_wait(&mutex);

    // procura vaga no estacionamento
    int vaga = procurarVaga(parking, totalVagas);
    printf("THREAD %d: Vaga %d livre. Carro %d vai estacionar...\n", threadid, vaga, threadid);

    // ocupa a vaga encontrada
    ocuparVaga(vaga, parking);
    printf("THREAD %d: Carro %d estacionado. \n", threadid, threadid);

    // conta quantas vagas sobraram
    totalVagasDisponiveis = contarVagasDisponiveis(parking, totalVagas);

    if (fprintf(file, "THREAD %d: Há %d vagas disponíveis\n", threadid, totalVagasDisponiveis)<0)
    {
        perror("\n\nERRO !!!\n\n");
        exit(1);
    }

    // fica um tempo estacionado
    sleep(rand() % 10);

    // desocupa vaga
    desocuparVaga(vaga, parking);

    sem_post(&mutex);

	if (fprintf(file,"Carro [%d] deixando o estacionamento, há %d vagas\n", threadid, totalVagasDisponiveis)<0) {
        perror("\n\nERRO !!!\n\n");
        exit(1);
    };

	pthread_exit(0);
}

int main (int argc, char *argv[]){

	struct timespec start, finish;
	double elapsed;

	if (argc != 3){
		printf("ERRO !!!\nSAINDO... ", argc);
		exit(1);
	}

	printf("Atribuindo numero de espaços para estacionar...");
	totalVagas = atoi(argv[1]);
	printf("PRONTO\n");


	totalVagasDisponiveis = totalVagas;
	printf("Iniciando...");
	int i = 0;

	sem_init(&mutex, 0, totalVagas);
    srand(time(NULL));

    parking = criarEstacionamento(totalVagas);

	printf("FEITO\n");
	printf("O estacionamento tem %d vagas. Todas estão vazias no momento\n",totalVagas);
	printf("Criando arquivo  de saída...\n");	

	const char *textfilename = argv[2];
	if ((file = fopen(textfilename,"a+")) == NULL) {
			perror("ERRO CRIANDO ARQUIVO DE SAÍDA !!!");
			return 1;
	}

	printf("FEITO\n");


	int totalCarros;
	int rc;

    printf("Quantos carros estão tentando entrar no estacionamento?\n");
    scanf("%d",&totalCarros);

    pthread_t threads[100];

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < totalCarros; i++) {
        carroId++;
        printf("Criando thread para o carro %d\n", carroId);
        rc = pthread_create(&threads[i], NULL, assignSpace, (void *)carroId);
        sleep(1);
        if (rc){
            printf("ERROR!!! pthread_create(): %d\n", rc);
            exit(-1);
        }
    }
    
    for (int i = 0; i < totalCarros; i++) {
    	pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;       // tratamento, conversao e soma dos valores de tempo(nanossegundo pra segundo)
	
    printf("Tempo: %.3f segundos.\n", elapsed);
    sleep(10);
    return 0;
}

