/**
 * @file writer_priority.c
 * @brief This file contains the implementation of a program that demonstrates writer priority in a multi-threaded environment.
 * 
 * The program creates multiple reader and writer threads. The readers read a shared variable, while the writers modify the shared variable.
 * The program uses semaphores to control access to the shared variable, ensuring that only one writer can modify it at a time.
 * 
 * The program demonstrates writer priority, where if a writer is waiting to write, no new readers are allowed to start reading until the writer has finished writing.
 * This ensures that writers are given priority over readers to prevent starvation.
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define READERS 3
#define WRITERS 3

sem_t wrt, rd, n_rd, n_wrt;
int shared = 1, r_count = 0, w_count = 0;

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
 * @param id A pointer to the ID of the writer thread.
 * @return void* Returns NULL.
 */
void *writer(void *id)
{
    sleep(rand()%3);
    // Acquire the write lock
    sem_wait(&wrt);
    w_count++;

    // If this is the first writer, block all readers
    if (w_count == 1)
    {
        sem_wait(&n_rd);
    }
    sem_post(&wrt);

    // Perform the write operation
    sem_wait(n_wrt);
    shared = shared * 2;
    printf("Writer %d modified the shared variable to : %d | %d\n", (*((int *)id)), shared, (unsigned)time(NULL));
    sem_post(&n_wrt);

    // Release the write lock
    sem_wait(&wrt);
    w_count--;

    // If there are no more writers, allow readers to proceed
    if (w_count == 0)
    {
        sem_post(&n_rd);
    }
    sem_post(&wrt);

    return NULL;
}

/**
 * @brief This function represents a reader thread that reads a shared variable.
 * 
 * @param id A pointer to the unique identifier of the reader thread.
 * @return void* This function does not return a value.
 */
void *reader(void *id)
{
    sleep(rand()%3);
    sem_wait(&n_rd);
    sem_wait(&rd);
    r_count++;
    if (r_count == 1)
    {
        sem_wait(&n_wrt);
    }
    sem_post(&rd);
    sem_post(&n_rd);
    printf("Reader %d has read the shared variable : %d | %d\n", (*((int *)id)), shared,(unsigned)time(NULL));

    sem_wait(&rd);
    r_count--;
    if (r_count == 0)
    {
        sem_post(&n_wrt);
    }
    sem_post(&rd);
}
// If writer need not to wait

// void *reader(void *id)
// {
//     sleep(rand()%5);

//     sem_wait(&n_rd);
//     sem_wait(&rd);
//     r_count++;
//     sem_wait(&n_wrt);
//     sem_post(&rd);
//     sem_post(&n_rd);
//     printf("Reader %d has read the shared variable : %d | %d\n", (*((int *)id)), shared,(unsigned)time(NULL));

//     sem_wait(&rd);
//     r_count--;
//     sem_post(&n_wrt);
//     sem_post(&rd);
// }