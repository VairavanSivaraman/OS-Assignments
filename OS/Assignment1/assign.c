#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <unistd.h> 
unsigned long nibble_sort(unsigned long number)
{
  int *byte,i,t,d,size=sizeof(number)*2;
  unsigned long temp=0xf,result=0x0;
  byte=(int*) malloc(sizeof(int)*sizeof(number)*2);
  if(byte==NULL)
    return 0;;
  for(i=0;i<size;i++){
    byte[i]= (number&temp)>>(i*4);
    temp<<=4;
    d=i;
    if(i!=0)
    while ( d > 0 && byte[d] < byte[d-1]) {
      t = byte[d];
      byte[d]= byte[d-1];
      byte[d-1] = t;
      d--;
    }
  }
  for(i=size-1;i>=0;i--)
    { result|=byte[i];
      if(i!=0)
        result<<=4;
    }
  free(byte);
  return(result);
}
unsigned long byte_sort(unsigned long number)
{
  int *byte,i,t,d,size=sizeof(number);
  unsigned long temp=0xff,result=0x0;
  byte=(int*) malloc(sizeof(int)*sizeof(number));
  if(byte==NULL)
    return 0;
  for(i=0;i<size;i++){
    byte[i]= (number&temp)>>(i*8);
    temp<<=8;
    d=i;
    if(i!=0)
    while ( d > 0 && byte[d] < byte[d-1]) {
      t = byte[d];
      byte[d]= byte[d-1];
      byte[d-1] = t;
      d--;
    }
  }
  for(i=size-1;i>=0;i--)
    { result|=byte[i];
      if(i!=0)
        result<<=8;
    }
  free(byte);
  return(result);
}
struct elt {
  char val;
  struct elt *link;
};

struct elt *name_list (void)
{
  struct elt *linkedList=NULL,*head=NULL,*temp=NULL;
  int i;
  char str[10]="Vairavan\0";
  for(i=0;str[i];i++)
    {

      temp=(struct elt*)malloc(sizeof(struct elt));
      if(temp!=NULL)
        {

          temp->val=str[i];
          temp->link=NULL;
          if(linkedList==NULL)
            {

              linkedList=temp;
              head=temp;
            }
          else
            {
              linkedList->link=temp;
              linkedList=temp;
            }
        }
      else
        {
          temp=head;
          while(temp!=NULL)
          {
              temp=temp->link;
              free(head);
              head=temp;
          }
          return NULL;
        }

    }
  return head;
}
enum format_t {
  OCT = 66, BIN, HEX
};

void convert (enum format_t mode, unsigned long value)
{ char *str1,str[16]="0123456789ABCDEF";
  int i,temp,size=0,lshift;
  if(mode==66){
    if(sizeof(value)%3==0)
      size=(sizeof(value)*8)/3;
    else
      size=((sizeof(value)*8)/3)+1;
    temp=7;
    lshift=3;
  }
  else if(mode==67)
  {
    size=(sizeof(value)*8);
    temp=1;
    lshift=1;
  }
  else if(mode==68)
  {
    size=(sizeof(value)*2);
    temp=15;
    lshift=4;
  }
  else{
    return ;
  }
  str1=(char*)malloc(sizeof(char)*size);
  if (str1==NULL){
    return;
  }
  for(i=0;i<size;i++){
    str1[i]=str[value&temp];
    value>>=lshift;
    }
for(i=size-1;i>=0;i--){
  putc(str1[i],stdout);
}
  putc('\n',stdout);
}
void draw_me(void)
{ int i,check,fp;
  char str[3496]="____________________________________________________________________________________________________\n|ooo oooooooooooo+++++++++++++:.+++++++++o++++++ooooooooooooooooosossssssssssssssssssssssssssssssssss|\n|ooooooooooooooooooooooo+++++++++::-/++++++++++++++++++oooooooooooooooosoosssssssssssssssssssssssssss|\n|+oooooooooooooooooooooooooo++/::::-::::///////////+///++++++++++++ooooosoossssssssssssssssssssssssss|\n|oooooooooooooooooooooooooooooo+/:://:::::///////////+++ooso+++++++++oosossssssssssssssssssssssssssss|\n|ooooo+oooooooooooooooooooooooooo+//+/+syhhhhhhhhhhddddmmmmdddddhyooooooossoossssssssssssssssssssssss|\n|++oo+o+o+ooooo++ooooooooooooooosyhhhddddmmddmmmmmmmmmmmNNmdmmmmmmmdhysoooooooooossooooooooosssosssss|\n|+++++++++++++oo+++oo+oooosyyhhdmmmmmmmmmmmmNmNNNNmNNmNmNNmdmmmmmmmmmmdhyssoooooooooooooooooosoosssoo|\n|+++++++++++++++++++o++shdmmmmmmmmmmmmNNNmmNNNNNNNNNNNNNNNmdmmNmmmNmmmmmmdhhyysoooooooooo+o++++oooooo|\n|/////++++++++++++++/shdmmNmmmmmmmNNNNmmmmmmmNmmmmmmNmmmmmmmmmmNmmmmmmmmmmdddddhhys+ooo+/+o+oo+oo++++|\n|+///:::::://///++++ydNNNNNmmNmmmmNNNmmmmmmmmNNNmNNNNNmmmmmmmmmmmmmmmmmmmmmddmmmddddhso+++++oo+osoooo|\n|+//////++////://::odmNNNmmNNmNNmNNNNNmmNNNNNNNNNNNNNNNNNmdmNNmmmmmmmmmmmmmmdmmmmdddmmdysooooo+/+++oo|\n|+///////+/://+//+ydmNNNmmmNmNNNNNNNNNmmNNNNNNNNNNNNNNNNNmmNNmNNNNmmmmmmmmdddmmmmmddmmmmdhoossso+ooso|\n|//+///+++////+//hmmNNNmmNNNNNNNNNNNNmmNNNNNNNNNNNNNNNNNmdmNNNNNNNmmmmNNNmmmdmddmmddmmmdhmys/os+//oo+|\n|//+++++/+/::../ymmmmmmmNNNNNNNNNNNNmdmNNNNNNNNNNNNNNNNNmdmNNNNNmNNmNNNNNmmmdmmmmmmddmmmhmmdysoosssss|\n|/++/+//+++//::sdmmmmmmNNNmNmmNNNNNNmmNNNNNNNNNNNNNNNNNNmmNNmNNmNNNmmNNNNmmddmmmmmmmmmmmddmmdhsssyyys|\n|/++///+++/+++ydmmmmNmNNNNNNmNNNNNNNNNNNNNNNNNNNNNNNNNNmmNNNNNmNmNmmmmNmNmmdmmmmmmmdmmmmddmmdhhssssss|\n|+++/..-::::/odmmmNNNmNNNNNNNNNNNNNmmNNNNNNNNNNNNNNNNNNddmNNNNNNmNNNNNNmmNdydmmmmmmdmdmdddmmdddyo++oo|\n|ooo++/:-..-/hdmmNNNmNNNNmNNNNNNNNNmmNNNNNNNNNNNNNNNNNmdmNNNNNNNNNNmmNNNmmmdmmmmmmddmmdmmddmdhdhsssys|\n|oooo+:/::/:sdmmNNNNNNNNNmNNNNNNNNNmmNNNNNNNNNNNNNNNNNmmNNNNNNNNNNNmmmNmNmhmmmmmmmddddmmmdddddddhsyyy|\n|++:++//:/+sdmmmNNNNmNNNNmNNNNNNNNNNNNNNNNNNNNNNNNNNNmdNNNNNNNNNNNNmmmmmmhymmmmmmmddddmmmddddhdhysssy|\n|ooo+o++//ohdmmNNNNmmNNNNNNNNNNNNNmmmmNNmmmmmNmmdmNmmmmmNNNNNNNNNNNNNNNmdydmmmmmmmdmddmmmddmmdhhyooss|\n|+++++ooo+sdmmmNNNNmNNNNNNNNNNNNNmmNNNNNNNNNNNNNNNNmmNmmNmmmmNmmmmmNNNNmmdmmmmmmmhdddmmmmhdmmddhs+oos|\n|/////+//:ydmmmNNNNmNNNNNmNNNNNNNNmNNNNNNNNNNNNmmmmmmmNNmNmmNNmmmmmNddddhdmmmmmmdhdhdmmmmhmmmdhhooo++|\n|``.--:::/hmNmmNNNmmmNNmNNNNNNNNNmmNmmmmmmNNNNNNNNNNNmmNNmmmmmmmNNNNNmdhdhddmmmmdmmdmmmmddmmddhy++sss|\n|..-.//+/ydmNmmmmNmmNNNNNNNNNNNNmmmmNNNNNNNNNNNNNNNNNNNNNNNNNNmNmmmmmmdmNmmmmddddmmmmmmdhdmmmdhho+yyy|\n|+//+o++sdmmmmmmmmmNNNNNNNNNmmmNNNNNNNNNNNNNNNNNNNNNNNNNNMNNNNNNNNNNmmmmmmNNmmmhddhmmmmdhmmmmdhhosyyy|\n|//////+dmmmmmmmmmNNNNNNmmmmNNNNNNNNNNNNNNNNNNNNNNNNNMNNNNNNNNNNNNNNNNNNmmmmmmmmmhdmddmdmmmmmddhoosso|\n|://::/+dmmmmmmmNNNNmmmmmNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNmmmmmmmmmmmhhmmmmdhdho-+o/|\n|ossoooodmmmmNNNNNmmmmNNNNNNNNNMMMMMMMMMNNNNNNNmNmmmmmmNNNNNNNNNNNNNMNNNNNNNNmmmmmmmmmhddmmmhhdho+ssy|\n|+ooo++ymmmNNNNNmmmNNNNNNNMMMMMMMMMMMMNMNNMNNNNNNmmmmmmNNNNNNNNNNNMNMMMMMMMMNNNmmmmmmdhmmddddddy/../s|\n|so+o//ymNNNNNNmmNNNNNNNMMMMMNNNNNNNMMMNNNNMNNNNNmmmmmmNNNNNNNNNNNNMNMMMMMMMMMNNNmmmdddmmdhhdmdh+-:/s|\n|soo+/odNNNNmmmNNNNNNMMMMNMNNNNNNNNNNNNNNNNNNNNNNNNmmmmNNNNNNNNNNNMNNNNMMMMMMMMMMNNNmmddmmdhdddh+/yyy|\n|/oooohNNNmmmNNNNNNNMMMNNNNNNNNNNNNNNNNNNNNNNNNNNmNNNNmmNNNNNNNNNNNNNNNNNNNNNNMMMNNNNmmddddddddd+/";
char str2[3816]="oo/|\n|osssymNNmmmNNNNNNNmmNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNmNNNNNNNNNNNNNNNNNNNNNNMMMMMNNNNmdddhhddh:///:|\n|+/shmmmmmNNNNMNNNNdNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMMNNNNNNNNNNNMMMMMMMMNNmdddhhhs-::/-|\n|+:smNmmmNNNMMNNNNNdNNNNNNNNNmNNNNmNmmNNNNNNNNNMNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMMMNMNmdhddho/yyyy|\n|:/dmNNmNNNNMMMNNNmdNNNNNNNNNNNNNNNNNNNNNNNNmmNNNNNNNNNNmNNNNNNmmNNNNNNNmNNNNNNNNNNNNNMMMNmdhhy+oyhyy|\n|:ommNNNNNNMMMNNNNNymNNNNNNNNmmmmmmmmNNNNNmmNNNNmdddhmNmmNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMMNNmhhs:+yyyy|\n|sydmNNNNNNMMNNNNNNmmmmmmmmmmNmmmmmmmNmmmmmNNNNmddhhyymNmmmmmNNmNNNmmmmmmmmNNNNNNNmNNNNMMNNNdh+.-++oo|\n|sydmNNNNNMMNmmmmmmmmNmmmmmmdddddddmmmmmmmmNNNmmdhhyyydNmmmmmmmmmmmmmmmmmmmmmmNmmmmNNNNMMNNNdhs:/syss|\n|/odNNNNMMNNNmmmmmmmmmmmmmmmmmmmmmmmmmNNNNNNNNmmdhhyyyymmdddddddddmdddmdddddddddddNNNNMMMNNNdhy::shyy|\n|--hNNNNNMNNmmmdddddddddddddddddddmmNNNNNNNNmmmddhysssyydmmmmdddhhhhhhyyyyhyyssyhmmmmNMMMNNmmhs:-yhhy|\n|+/yNNNNMMNNmmmdmmdddhhhhhhhhhhhddmmNNNNNNNNmmddhhyssssshddmmmmmmmmmddddhddddhhdddhyhmMMNNNmdh+-:yyhy|\n|sosmNMNNNNNmmddddddddhhhhyyyyyhdmNNNNNNNNmmmdhhhysoosssydddmmmmddddhhhyyyyysooooosyydNNNNNmdh+:/:ohy|\n|sssdNNNNNNNmmdddddddhhhhyyyyyyhdmNNNmmmmmmdmddhhyo++ooosyyydmNmmddddhyyysso+++o+osyhmNNNNNmmy+:/oyyy|\n|sssymNNNNNNmmdddddddhhhhyyyyyhdmmNNmmmmmmmmmdddhysso+osssssydmmmmddhhyssoo++++oossyhmNNNNNmds/:::oo+|\n|so+syhmmNNNmmmdddddddhhhhyyhdmmmNNNNmmmmmmmmmmddhhyssyyysssyhdmNNmddhyyssooo+oossyydNNNNNNmho+/:/oss|\n|ooo+/sdmmNNmmmdddmddddhhhhdmNNNNNNNNNNNNNNNNNmmmdddhdmdhyyyhhhdmNNmddhyyssoooosssyhdNNNNNNms::///oos|\n|/./+/odmmmmmmmmmmmddddhhhmNNNNNNNNNNNNNNNNNNNNNmmNNmmmmddhhhhddmNNNmddhyssosssssyyhmNNNNNmy/:.-.-//o|\n|+//+++shhhhdmmmmmmdddhhhmNNNNNNmmmmmmmmmNmmmmmmddddhhhhyyhhhhhddmNNNmdhyyssssssyyyhdddmmdo-.-::-:/:/|\n|ooosooosssosdmmmmmddhhhdmmNNNNNNNNNNNNNmNmmmmmdhhhhhhhhhyyyyyyhdmmNNNmdhyssssssyyyyyssys:-.`.://+ooo|\n|ysssssssoo+odmmmmmddddhddmNNNNNNNNmmmmmmddddddhhhhhhhhhhhddmmdddmmNNNmdhyssssssyyyysoo+-```..-:.-+oo|\n|yyysyssssoohmmmmmmmmddhhhmmmmNNNNmhhhhyys+++ooo+/+++/+//ohmNNNmmdmNNNmdhysssssyyyyysso+//:::-:---:++|\n|yysyysyyyhmNNmmmmmmmddhhhdmmmmmmmmdddhyyo+//++/:-:/::/+/oydmmdhhhdmmmdhyyyyssyyyyoosssooooo+//:::/++|\n|yyyyhhdmNNNNNNmmmmmmmmddddddmmmmmmmmddddhyyyhysooyyssyyyyyhhyssyhdmmdhhyysyyyyyhyo+ssyyssyysssoooooo|\n|ddmmmmmNNNNMMMNmmmmmmmmdddmdmmmmmmmmmdddhyyyhhhhhhyysyyyyysssssyhdddhhyyyyyyyyhmmy+ossyssyyyyyyyysss|\n|mmmmmNNNNNNNMMMNmmmmmmmdmmmmmmmmmmmmmmmddhhhhhhhhyyysyyssssssssyhdhhhyyyyyyyyhdNNdoosssysssyyyyyyyyy|\n|mmmNNNNNNNNMMMMNNmNNmmmmmmmmmmmmmmmmNNNmmmmddddddhyyyyysssssyyyhhhhhyyyyyyyhdmNNNmsssssyyssyyyyyyyyy|\n|mNNNNNNNNNNNMMMNNNNNNNmmNmmmmmmmmmmmmmmmmmmmmmmmdddhhyyyyyyyyyyyyyyyyyyyyyhmNNNmNNhsyysyysshhhyssyyy|\n|NNNNNNMNNMMMNNNNNNNNNNmNNmmmmmmdddmmmmmmmmmmmdddhhhhyyyyyyyyyyyyyyyyyhhhhdNMMNNNNNdshhyyyyyyyysssyyy|\n|NNNNNNNNNMMNNMMMNNmNNNNNNNNNmmmdddddddddddddddhhhhyyyyyyyyyhhyyyhhhhhddddmNMMNNNmNmyhhhyyysyyssssyyy|\n|NNMNNNMMMMNNNNNMNNNNNNNNNNNNNmmmmddddddddhdddhhhhhyyyyyyyyyhhhhhhhhdddddmNMMMMNNNNNyhdhyyysssssssyyy|\n|NNNNNNNNNMMMMNNMMNNNNNNNNNNNNNNNmmmmddmdddddhddhhyyyyyyyyyhhhhhdddddhddmNMMMMMNNNNmyhddhyysssssyyyyy|\n|NNNNNNNNNNNMMMMMMNNNNNNNNNNNNNNNNNNmmmmmddddddmddhhhhyhhhhhddddddhhhdmmmNMMMMNNNNNmhhddyyysssyyyyhhh|\n|NNNNNNNMNNNNNNNMMNNNmNNNNNNNNNNNNNNNNNNmmmmmmmmmdddddddddddddhhhhddddmNMMMMMMNNNNNmmddhyyysssyyhhhhh|\n|NNNMNNNMNNNNNNMMNNNNNNNmmNNNNNNNNNNNNNNNNNNNNNNmmmmmmmmmdhhhhyhhddddmmNMMMMMMMNNNNNmmhhyyssyyhhddhhy|\n|NNNNNNNNMNMNNNNMMMNNNNmNNNNNNNmNNNmNNNNNNNNNNNNNmmmmddhhyyyhhhdddddmmNMMMMMMMNNNNNNmmdyyysyyhhdhhyso|\n|NNNNNMNMMNMNNNNNNMMMNNmNNNmmmNNNNNNNmmmNmmmmmmmddhhhyhhyyhhhddddddmNNMMMMMMMMMNNNNNNNmhyssyhdhhysooo|\n|NNNNNNNMNNNNNNMMMNNMMNNNNNmmNNNNmmmmmmmmmmmmmddddhhhhhhyhhdddddddmNMMMMMMMMMMNNNNNNNNNmhyyhhhysooooo|\n|____________________________________________________________________________________________________|"; 
 
 fp=syscall(SYS_creat,"me.txt",0777);
  if(fp==-1){
    check=syscall(SYS_unlink,"me.txt");
    if(check==-1)
       return;
    fp=syscall(SYS_creat,"me.txt",0777);
    if(fp==-1)
	return;
}
  for(i=0;str[i];i++);
  check=syscall(SYS_write,fp,str,i);
if(check!=i)
    { 
      syscall(SYS_close,fp);
      check=syscall(SYS_unlink,"me.txt");
      return;
      }
  for(i=0;str2[i];i++);
  check=syscall(SYS_write,fp,str2,i);
if(check!=i)
    { 
      syscall(SYS_close,fp);
      syscall(SYS_unlink,"me.txt");
      return;
    }
  syscall(SYS_close,fp);
  check = syscall(SYS_chmod,"me.txt", 0777);
if(check==-1)
    {
      syscall(SYS_unlink,"me.txt");
      return;
      }
  return;
}
