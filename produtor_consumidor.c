#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h> 

int x = 0;
int maxValue;

int size;
int *valores;

int countP = 0;
int countC = 0;

sem_t full;
sem_t empty;
sem_t mutex;

void *Produtor(void *arg);
void *Consumidor(void *arg);

int main(void){
    int produtor;
    int consumidor;

    scanf("%d %d %d %d", &produtor, &consumidor, &maxValue, &size);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, size);
    sem_init(&mutex, 0, 1);

    int idProdutor[produtor];
    int idConsumidor[consumidor];

    pthread_t threads[consumidor+produtor];
    valores = (int *)malloc(size * sizeof(int));
    
    for(int i = 0; i < size; i++){
        valores[i] = 0;
    }

    for(int i = 0; i < produtor; i++){
        idProdutor[i] = i;
        pthread_create(&threads[i], NULL, Produtor, &idProdutor[i]);
    }

    for(int i = 0; i < consumidor; i++){
        idConsumidor[i] = i;
        pthread_create(&threads[i], NULL, Consumidor, &idConsumidor[i]);
    }

    for(int i = 0; i < produtor + consumidor; i++){
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

void *Produtor(void *arg){
    int *id = arg;
    while (1){
        sem_wait(&empty);
        sem_wait(&mutex);
        if (valores[countP] == 0){
            valores[countP] = (2 * x) + 1;
            printf("Produtor %d produzindo %d na posição %d\n", *id, valores[countP], countP);
            countP = (countP + 1) % size;
        }
      	x = (x + 1) % maxValue;
          
        sem_post(&full);
        sem_post(&mutex);
    }
}

void *Consumidor(void *arg){
    int *id = arg;
    int consumed;
        while (1){
        sem_wait(&full);
        sem_wait(&mutex);
        if (valores[countC] != 0){
            consumed = valores[countC];
            printf("Consumidor %d consumindo %d da posição %d\n", *id, consumed, countC);
            valores[countC] = 0;
            countC= (countC + 1) % size;
        sem_post(&mutex);
        sem_post(&empty);
        }
    }
}
