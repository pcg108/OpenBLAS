#include "gemmini_semaphore.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

sem_t *gemmini_sem = NULL;
pthread_once_t gemmini_init_once = PTHREAD_ONCE_INIT;

void gemmini_semaphore_init_once(void) {
    gemmini_sem = sem_open("/gemmini_mutex", O_CREAT, 0666, 1);
    if (gemmini_sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
}

void gemmini_sempahore_destroy(void) {
    if (gemmini_sem != NULL) {
        sem_close(gemmini_sem);
        gemmini_sem = NULL;
    }
}
