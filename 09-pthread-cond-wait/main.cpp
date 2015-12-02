#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

/* auteur: Bernard Pallotta */

using namespace std;

typedef struct arg {
    int n;
    int id;
    void * data;
} arg_t;

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t memcopy_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t memcopy_mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
int copyCount = 0;
int countMax = 0;

void syncLog(string s) {
    pthread_mutex_lock(&log_mutex);
    cout << s << endl;
    pthread_mutex_unlock(&log_mutex);
}

void * worker(void * arg) {
    //on va chercher les arguments
    arg_t *a = new arg_t();
    memcpy(a, arg, sizeof(arg_t));
    arg_t copie = *a;
    char *data = static_cast<char *>(copie.data);

    //on augmente le nombre de fois qu'on a copié les arguments de façon synchrone
    pthread_mutex_lock(&memcopy_mutex);
    ++copyCount;
    //si le nombre de copie est égal au nombre de thread, on envoit un signal pour dire a
    //start_threads qu'il peut quitter
    if (copyCount == countMax) {
        pthread_cond_signal(&memcopy_cond);
    }
    pthread_mutex_unlock(&memcopy_mutex);

    //simulation de travail
    sleep(copie.id % 5);

    //un peu de verbosité pour voir ce qu'il se passe
    stringstream sstm;
    sstm << "ThreadID: " << copie.id << " Data: " << (char) data[copie.id]
         << " Count courant: " << count;
    syncLog(sstm.str());

    //si le count est égal au nombre de thread, on signal que la condition est remplie
    pthread_mutex_lock(&count_mutex);
    ++count;
    if (count == countMax) {
        syncLog("Signalement");
        pthread_cond_signal(&count_cond);
    }
    pthread_mutex_unlock(&count_mutex);

    //clean up
    delete a;
    return NULL;
}

void start_threads(int n, void * data, pthread_t *threads) {
    arg_t args[n];

    for (int i = 0; i < n; ++i) {
        args[i].n = n;
        args[i].id = i;
        args[i].data = data;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    //on s'assure que les memcpy on fini avant de quitter
    pthread_mutex_lock(&memcopy_mutex);
    while (copyCount < countMax) {
        pthread_cond_wait(&memcopy_cond, &memcopy_mutex);
    }
    pthread_mutex_unlock(&memcopy_mutex);
    cout << "start_threads quitte" << endl;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    char data[] = "abcdefghijklmnopqrstuvwxyz";
    countMax = strlen(data);
    pthread_t threads[countMax];

    start_threads(countMax, &data, threads);

    //Selon la doc: They shall be called with mutex locked by the calling thread or undefined behavior results.
    pthread_mutex_lock(&count_mutex);

    //au cas s'il y a un "Spurious wakeup"
    while (count < countMax) {
        syncLog("main entre dans la boucle de cond_wait");
        pthread_cond_wait(&count_cond, &count_mutex);
    }
    syncLog("main vient de finir le cond_wait");
    pthread_mutex_unlock(&count_mutex);

    //clean-up
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_cond);
    pthread_mutex_destroy(&memcopy_mutex);
    pthread_cond_destroy(&memcopy_cond);

    // il est requis de lire le status de retour des threads
    for (int i = 0; i < countMax; i++) {
        pthread_join(threads[i], NULL);
    }
    std::cout << "done" << std::endl;
    return 0;
}
