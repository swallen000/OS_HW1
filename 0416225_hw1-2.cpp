#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <math.h>
#include <stdlib.h>

unsigned int one_process(int,unsigned long long int **,unsigned long long int **);
unsigned int four_process(int,unsigned long long int **,unsigned long long int **);

int main(){
	int size;
	float time=0;
	printf("Dimension: ");
	scanf("%d",&size);
	unsigned long long int **a,**b;
	unsigned int sum1,sum2;
	struct timeval start;
	struct timeval end;
	a=new unsigned long long int*[size];
	b=new unsigned long long int*[size];
	for(int i=0;i<size;i++){
		a[i]=new unsigned long long int[size];
		b[i]=new unsigned long long int[size];
		for(int j=0;j<size;j++){
			a[i][j]=i*size+j;
			b[i][j]=i*size+j;
		}
	}	

	gettimeofday(&start,NULL);
	sum1=one_process(size,a,b);
	gettimeofday(&end,NULL);
	time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
	printf("1-process, checksum = %u\n",sum1);
	printf("elapsed %f  s\n",time/1000000);

	gettimeofday(&start,NULL);
	sum2=four_process(size,a,b);
	gettimeofday(&end,NULL);
	time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
	printf("4-process, checksum = %u\n",sum2);
	printf("elapsed %f  s\n",time/1000000);

	for(int i=0;i<size;i++){
		delete [] a[i];
		delete [] b[i];
	}
	delete a;
	delete b;
	return 0;
}

unsigned int four_process(int size,unsigned long long int **a,unsigned long long int **b){
	unsigned int sum=0;
	int shmid;
	unsigned long long int *shm_address;
	if((shmid=shmget(IPC_PRIVATE,sizeof(unsigned long long int)*pow(size,2),IPC_CREAT|0666))==-1){
		perror("fail of shmget");							   
		return 0;
	}
	if((shm_address=(unsigned long long int *)shmat(shmid,NULL,0))==(unsigned long long int *)-1){
		perror("fail of shmat");
		return 0;
	}
	else{
		pid_t pid1,pid2,pid3,par1,par2,par3;
		for(int i=0;i<pow(size,2);i++){
			shm_address[i]=0;
		}
		pid1=fork();
		if(pid1<0){
			printf("pid1 error\n");
			return 1;
		}
		else if(pid1==0){
			pid3=fork();
			if(pid3<0){
				printf("pid3 error\n");
				return 1;
			}
			else if(pid3==0){
				for(int i=3*size/4;i<size;i++){
					for(int j=0;j<size;j++){
						for(int k=0;k<size;k++)
							shm_address[i*size+j]+=a[i][k] * b[k][j];
					}
				}
				if(shmdt(shm_address)==-1)
					printf("fail of shmdt ");
				exit(EXIT_SUCCESS);
			}
			else{
				for(int i=size/2;i<3*size/4;i++){
					for(int j=0;j<size;j++){
						for(int k=0;k<size;k++)
							shm_address[i*size+j]+=a[i][k] * b[k][j];
					}
				}
				/*par3=waitpid(pid3,NULL,WNOHANG);
				while(par3==0){
					par3=waitpid(pid3,NULL,WNOHANG);
					if(par3!=0)
					break;
				}*/
				if( wait(NULL)==-1 )
					printf("fail of wait");
				//printf("end of par3\n");
				
			}
			if(shmdt(shm_address)==-1)
					printf("fail of shmdt ");
			exit(EXIT_SUCCESS);
		}
		else{
			pid2=fork();
			if(pid2<0){
				printf("pid2 error\n");
				return 1;
			}
			else if(pid2==0){
				for(int i=size/4;i<size/2;i++){
					for(int j=0;j<size;j++){
						for(int k=0;k<size;k++)
							shm_address[i*size+j]+=a[i][k] * b[k][j];
					}
				}
				if(shmdt(shm_address)==-1)
					printf("fail of shmdt ");
				exit(EXIT_SUCCESS);
			}
			else{
				for(int i=0;i<size/4;i++){
					for(int j=0;j<size;j++){
						for(int k=0;k<size;k++)
							shm_address[i*size+j]+=a[i][k] * b[k][j];
					}
				}
				/*par2=waitpid(pid2,NULL,WNOHANG);
				while(par2==0){
					par2=waitpid(pid2,NULL,WNOHANG);
					if(par2!=0)
						break;
				}
				par1=waitpid(pid1,NULL,WNOHANG);
					par1=waitpid(pid1,NULL,WNOHANG);
					if(par1!=0)
					break;
				}*/
				if( wait(NULL)==-1 )
					printf("fail of wait");
				if( wait(NULL)==-1 )
					printf("fail of wait");
				//printf("end of par2\n");
				//printf("end of par1\n");
				for(int z=0;z<pow(size,2);z++)
					sum+=shm_address[z];
				if(shmdt(shm_address)==-1)
					printf("fail of shmdt ");
				if(shmctl(shmid,IPC_RMID,0) == -1) {
        			perror("fail of shmctl");
					return 1;
				}
			}
		}

	}
	return sum;
}

unsigned int one_process(int size,unsigned long long int **a,unsigned long long int **b){
	unsigned int sum=0;
	for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            unsigned int temp = 0;
            for(int k=0;k<size;k++){
                temp+= (a[i][k] * b[k][j]);
            }
            sum+=temp;
        }
    }
	return sum;
}