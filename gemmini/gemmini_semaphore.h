#ifndef GEMMINI_SEMAPHORE_H
#define GEMMINI_SEMAPHORE_H

#include <semaphore.h>
#include <pthread.h>

extern sem_t *gemmini_sem;

void gemmini_semaphore_destroy(void);

void gemmini_semaphore_init_once(void);

// Ensure macro is available everywhere
extern pthread_once_t gemmini_init_once;
#define ENSURE_GEMMINI_SEMAPHORE() pthread_once(&gemmini_init_once, gemmini_semaphore_init_once)


#endif // GEMMINI_SEMAPHORE_H
