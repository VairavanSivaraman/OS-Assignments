#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char* argv[]) {
	int child_id,i,j;
        unsigned long long number=0;
	if(argc!=2)
		return 1;
	for(i=0;argv[1][i];i++){
		if(argv[1][i]>47 && argv[1][i]<58)
			number=(number*10)+argv[1][i]-48;
		else
			return 1;
	} 
	if(number<=0)
		return 0;
	child_id = fork();
	if (child_id < 0)
		return 1;
	if (child_id) {
		wait(NULL);
		}
	else {
		printf("%llu",number);
		while(number!=1)
		{
			if(number%2==0)
			{
				number=number/2;
			}
			else
				number=(3*number)+1;
			printf(", %llu",number);
				
		}	
		printf("\n");
	}
	return 0;
	}
