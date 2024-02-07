/**
 * @file read_priority.c
 * @brief This program demonstrates the priority of readers over writers using semaphores.
 * 
 * The program creates multiple reader and writer threads. The readers read the shared variable,
 * while the writers modify the shared variable. The readers have priority over the writers,
 * meaning that if there are readers currently reading, the writers have to wait.
 * 
 * The program uses semaphores to synchronize access to the shared variable and to control the
 * priority of readers over writers.
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define READERS 3
#define WRITERS 3

sem_t wrt, rd;
int shared = 1, r_count = 0;

void *writer(void *id);
void *reader(void *id);

int main()
{
    pthread_t read[READERS], write[WRITERS];
    sem_init(&wrt, 0, 1);
    sem_init(&rd, 0, 1);

    int a[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(read[i], NULL);
    }
    for (int i = 0; i < 3; i++)
    {
        pthread_join(write[i], NULL);
    }
    sem_destroy(wrt);
    sem_destroy(rd);
}

/**
 * @brief This function represents a writer thread that modifies a shared variable.
 * 
 * @param id A pointer to the writer thread ID.
 * @return void* This function does not return a value.
 */
void *writer(void *id)
{
    sem_wait(&wrt);
    shared = shared * 2;
    printf("Writer %d modified the shared variable to: %d\n", (*((int *)id)), shared);
    sem_post(&wrt);
}

/**
 * @brief This function represents a reader thread that reads a shared variable.
 * 
 * @param id A pointer to the reader's ID.
 * @return void* Returns NULL when the thread finishes execution.
 */
void *reader(void *id)
{
    sem_wait(&rd);
    r_count++;
    if (r_count == 1)
    {
        sem_wait(&wrt);
    }
    sem_post(&rd);
    printf("Reader %d has read the shared variable: %d\n", (*((int *)id)), shared);

    sem_wait(&rd);
    r_count--;
    if (r_count == 0)
    {
        sem_post(&wrt);
    }
    sem_post(&rd);
}