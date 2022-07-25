#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num=1;
int MAX=51;

pthread_mutex_t mtx;
pthread_cond_t cond;

/* Again no issue without sync */
void *print_even(void *arg){
   while(num<MAX){
        if(!(num&1)){
            printf("\nEven num: %d", num);
            num++;
        }
   }
    pthread_exit(0);
}

void *print_odd(void *arg){
   while(num<MAX){
        if(num&1){
            printf("\nOdd num: %d", num);
            num++;
        }
   }

   pthread_exit(0);
}

/* Threads with sync mechanism using condition mechanism */
void *printEvenSync(void *arg){
    while(num<MAX){
        pthread_mutex_lock(&mtx);
        while(!(num&1)){
            pthread_cond_wait(&cond, &mtx);
        }
        printf("\nEven num: %d", num);
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&cond);
        num++;
   }
    pthread_exit(0);
}

void *printOddSync(void *arg){
    while(num<MAX){
        pthread_mutex_lock(&mtx);
        while(num&1){
            pthread_cond_wait(&cond, &mtx);
        }
        printf("\nOdd num: %d", num);
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&cond);
        num++;
   }
    pthread_exit(0);
}

int main(){

    /* Thread without sync */
    /*pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, 0, &print_odd, NULL);
    pthread_create(&t2, 0, &print_even, NULL);

    pthread_join(t1, 0);
    pthread_join(t2, 0);*/

    pthread_t ts1;
    pthread_t ts2;

    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cond, 0);

    pthread_create(&ts1, 0, &printOddSync, NULL);
    pthread_create(&ts2, 0, &printEvenSync, NULL);

    pthread_join(ts1, 0);
    pthread_join(ts2, 0);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    printf("\n");
    return 0;
}