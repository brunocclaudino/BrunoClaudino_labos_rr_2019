#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define CHAIRS 5
#define TRUE 1

sem_t customers;
sem_t barbers;
sem_t mutex;
int waiting = 0;

void *barber(void *arg);
void *customer(void *arg);
void cut_hair();
void customer_arrived();
void get_haircut();
void giveup_haircut();

int main()
{
    sem_init(&customers, TRUE, 0);
    sem_init(&barbers, TRUE, 0);
    sem_init(&mutex, TRUE, 1);

    pthread_t b, c;

    pthread_create(&b, NULL, (void *)barber, NULL);

    while (TRUE)
    {
        pthread_create(&c, NULL, (void *)customer, NULL);
        sleep(1);
    }

    return 0;
}

void *barber(void *arg)
{
    while (TRUE)
    {
        sem_wait(&customers);
        sem_wait(&mutex);
        waiting = waiting - 1;
        sem_post(&barbers);
        sem_post(&mutex);
        cut_hair();
    }

    pthread_exit(NULL);
}

void *customer(void *arg)
{
    sem_wait(&mutex);

    if (waiting < CHAIRS)
    {
        customer_arrived();
        waiting = waiting + 1;
        sem_post(&customers);
        sem_post(&mutex);
        sem_wait(&barbers);
        get_haircut();
    }
    else
    {
        sem_post(&mutex);
        giveup_haircut();
    }

    pthread_exit(NULL);
}

void cut_hair()
{
    printf("Barbeiro estah cortando o cabelo de alguem!\n");
    sleep(3);
}

void customer_arrived()
{
    printf("Cliente chegou para cortar cabelo!\n");
}
void get_haircut()
{
    printf("Cliente estah tendo o cabelo cortado!\n");
}

void giveup_haircut()
{
    printf("Cliente desistiu! (O salao estah muito cheio!)\n");
}