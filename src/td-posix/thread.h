 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Threading support, using pthreads
  *
  * Copyright 1997 Bernd Schmidt
  */

#include <pthread.h>
#include <semaphore.h>

/* Sempahores. We use POSIX semaphores; if you are porting this to a machine
 * with different ones, make them look like POSIX semaphores. */
typedef sem_t uae_sem_t;
#define uae_sem_init sem_init
#define uae_sem_destroy sem_destroy
#define uae_sem_post sem_post
#define uae_sem_wait sem_wait
#define uae_sem_trywait sem_trywait
#define uae_sem_getvalue sem_getvalue

#include "commpipe.h"

typedef pthread_t uae_thread_id;
#define BAD_THREAD -1

static __inline__ int uae_start_thread (void *(*f) (void *), void *arg, uae_thread_id *foo)
{
    return pthread_create (foo, 0, f, arg);
}

STATIC_INLINE int uae_wait_thread (uae_thread_id thread)
{
    return pthread_join (thread, (void**)0);
}

#define UAE_THREAD_EXIT pthread_exit(0)
