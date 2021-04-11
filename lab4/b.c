#include<stdio.h>
#include <pthread.h>  
#include<stdlib.h>  
#include <time.h>       
#include <unistd.h>     
#define MAXCOUNT 5    // MAXCOUNT = 5  

typedef struct {        //structure defining 
        pthread_mutex_t *mut;
        int writers;
        int readers;
        int waiting;
        pthread_cond_t *writeOK, *readOK;
}rwl;

typedef struct {    //this struct embodies lock,key and delay
        rwl *lock;
        int id;
        long delay;
}rwargs;


rwl *initlock (void);
void readlock (rwl *lock, int d);
void writelock (rwl *lock, int d);
void readunlock (rwl *lock);
void writeunlock (rwl *lock);
void deletelock (rwl *lock);




int main(int argc, char const *argv[])
{	pthread_t r1, r2, r3, r4, w1;// thread 's declared
        rwargs *a1, *a2, *a3, *a4, *a5;
        rwl *lock;
        lock = initlock ();
        long  Writer1 = 0,Reader1 = 0,Reader2 = 0,Reader3 = 0 ,Reader4 = 0;
        a1 = newRWargs (lock, 1, Writer1);
        pthread_create (&w1, NULL, writer, a1);
        a2 = newRWargs (lock, 1, Reader1);
        pthread_create (&r1, NULL, reader, a2);
        a3 = newRWargs (lock, 2, Reader2);
        pthread_create (&r2, NULL, reader, a3);
        a4 = newRWargs (lock, 3, Reader3);
        pthread_create (&r3, NULL, reader, a4);
        a5 = newRWargs (lock, 4, Reader4);
        pthread_create (&r4, NULL, reader, a5);
        pthread_join (w1, NULL);
        pthread_join (r1, NULL);
        pthread_join (r2, NULL);
        pthread_join (r3, NULL);
        pthread_join (r4, NULL);
  		free (a1); 
 	    free (a2);
        free (a3);
        free (a4); 
        free (a5);
        return 0;
	
	return 0;
}

rwargs *newRWargs (rwl *l, int i, long d)//this function is for new read and write arguments
{
        rwargs *args;
        args = (rwargs *)malloc (sizeof (rwargs));
        if (args == NULL) return (NULL);
        args->lock = l; args->id = i; args->delay = d;
        return (args);
}

void readlock (rwl *lock, int d)//this function works as read_lock
{
        pthread_mutex_lock (lock->mut);
        if (lock->writers || lock->waiting) {
                do {
                        printf ("reader %d blocked.\n", d);
                        pthread_cond_wait (lock->readOK, lock->mut);
                        printf ("reader %d unblocked.\n", d);
                }while (lock->writers);
        }
        lock->readers++;
        pthread_mutex_unlock (lock->mut);
        return;
}



void writelock (rwl *lock, int d){//this function works as write_lock
        pthread_mutex_lock (lock->mut);
        lock->waiting++;
        while (lock->readers || lock->writers) {
                printf ("writer %d blocked.\n", d);
                pthread_cond_wait (lock->writeOK, lock->mut);
                printf ("writer %d unblocked.\n", d);
 }
        lock->waiting--;
        lock->writers++;
        pthread_mutex_unlock (lock->mut);
        return;
}


void readunlock (rwl *lock){//this function works as read_unlock
        pthread_mutex_lock (lock->mut);
        lock->readers--;
        pthread_cond_signal (lock->writeOK);
        pthread_mutex_unlock (lock->mut);
        }


void writeunlock (rwl *lock){//this function works as write_unlock
        pthread_mutex_lock (lock->mut);
        lock->writers--;
        pthread_cond_broadcast (lock->readOK);
        pthread_mutex_unlock (lock->mut);
}


void deletelock (rwl *lock){    //this function is used to delete a particular lock
        pthread_mutex_destroy (lock->mut);
        pthread_cond_destroy (lock->readOK);
        pthread_cond_destroy (lock->writeOK);
        free (lock);
        return;
}

rwl *initlock (void){//this function is for initialisation of lock
        rwl *lock;
        lock = (rwl *)malloc (sizeof (rwl));
        if (lock == NULL) return (NULL);
        lock->mut = (pthread_mutex_t *) malloc (sizeof   (pthread_mutex_t));
        if (lock->mut == NULL) {
        free (lock);
        return (NULL);
         }
        lock->writeOK = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
        if (lock->writeOK == NULL) {
                free (lock->mut); free (lock);
                return (NULL);
         }
        lock->readOK = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
         if (lock->writeOK == NULL) { free (lock->mut);
                free (lock->writeOK);
                free (lock); return (NULL);
         }
        pthread_mutex_init (lock->mut, NULL);
        pthread_cond_init (lock->writeOK, NULL);
        pthread_cond_init (lock->readOK, NULL);
        lock->readers = 0;
        lock->writers = 0;
        lock->waiting = 0;
        return (lock);
}


void *reader (void *args){// this function displays reader's information
        rwargs *tempr;
        int d;
        tempr = (rwargs *)args;
        do {
                readlock (tempr->lock, tempr->id);
                d = data;


                readunlock (tempr->lock);
                printf ("Reader %d : Data = %d\n", tempr->id, d);
                
        } while (d != 0);
        printf ("Reader %d: Finished.\n", tempr->id);
        return (NULL);
}

void *writer (void *args)//this function is for writer's part
{
        rwargs *tempw;
        int i;
        tempw = (rwargs *)args;
        for (i = 2; i < MAXCOUNT; i++) {
                writelock (tempw->lock, tempw->id);
                data = i;
            
                writeunlock (tempw->lock);
                printf ("Writer %d: Wrote %d\n", tempw->id, i);
        
        }
        printf ("Writer %d: Finishing...\n", tempw->id);
        writelock (tempw->lock, tempw->id);
        data = 0;
        writeunlock (tempw->lock);
        printf ("Writer %d: Finished.\n", tempw->id);
        return (NULL);
}
