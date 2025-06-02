#include "gemmini_semaphore.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

sem_t *gemmini_sem = NULL;
static pthread_once_t gemmini_init_once = PTHREAD_ONCE_INIT;

static void gemmini_semaphore_init_once(void) {
    gemmini_sem = sem_open("/gemmini_mutex", O_CREAT, 0666, 1);
    if (gemmini_sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
}

// This macro ensures the semaphore is initialized before any use
#define ENSURE_GEMMINI_MUTEX() pthread_once(&gemmini_init_once, gemmini_mutex_init_once)

void gemmini_mutex_destroy(void) {
    if (gemmini_sem != NULL) {
        sem_close(gemmini_sem);
        // Optional: sem_unlink("/gemmini_mutex");
        gemmini_sem = NULL;
    }
}
