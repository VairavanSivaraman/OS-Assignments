#include<time.h>
#include<dirent.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<stdint.h>
pthread_mutex_t lock;
volatile int incrementer=0,Num_of_strings;
char string[1000][100];
char *string1;
uint32_t *checksum;
int *flags;
static uint32_t crc32_tab[] = {
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
  0xe963a535, 0x9e6495a3,0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
  0xf3b97148, 0x84be41de,0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,0x14015c4f, 0x63066cd9,
  0xfa0f3d63, 0x8d080df5,0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,0x35b5a8fa, 0x42b2986c,
  0xdbbbc9d6, 0xacbcf940,0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
  0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,0x76dc4190, 0x01db7106,
  0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
  0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
  0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
  0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
  0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
  0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
  0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
  0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
  0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
  0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
  0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
  0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
  0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
  0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
  0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
  0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint32_t crc32(uint32_t crc, const void *buf, size_t size)
{
  const uint8_t *p;

  p = buf;
  crc = crc ^ ~0U;

  while (size--)
    crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

  return crc ^ ~0U;
}
void* threading ( void* threadid ) {
  int file_id,flag;
  size_t size;
  char *temp;
  char string2[100];
  FILE *fptr;
  while(1){
    flag=0;
    pthread_mutex_lock(&lock);
    if(incrementer<Num_of_strings){
      file_id=incrementer;
      strcpy(string2,string1);
    }
    else{
    pthread_mutex_unlock(&lock);
    break;
    }
    incrementer++;
    pthread_mutex_unlock(&lock);
    fptr=fopen(string2,"rb");
    if(fptr==NULL){
      flag=1;
      goto L1;
    }
    if(fseek(fptr,0L,SEEK_END)){
      flag=1;
      goto L1;
    }
    size=ftell(fptr);
    if(size==-1U){
      flag=1;
      goto L1;
    }
    if(fseek(fptr,0L,SEEK_SET)){
      flag=1;
      goto L1;
    }
    fclose(fptr);
    temp=(char*) malloc(sizeof(char)*size);
    if(temp==NULL)
      printf("Malloc Error\n");
    fptr=fopen(string2,"r");
    if(fptr==NULL){
      flag=1;
      goto L1;
    }
    if(fread(temp,size,1,fptr)!=1 && size!=0)
      flag=1;
    //printf("%d.\t%s\t%d\t%u\n",i,string[i],size,crc32(0,temp,size));
  L1: 
    if(flag==1)
      flags[file_id]=1;
    else {
      checksum[file_id]=crc32(0U,temp,size);
      flags[file_id]=0;}
    if(fptr!=NULL)
    fclose(fptr);	
  }
  pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
  int rc,COUNT;
  long t;
  pthread_t *threads;
  DIR *dir;
  char string2[100];
  struct dirent *files;
  int i=0,j=0;
  if(argc!=3){
    printf("Not enough cmd line arguments\n");
    exit(1);
  }
  string1=(char*)malloc(sizeof(char)*(strlen(argv[1])+2));
  COUNT=atoi(argv[2]);
  if(COUNT<1 || COUNT>99){
    printf("Thread Count should be in range 0 to 99\n");
    return 0;
  }
  threads =(pthread_t*)malloc(sizeof(pthread_t)*COUNT);
  if(threads == NULL){
    printf("Malloc Failed: Threads\n");
    return 0;
  }  
  pthread_mutex_init(&lock,NULL);
    if ((dir = opendir (argv[1])) != NULL) {
      strcpy(string1,argv[1]);
      i=strlen(string1);
      if(string1[i-1]!='/'){
	string1[i]='/';
	string1[i+1]='\0';
      }
    while ((files = readdir (dir)) != NULL) {
      strcpy(string2,string1);
      strcat(string2,files->d_name);
      if(opendir(string2)==NULL){
	strcpy(string[j],files->d_name);
	
	//	printf("\n %s %d",string[j],strlen(string[j]));
	j++;
      }
    }
    Num_of_strings=j;
    checksum=(uint32_t*)malloc(sizeof(uint32_t)*Num_of_strings);
    flags=(int*)malloc(sizeof(int)*Num_of_strings);
    for(i=0;i<Num_of_strings;i++)
      for(j=0;j<Num_of_strings;j++){
        if(strcmp(string[i],string[j])<0){
          strcpy(string2,string[i]);
          strcpy(string[i],string[j]);
          strcpy(string[j],string2);
        }
      }
    }
    else{
      printf("No directory exists\n");
    }
    for(t=0; t<COUNT; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, threading, (void *)t);
      if (rc){
	printf("ERROR; return code from pthread_create() is %d\n", rc);
	exit(-1);
      }   
    }
    for(t=0;t<COUNT;t++)
      pthread_join(threads[t],NULL);
    for(t=0;t<Num_of_strings;t++){
      if(flags[t]!=1)
	printf("File:%s%s CRC:0x%08X\n",string1,string[t],checksum[t]);
      else
	printf("File:%s%s CRC:ACCESS ERROR\n",string1,string[t]);
    }
    pthread_exit(NULL);
}
