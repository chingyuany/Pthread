#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

const size_t numthreads =3;

int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * ThreadEntry(void * id){
  
    const int myid = (long)id;
    printf("In the ThreadEntry, Assigning argument id to myid =%d\n", myid);
    const int workloops =5;
    int i;
    for(i=0; i < workloops; i++){
        printf("Thread %d sleeping %d secs\n", myid, i + 1);
        sleep(1);
    }
   
    pthread_mutex_lock(&mutex);
    printf("Thread %d acquire to lock mutex variable\n", myid);

    done++;
    printf("Thread %d execute done = done +1, done = %d\n", myid, done);
    
    pthread_cond_signal(&cond);
    printf("Thread %d Signals (or wake up) another thread in the main which is waiting on the condition variable\n", myid);
  
    pthread_mutex_unlock(&mutex);
    printf("Thread %d acquire to unlock mutex variable\n", myid);
    return NULL;
}

int main(int argc, char** argv){
    printf("starting declare  %zu threads in pthread_t array type.\n", numthreads);
    pthread_t threads[numthreads];
    int t;
    for(t=0; t < numthreads; t++){
        printf("creating thread %d and excute ThreadEntry function with argument t = %d\n", t, t);
        pthread_create(&threads[t], NULL, ThreadEntry, (void*)(long)t);
    }
    
    pthread_mutex_lock(&mutex);
    printf("main Thread acquire to lock mutex variable\n");
    int j =1;
    while(done < numthreads){
        printf("This is %d time wait until receive signal \n", j);
        pthread_cond_wait(&cond,&mutex);
        printf("This is %d time received signal \n", j++);
    }
    printf("all threads finished and done become number of threads = 3\n");

    pthread_mutex_unlock(&mutex);
    printf("main Thread  acquire to unlock mutex variable\n");
    printf("exiting program\n");
    return 0;
}