#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<assert.h>
int COUNT=0;
double run=0.0;   
volatile int in_cs=0,*Number;                              
int *Entering;  
int max () {
  int i, max = Number[ 0 ];
  for (i=1;i<COUNT;i++)
    if (Number[i] > max)
      max=Number[i];
  return max;
}

void lock(long i){
  int j;
  Entering[i] = 1;
  Number[i] = 1 + max();
  Entering[i] = 0;
  for (j=0;j<COUNT;j++ ) {
    while (Entering[j]);
    while (Number[j] != 0 && (Number[j] <  Number[i] || ( Number[j] == Number[i] && j < i )));
  }
}
void unlock(long i){
  Number[ i ] = 0; 
}
void* threading ( void* threadid ) {
  pthread_detach( pthread_self() ); 
  long i =(long)threadid;
  long occur=0;
  time_t start,end;
  time(&start);
  time(&end);
  while (difftime(end,start)<run ) {
    /* Obtain Lock to critical section. */
    lock(i);
    /* Critical section. */
    assert(in_cs==0);
    in_cs++;
    assert(in_cs==1);
    in_cs++;
    assert(in_cs==2);
    in_cs++;
    assert(in_cs==3);
    in_cs=0;
    /* End critical section.Release the lock */
    unlock(i);
    /*Schedule another processor for Execution*/
    sched_yield();
    occur++;
    time(&end);
  }
  printf(" thread: %ld \t count: %ld\n",i,occur);
  pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    int i = 0,rc;
    long t;
    pthread_t *threads;
    if(argc!=3){
	printf("Not enough cmd line arguments\n");
	exit(1);
    }
    COUNT=atoi(argv[1]);
    run=(double)atoi(argv[2]);
    if(COUNT <= 0){
      printf("Threads counts error:  check your cmd line argument\n");
      return 0;
    }
    if(run <= 0){
      printf("Run Time error:  check your cmd line argument\n");
      return 0;
    }
    threads =(pthread_t*)malloc(sizeof(pthread_t)*COUNT);
    if(threads == NULL){
      printf("Malloc Failed: Threads\n");
      return 0;
    }
    Number = (int*)malloc(sizeof(int)*COUNT);
    if(Number == NULL){
      printf("Malloc Failed: Number\n");
      return 0;
    }
    Entering = (int*)malloc(sizeof(int)*COUNT);
    if(Entering == NULL){
      printf("Malloc Failed: Entering\n");
    }
      for ( i = 0 ; i < COUNT ; i++ )
      Number[ i ] = Entering[ i ] = 0;
      for(t=0; t<COUNT; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, threading, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
      
   }
   pthread_exit(NULL);
}
