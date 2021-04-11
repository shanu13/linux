
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESS 8

typedef struct {
 char *name;
 int arrival;
 int cpuburst;
 int turnaround;
 int wait;
}Process;

int no_process = 0;
Process processtable[MAX_PROCESS];

// print the statistics of a process
void Stats(){
	int total_turnaround = 0, total_wait = 0;
	for(int i=0; i<no_process; i++){
		total_turnaround += processtable[i].turnaround;
		total_wait += processtable[i].wait;
	}

	printf("\n Turnaround time: ");
	for(int i=0; i<no_process; i++){
		printf("%s[%d] ", processtable[i].name,processtable[i].turnaround);
	}
	printf("\n");
	printf("Wait time : \t");
	for(int i=0; i<no_process; i++){
		printf("%s[%d]  ",processtable[i].name,processtable[i].wait);
	}
	printf("\n");

    printf("Average turnaround time: %.2f\n", (float)total_turnaround / no_process	);
	printf("Average wait time: %.2f\n", (float)total_wait / no_process);

}

// reset 

void reset(){
	for(int i=0; i<no_process; i++){
		processtable[i].wait = 0;
		processtable[i].turnaround = 0;
	}
}


void FCFS(){
	int currentTime = 0;
	int startTime = 0, endTime = 0;

	printf("---------- FIRST COME FIRST SERVE SCHEDULING\n");

	for(int i=0; i<no_process; i++){
		startTime = currentTime;
		endTime = processtable[i].cpuburst + startTime;
		processtable[i].wait = startTime - processtable[i].arrival;
		processtable[i].turnaround = endTime - processtable[i].arrival;
		currentTime = endTime;

		printf("[%d - %d]\t %s running\n",startTime,endTime,processtable[i].name );

	}

	Stats();
	reset();


}

void RR(int quantum){

	printf("\n---------------Round Robin Scheduling--------\n");

	int process = no_process;
	int index = 0;
	int currentTime = 0, runningTime = 0;

	int burst_remaining[no_process];
	for(int i=0; i<no_process; i++){
		burst_remaining[i] = processtable[i].cpuburst;
	}

	while(process){
		if(currentTime >= processtable[index].arrival && burst_remaining[index] !=0){
			runningTime = (burst_remaining[index] - quantum) >= 0 ? quantum : burst_remaining[index]; 
		}

		if(runningTime == burst_remaining[index]){
			process--;
		}
		for(int i=0; i< no_process; i++){
			if(i==index){
				burst_remaining[index] -= runningTime;
			}else{

				if(currentTime + runningTime >= processtable[i].arrival && burst_remaining[i]!=0){
					if(currentTime < processtable[i].arrival){
						processtable[i].wait += currentTime + runningTime - processtable[i].arrival;
					}else{
						processtable[i].wait += runningTime;				
					 }

				}
			}
		}

		if(runningTime){
			printf("[%d-%d]\t %s running",currentTime,currentTime+runningTime,processtable[index].name);
			currentTime+= runningTime;
		}
		index = (index+1)%no_process;
		runningTime = 0;

	}

	for (int i = 0; i <no_process; i++)
		processtable[i].turnaround = processtable[i].wait + processtable[i].cpuburst;


	Stats();
	reset();

}

int shortestRemaining(int *burst_remaining, int currentTime){
	int index = -1, value = INT_MAX;

	for (int i = 0; i < no_process; i++)
	{
		if (burst_remaining[i] && value > burst_remaining[i] && currentTime >= processtable[i].arrival)
		{
			value = burst_remaining[i];
			index = i;
		}
	}	

	return index;
}

int next_arrival(int currentTime){
	for (int i = 0; i < no_process; i++)
	{
		if (currentTime < processtable[i].arrival)
			return processtable[i].arrival;
	}

	return -1;
}




void SRBF(){

	printf("\n-----------------SRBF--------------------------\n");

	int burst_remaining[no_process];
	for(int i=0; i<no_process; i++){
		burst_remaining[i] = processtable[i].cpuburst;
	}

	int process = no_process;
	int runningTime =0, currentTime =0, next_interrupt = 0;
	int index =0;

	while(process){
		if((index = shortestRemaining(burst_remaining,currentTime)) != -1){
				// getting -1 means no new process are arriving
				if((next_interrupt = next_arrival(currentTime)) != -1){

					 if(currentTime + burst_remaining[index] <= next_interrupt){
					 	runningTime = burst_remaining[index];
					 }else{
					 	runningTime = next_interrupt - currentTime;
					 }
				}else{
					runningTime = burst_remaining[index];
				}
				if(runningTime = burst_remaining[index]){
					process--;
				}

				for(int j=0; j< no_process; j++){
					if(index == j){
						burst_remaining[j] -= runningTime;
					}else{
						if(burst_remaining[j] != 0 && currentTime >= processtable[j].arrival){
							processtable[j].wait += runningTime;
						}
					}
				}
				printf("[%d - %d]\t%s running\n",currentTime,currentTime+runningTime, processtable[index].name);
				currentTime += runningTime;
			}
	   }
	   for(int i=0; i<no_process; i++){
					processtable[i].turnaround = processtable[i].wait + processtable[i].cpuburst;
				}
	Stats();
	reset();
} 




int main(int argc, char const *argv[])
{
	if(argc < 2){
		perror("Input file Missing");
		exit(-1);
	}
 	 

 	 FILE *finput = NULL;

 	  if((finput = fopen(argv[1],"r")) == NULL){ // process.txt not opening
 	  	perror("Error in reading file");
 	  	exit(-1);
 	  }

 	  // allocating name dynamically 

 	  for(int i=0; i<MAX_PROCESS; i++){
 	  	processtable[i].name = (char*) calloc(8,sizeof(char)); // allocting name of a process of 8 bytes
 	  }

 	  // reading from process.txt files

 	  for(int i=0; i<MAX_PROCESS; i++){
 	  	if(!feof(finput)){ // scanning the process
 	  		fscanf(finput, "%s %d %d", (processtable[i].name), &(processtable[i].arrival),&(processtable[i].cpuburst));
 	  	}
 	  	else{
 	  		no_process = i-1;
 	  		break;
 	  	}
 	  }

 	  // printf("%d\n",no_process);

 	  int choice;
 	  int quantum;

 	  while(1){
 	  	printf("\n---------------------------------------------------------------\n"
 	  		   "      CPU Scheduling Simulation\n"
 	  		   "-----------------------------------------------------------------\n"
 	  		   "Select the Scheduling Algorithm [1, 2, 3, or 4] : \n"
 	  		   "1. First Come First Serve (FCFS)\n"
 	  		   "2. Round Robin (RR)\n"
 	  		   "3. Shortest Remaining Bursttime First (SRBT)\n"
 	  		   "4. Exit\n");

 	  	scanf("%d",&choice);

 	  	 if(choice == 4){
 	  	 	break;
 	  	 }
 	  	 switch(choice){
 	  	 	case 1: FCFS();
 	  	 			break;
 	  	 	case 2 : printf("Enter quantum number\n");
 	  	 			 scanf("%d",&quantum);
 	  	 			 RR(quantum);
 	  	 			 break;

 	  	 	case 3: SRBF();
 	  	 			break;

 	  	 	default: printf("Invalid Option \n");
 	  	 			 break;
 	  	 }

 	  	 printf("\n Hit Any key to continue...\n");
 	  	 getchar();

 	  }

   		fclose(finput);

	return 0;
}

