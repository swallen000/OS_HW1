#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(){
	pid_t pid1,pid2,pid3,pid4,pid5,par1,par2,par3,par4,par5;
	printf("Main Proccess ID : %d\n\n",getpid());
	pid1=fork();
	if(pid1< 0){
		printf("error!");
		return 1;
	}
	else if(pid1==0){
		printf("Fork 1. I'm the child %d,my parent is %d\n",getpid(),getppid());
		int i=0;
		for(i=0;i<2;i++){
			pid2=fork();
			if(pid2==0 || pid2==-1)
				break;
		}
		if(pid2<0){
			printf("error!");
			return 1;
		}
		else if(pid2==0){
			printf("Fork 2. I'm the child %d,my parent is %d\n",getpid(),getppid());
			pid3=fork();
			if(pid3==0){
				sleep(1);
				printf("Fork 3. I'm the child %d,my parent is %d\n",getpid(),getppid());
				exit(EXIT_SUCCESS);
			}
			else{
				wait(NULL);
				/*par3=waitpid(pid3,NULL,WNOHANG);
				while(par3==0){
				par3=waitpid(pid3,NULL,WNOHANG);
				if(par3!=0)
					break;
				}*/
				//printf("end of 333\n");
				exit(EXIT_SUCCESS);
			}
		}
		else{
			wait(NULL);
			/*par2=waitpid(pid2,NULL,WNOHANG);
			while(par2==0){
			//sleep(1);
			par2=waitpid(pid2,NULL,WNOHANG);
			if(par2!=0)
				break;
			}*/
			//printf("end of here\n");
		}
		
		exit(EXIT_SUCCESS);
	}
	else{
		wait(NULL);
		/*par1=waitpid(pid1,NULL,WNOHANG);
		while(par1==0){
			par1=waitpid(pid1,NULL,WNOHANG);
			if(par1!=0)
				break;
		}*/
		//printf("end of par1\n");
	} 
	return 0;
}