#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <pthread.h>


#define LL_ADD(item, list) do { \
    item->pre = NULL;           \
    item->next = list;          \
    list = item;                \
} while(0)

#define LL_REMOVE(item, list) do {                  \
    if(item->pre)item->pre->next = item->next;      \
    if(item->next)item->next->pre = item->pre;      \
    if(item == list)  list = item->next;            \
    item->next = item->pre = NULL;                  \
} while(0)

typedef struct NWORKER {
    pthread_t thread;
    int terminate;
    struct NWORKQUEUE *workqueue;
    struct NWORKER* next;
    struct NWORKER* pre;
} nWorker;

typedef struct NJOB {
    void (*job_function)(struct NJOB* job);
    void *user_data;
    struct NJOB* pre;
    struct NJOB* next;
} nJob;

typedef struct NWORKQUEUE {
    struct NJOB* waiting_jobs;
    struct NWORKER* workers;
    pthread_mutex_t jobs_mtx;
    pthread_cond_t jobs_cond;
} nWorkQueue;

typedef nWorkQueue nThreadPool;

static void *ntyWorkerThread(void* ptr) {
    nWorker* worker = (nWorker*)ptr;

    while(1) {
        pthread_mutex_lock(&worker->workqueue->jobs_mtx);
        while(worker->workqueue->waiting_jobs == NULL) {
            if(worker->terminate) break;
            pthread_cond_wait(&worker->workqueue->jobs_cond, &worker->workqueue->jobs_mtx);
        }

        if(worker->terminate) {
            pthread_mutex_unlock(&worker->workqueue->jobs_mtx);
            break;
        }

        nJob* job = worker->workqueue->waiting_jobs;
        if(job != NULL) {
            LL_REMOVE(job, worker->workqueue->waiting_jobs);
        }

        pthread_mutex_unlock(&worker->workqueue->jobs_mtx);

        if(job == NULL) continue;
        job->job_function(job);
    }

    free(worker);
    pthread_exit(NULL);
}

int ntyThreadPoolCreate(nThreadPool *workqueue, int numWorkers) {
    if(numWorkers < 1) numWorkers = 1;
    memset(workqueue, 0, sizeof(nThreadPool));

    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&workqueue->jobs_cond, &blank_cond, sizeof(workqueue->jobs_cond));

    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&workqueue->jobs_mtx, &blank_mutex, sizeof(workqueue->jobs_mtx));

    int i = 0;
    for(i = 0; i < numWorkers; i ++) {
        nWorker* worker = (nWorker*)malloc(sizeof(nWorker));
        if(worker == NULL) {
            perror("malloc");
            return 1;
        }
        memset(worker, 0, sizeof(nWorker));
        worker->workqueue = workqueue;
        
        int ret = pthread_create(&worker->thread, NULL, ntyWorkerThread, (void*)worker);
        if(ret) {
            perror("pthread_create");
            free(worker);
            return 1;
        }

        LL_ADD(worker, worker->workqueue->workers);
    }
    return 0;
}

    void ntyThreadPoolShutdown(nThreadPool* workqueue) {
        nWorker* worker = NULL;
        for(worker = workqueue->workers; worker != NULL, worker ++) {
            worker->terminate = 1;
        }
        
        pthread_mutex_lock(&workqueue->jobs_mtx);
        workqueue->workers = NULL;
        workqueue->waiting_jobs = NULL;
        
        pthread_cond_broadcast(&workqueue->jobs_cond);
        pthread_mutex_unlock(&workqueue->jobs_mtx);
    }


    void ntyThreadPoolQueue(nThreadPool* workqueue, nJob* job) {
        pthread_mutex_lock(workqueue->jobs_mtx);
        LL_ADD(job, workqueue->waiting_jobs);

        pthread_cond_signal(&workqueue->jobs_cond);
        pthread_mutex_unlock(workqueue->jobs_mtx);
    }


    #define TAO_MAX_THREAD 80
    #define TAO_COUNTER_SIZE 1000
    
    void tao_counter(nJob* job) {
        int val = *(int*)job->user_data;

        printf("val = %d, selfid: %lu\n", val, pthread_self());

        free(job->user_data);
        free(job);
    }


    int main(int argc, char *argv[]) {
        nThreadPool pool;
        ntyThreadPoolCreate(&pool, TAO_MAX_THREAD);

        int i = 0;
        for(i = 0; i < TAO_COUNTER_SIZE; i ++) {
            nJob* job = (nJob*)malloc(sizeof(nJob));
            if(job == NULL) {
                perror("malloc");
                exit(1);
            }

            job->job_function = tao_counter;
            job->user_data = malloc(sizeof(int));
            *(int*)job->user_data = i;
            ntyThreadPoolQueue(&pool, job);
        }

        getchar();
        printf("\n");
    } 
