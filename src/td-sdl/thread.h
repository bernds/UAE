 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Threading support, using SDL
  *
  * Copyright 1997, 2001 Bernd Schmidt
  */

#include "SDL.h"
#include "SDL_thread.h"

/* Sempahores. We use POSIX semaphores; if you are porting this to a machine
 * with different ones, make them look like POSIX semaphores. */
typedef SDL_sem *uae_sem_t;

#define uae_sem_init(PSEM, DUMMY, INIT) do { \
    *PSEM = SDL_CreateSemaphore (INIT); \
} while (0)
#define uae_sem_destroy(PSEM) SDL_DestroySemaphore (*PSEM)
#define uae_sem_post(PSEM) SDL_SemPost (*PSEM)
#define uae_sem_wait(PSEM) SDL_SemWait (*PSEM)
#define uae_sem_trywait(PSEM) SDL_SemTryWait (*PSEM)
#define uae_sem_getvalue(PSEM) SDL_SemValue (*PSEM)

#include "commpipe.h"

typedef SDL_Thread *uae_thread_id;
#define BAD_THREAD NULL

static __inline__ int uae_start_thread (void *(*f) (void *), void *arg, uae_thread_id *foo)
{
    *foo = SDL_CreateThread ((int (*)(void *))f, arg);
    return *foo == 0;
}

/* Do nothing; thread exits if thread function returns.  */
#define UAE_THREAD_EXIT do {} while (0)
