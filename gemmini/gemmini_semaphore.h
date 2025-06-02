#ifndef GEMMINI_SEMAPHORE_H
#define GEMMINI_SEMAPHORE_H

#include <semaphore.h>

extern sem_t *gemmini_sem;

void gemmini_mutex_destroy(void);

#endif // GEMMINI_MUTEX_H
