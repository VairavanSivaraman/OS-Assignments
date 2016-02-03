#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<assert.h>
int COUNT=0;
double run=0.0;   
volatile int in_cs;                             

struct spin_lock_t{
  volatile int serve;
  volatile int nextinLine;
}mr_lock;

void spin_init(struct spin_lock_t *s)
{
  s->serve=0;
  s->nextinLine=0;
}

static inline int atomic_xadd (volatile int *ptr)
{
  register int val __asm__("eax") = 1;
  asm volatile ("lock xaddl %0,%1"
		: "+r" (val)
		: "m" (*ptr)
		: "memory"
		);
  return val;
}

void spin_lock(struct spin_lock_t *s)
{
  int num;
  num = atomic_xadd(&(s->nextinLine));
  while (num != s->serve){}
}

void spin_unlock(struct spin_lock_t *s)
{
  atomic_xadd(&(s->serve));
}

void* threading ( void* threadid ) {
  long i=(long)threadid;
  long occur=0;
  time_t start,end;
  time(&start);
  time(&end);
    while (difftime(end,start)<run ) {
      /* Obtain Lock to enter critical section. */
      spin_lock(&mr_lock);
      /* Critical section. */
      occur++;
      assert(in_cs==0);
      in_cs++;
      assert(in_cs==1);
      in_cs++;
      assert(in_cs==2);
      in_cs++;
      assert(in_cs==3);
      in_cs=0;
      /* End critical section.Release the lock  */
      spin_unlock(&mr_lock);
      time(&end);
    }
    printf(" thread: %ld \t count: %ld\n",i,occur);
    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
  int rc;
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
  spin_init(&mr_lock);
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
