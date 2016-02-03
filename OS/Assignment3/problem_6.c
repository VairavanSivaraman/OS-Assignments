#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <pthread.h>
#define seed 1234
int COUNT=0;
double run=0.0;   
volatile long int counter,pi;
struct spin_lock_t{
  volatile int t;
}mr_lock;

void spin_init(struct spin_lock_t *s){
  s->t = 0;
}

static inline int atomic_cmpxchg (volatile int *ptr, int old, int new)
{
  int ret;
  asm volatile ("lock cmpxchgl %2,%1"
		: "=a" (ret), "+m" (*ptr)
		: "r" (new), "0" (old)
		: "memory");
  return ret;
}

void spin_unlock(struct spin_lock_t *s){
  atomic_cmpxchg(&s->t,1,0);
}

void spin_lock(struct spin_lock_t *s){
  while(atomic_cmpxchg(&s->t,0,1)==1);
}
                              
void* threading () {
  int t=0;
  double x,y,dist;
  time_t start,end;
  time(&start);
  time(&end);
  while (difftime(end,start)<run ) {
    t++;
    srand48(seed+t);
    x= -1+(((double)rand()/(double)RAND_MAX)*2);		 	/* creates floats between -1 and 1 */
    y= -1+(((double)rand()/(double)RAND_MAX)*2); 
    dist = (x*x)+(y*y);
    /* Obtain lock to enter critical section. */
    spin_lock(&mr_lock);
    /* Critical section. */
    counter++;
    if(dist<1.0) pi++;
    /* End Critical section.Release the Lock */
    spin_unlock(&mr_lock);
    time(&end);
  }
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
    rc = pthread_create(&threads[t], NULL, threading,NULL);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  for(t=0;t<COUNT;t++)
    pthread_join(threads[t],NULL);
  printf("\n\t pi:%lf,\n\t Successful points:%ld,\n\t Number of points:%ld\n",4.0*(double)pi/(double)counter,pi,counter);
  pthread_exit(NULL);
}
