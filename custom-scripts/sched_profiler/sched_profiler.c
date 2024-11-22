#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sched.h>
#include "../setpriority/setpriority.h" // Inclua o cabeçalho aqui

#define SCHED_LOW_IDLE 7
#define SCHED_IDLE 5
#define SCHED_RR 2
#define SCHED_FIFO 1

pthread_barrier_t barrier;
pthread_mutex_t mutex;
char* buffer;
int counter;
int max_size;
sem_t sem;

void* write(void* arg) {
    char character = *(char*)arg;
    pthread_barrier_wait(&barrier);

    while (1) {
        sem_wait(&sem);
        if (counter >= max_size) {
            sem_post(&sem);
            break;
        }
        buffer[counter++] = character;
        sem_post(&sem);
    }
    return NULL;
}

void post_process_buffer(char* buffer, int size) {
    char last_char = buffer[0];
    int count = 1;
    int counts[26] = {0}; // Para contar ocorrências de A-Z
    counts[last_char - 'A']++;

    for (int i = 1; i < size; i++) {
        if (buffer[i] != last_char) {
            printf("%c", last_char);
            last_char = buffer[i];
            count++;
        }
        counts[buffer[i] - 'A']++;
    }
    printf("%c\n", last_char);

    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c = %d\n", 'A' + i, counts[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Uso: %s <tamanho_buffer> <num_threads> <policy>\n", argv[0]);
        return 1;
    }

    int tamanho_buffer = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    char* policy_str = argv[3];
    int policy = atoi(policy_str);
    printf("Policy: %d\n", policy);


    pthread_t threads[num_threads];
    char characters[num_threads];
    buffer = (char*)malloc(tamanho_buffer * sizeof(char));
    max_size = tamanho_buffer;
    counter = 0;

    if (pthread_mutex_init(&mutex, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }

    pthread_barrier_init(&barrier, NULL, num_threads);
    sem_init(&sem, 0, 1);

    for (int i = 0; i < num_threads; i++) {
        characters[i] = 'A' + i;
        pthread_create(&threads[i], NULL, write, &characters[i]);
        setpriority(&threads[i], policy, sched_get_priority_max(policy));
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    sem_destroy(&sem);

    printf("Buffer sem pós-processamento:\n");
    for (int i = 0; i < tamanho_buffer; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    printf("Buffer após pós-processamento:\n");
    post_process_buffer(buffer, tamanho_buffer);
    printf("Policy : \n");
    print_sched(policy);

    free(buffer);
    return 0;
}