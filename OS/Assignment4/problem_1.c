#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <pthread.h>
#include <assert.h>
volatile int cat_in_play=0,dog_in_play=0,bird_in_play=0,cat_count=0,dog_count=0,bird_count=0;
volatile int NCAT,NDOG,NBIRD;
pthread_mutex_t locker;
pthread_cond_t cv1,cv2,cv3;
void play(void) {
  int i;
  for (i=0; i<10; i++) {
    //printf("%d %d %d\n",dog_in_play,cat_in_play,bird_in_play);
    assert(cat_in_play >= 0 && cat_in_play <= NCAT);
    assert(dog_in_play >= 0 && dog_in_play <= NDOG);
    assert(bird_in_play >= 0 && bird_in_play <= NBIRD);
    assert(cat_in_play == 0 || dog_in_play == 0);
    assert(cat_in_play == 0 || bird_in_play == 0);
  }
}
void dog_enter(void){
  if(dog_in_play < NDOG){
  pthread_mutex_lock(&locker);
  while(cat_in_play>0)
    pthread_cond_wait(&cv2,&locker);
  // printf("In dog\n");
  dog_in_play++;
  dog_count++;
  pthread_mutex_unlock(&locker);
  }
}
void cat_enter(void){
  if(cat_in_play < NCAT){
  pthread_mutex_lock(&locker);
  while(dog_in_play>0 || bird_in_play>0)
    pthread_cond_wait(&cv1,&locker);
  // printf("In cat\n");
  cat_in_play++;
  cat_count++;
  pthread_mutex_unlock(&locker);
  }
}
void bird_enter(void){
  if(bird_in_play <NBIRD)
   {
  pthread_mutex_lock(&locker);
  while(cat_in_play>0)
    pthread_cond_wait(&cv3,&locker);
  //printf("In bird\n");
  bird_in_play++;
  bird_count++;
  pthread_mutex_unlock(&locker);
   }
}
void dog_exit(void){
  pthread_mutex_lock(&locker);
  dog_in_play--;
  if(dog_in_play==0 && bird_in_play==0)
    pthread_cond_broadcast(&cv1);
  pthread_mutex_unlock(&locker);

}
void cat_exit(void){

  pthread_mutex_lock(&locker);
  cat_in_play--;
  if(cat_in_play==0){
    pthread_cond_broadcast(&cv2);
    pthread_cond_broadcast(&cv3);
  }
  pthread_mutex_unlock(&locker);

}
void bird_exit(void){

  pthread_mutex_lock(&locker);
  bird_in_play--;
  if(dog_in_play==0 && bird_in_play==0)
    pthread_cond_broadcast(&cv1);
  pthread_mutex_unlock(&locker);

}
void* dog (void * threadid) {
  time_t start,end;
  time(&start);
  time(&end);
  while(difftime(end,start)<10){
    dog_enter();
    play();
    dog_exit();
    time(&end);
  }    
  pthread_exit(NULL);
}
void* cat (void * threadid) {
  time_t start,end;
  time(&start);
  time(&end);
  while(difftime(end,start)<10){
    cat_enter();
    play();
    cat_exit();
    time(&end);
  }    
  pthread_exit(NULL);
}
void* bird (void* threadid) {
  time_t start,end;
  time(&start);
  time(&end);
  while(difftime(end,start)<10){
    bird_enter();
    play();
    bird_exit();
    time(&end);
  }    
  pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
  int rc;
  long t;
  int run;
  pthread_t *dogs,*cats,*birds;
  if(argc!=4){
    printf("Not enough cmd line arguments\n");
    exit(1);
  }
  NCAT=atoi(argv[1]);
  NDOG=atoi(argv[2]);
 NBIRD=atoi(argv[3]);
  run=10;
  if(NCAT <= 0 || NDOG <=0 || NBIRD <=0 ){
    printf("Threads counts error:  check your cmd line argument\n");
    return 0;
  }
  dogs =(pthread_t*)malloc(sizeof(pthread_t)*NDOG);
  cats =(pthread_t*)malloc(sizeof(pthread_t)*NCAT);
  birds =(pthread_t*)malloc(sizeof(pthread_t)*NBIRD);
  if(dogs == NULL || cats ==NULL || birds==NULL){
    printf("Malloc Failed: Threads\n");
    return 0;
  }
  if (pthread_mutex_init(&locker, NULL) != 0)
    {
      printf("\n mutex init failed\n");
      return 1;
    }
  if (pthread_cond_init(&cv1, NULL) != 0)
    {
      printf("\n mutex init failed\n");
      return 1;
    }
  if (pthread_cond_init(&cv2, NULL) != 0)
    {
      printf("\n mutex init failed\n");
      return 1;
    }
  if (pthread_cond_init(&cv3, NULL) != 0)
    {
      printf("\n mutex init failed\n");
      return 1;
    }

  for(t=0; t<NCAT; t++){
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&cats[t], NULL,cat,(void*)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  for(t=0; t<NDOG; t++){
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&dogs[t], NULL,dog,(void*)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  for(t=0; t<NBIRD; t++){
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&birds[t], NULL,bird,(void*)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  for(t=0;t<NCAT;t++)
    pthread_join(cats[t],NULL);
  for(t=0;t<NDOG;t++)
    pthread_join(dogs[t],NULL);
  for(t=0;t<NBIRD;t++)
    pthread_join(birds[t],NULL);
  printf("Cat Play:%d, Dog Play:%d, Bird Play:%d\n",cat_count,dog_count,bird_count);
   pthread_exit(NULL);
}
