#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

pthread_mutex_t the_mutex;
static volatile int glob = 0;   
static void *                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
	pthread_mutex_lock(&the_mutex);
        loc = glob;
        loc++;
        glob = loc;
	pthread_mutex_unlock(&the_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? atoi(argv[1]) : 10000000;

    s = pthread_create(&t1, NULL, threadFunc, &loops);

    if (s != 0)
        printf("pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        printf("pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        printf("pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        printf("pthread_join");

    printf("glob = %d\n", glob);

    exit(EXIT_SUCCESS);
}
