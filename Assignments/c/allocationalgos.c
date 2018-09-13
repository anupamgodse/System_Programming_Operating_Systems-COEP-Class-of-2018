#include <stdio.h>
#include <limits.h>
typedef struct partition{
	float size;
	float avail;
	int index;
}partition;

typedef struct process{
	float size;
	int status;
	float offset;
}process;
/*here status of process is  -1 				if it's not allocated a partition
					index no of partition 	if allocated a partition
					-2				if finished
 
  offset is with reference to starting address of partition it is allocated -1 otherwise
*/


float calinternalfrag(partition *p, int npar) {
	int i;
	float t;
	printf("Internal fragmentation for partition with index\n");
	for(i = 0; i < npar; i++) {
		if(p[i].avail != 0) {
			printf("\t\t\t\t\t\t%d is %f\n", i, p[i].size - p[i].avail);
			t += p[i].size - p[i].avail;
		}
		else
			printf("\t\t\t\t\t\t%d is %f\n", i, 0.0);
	}
	printf("\nTotal Internal fragmentation is %f\n\n", t);
	return t;	
}

void calexternalfrag(partition *p, int npar, process *pro, int np) {
	int i;
	float available = 0;
	float needed = INT_MAX;
	
	for(i = 0; i < np; i++) {
		if(pro[i].status == -1 && needed > pro[i].size)
			needed = pro[i].size; 
	}
	
	float internal = calinternalfrag(p, npar);
	available += internal;
	
	for(i = 0; i < npar; i++) {
		if(p[i].avail == 0)
			available += p[i].size;
	}
	
	if(needed == INT_MAX || needed > available) {
		printf("Total external fragmentation is : %d\n\n",  0);
		return;
	}
	
	printf("Total external fragmentation is : %f\n\n",  available);
	return;
}

void printprocess(process *p, int np) {
	int i;
	for(i = 0;  i < np; i++) {
		printf("Process %d:-\n", i + 1);
		printf("\tSize = %f", p[i].size);
		printf("\tStatus = %d", p[i].status);
		printf("\tOffset = %f\n", p[i].offset);
	}
	printf("\n");
	return;
}

void printpartition(partition *p, int npar) {
	int i;
	for(i = 0;  i < npar; i++) {
		printf("Partition index %d:-\n", i);
		printf("\tSize = %f", p[i].size);
		printf("\tAvailable = %f\n", p[i].size - p[i].avail);
	}
	printf("\n");
	return;
}

void initpar(partition *p, int npar) {
	int i;
	for(i = 0; i < npar; i++) {
		p[i].index = i;
		p[i].avail = 0;
	}
	return;
}

void initpro(process *p, int np) {
	int i;
	for(i = 0; i < np; i++) {
		p[i].status = -2;
		p[i].offset = -1;
	}
	return;
}

void addnewprocess(process *p, int np , float size) {
	//search for available partition;
	int i;
	for(i = 0; i < np; i++) {
		if(p[i].status == -2) {
			p[i].size = size;
			p[i].status = -1;
			break;
		}
	}
	if(i == np) 
		printf("Process queue is full, process cannot be added\n");
	return;
}

void usefirstfit(partition *par, int npar, process *pro, int np) {
	int i, j;
   	for(i = 0; i < np; i++){
   		if(pro[i].status != -1)
   			continue;
   		//Here process is in process queue and is to be allocated if memory is available.	
       	
       	for(j = 0; j < npar; j++) {
         		if(par[j].size - par[j].avail >= pro[i].size){
         			pro[i].offset = par[j].avail;
         			pro[i].status = j;
         			par[j].avail += pro[i].size;
              		printf("Process allocated of size %f\n", pro[i].size);
              		printf("\tPartition Index = %d\n", pro[i].status);
              		printf("\tStarting offset = %f\n", pro[i].offset);
 				printf("\tMemory available in partiton = %f\n\n", par[j].size - par[j].avail);             		
             		break;            
         		}  
     		}    
		
		if(j == npar)
			printf("Not enough memory for process number %d\n\n", i + 1);
			        
   	}
   	return;
}

void usebestfit(partition *par, int npar, process *pro, int np) {
	int i, j;
	for (i = 0; i < np; i++) {
		if(pro[i].status != -1)
			continue;
		
		int min = 0;	
		for(j = 1; j < npar; j++) {
			if((par[j].size - par[j].avail) >= pro[i].size) 
				if((par[min].size - par[min].avail < pro[i].size) || (par[min].size - par[min].avail > (par[j].size - par[j].avail)))
					min = j;	
		}
		//printf("min = %d\n", min);
		if(min == 0 && (par[0].size - par[0].avail < pro[i].size)) 
			printf("Not enough memory for process number %d\n\n", i + 1);				
		
		else {
			pro[i].offset = par[min].avail;
			pro[i].status = min;
			par[min].avail += pro[i].size;
	  		printf("Process allocated of size %f\n", pro[i].size);
	  		printf("\tPartition Index = %d\n", pro[i].status);
	  		printf("\tStarting offset = %f\n", pro[i].offset);
			printf("\tMemory available in partiton = %f\n\n", par[min].size - par[min].avail);             			
		}
	}
	return;
}

void useworstfit(partition *par, int npar, process *pro, int np) {
	int i, j;
	for (i = 0; i < np; i++) {
		if(pro[i].status != -1)
			continue;
		
		int max = 0;	
		for(j = 1; j < npar; j++) {
			if((par[j].size - par[j].avail) >= pro[i].size) 
				if((par[max].size - par[max].avail < pro[i].size) || (par[max].size - par[max].avail < (par[j].size - par[j].avail)))
					max = j;	
		}
		//printf("max = %d\n", max);
		if(max == 0 && (par[0].size - par[0].avail < pro[i].size)) 
			printf("Not enough memory for process number %d\n\n", i + 1);				
		
		else {
			pro[i].offset = par[max].avail;
			pro[i].status = max;
			par[max].avail += pro[i].size;
	  		printf("Process allocated of size %f\n", pro[i].size);
	  		printf("\tPartition Index = %d\n", pro[i].status);
	  		printf("\tStarting offset = %f\n", pro[i].offset);
			printf("\tMemory available in partiton = %f\n\n", par[max].size - par[max].avail);             			
		}
	}
	return;
}

void usenextfit(partition *par, int npar, process *pro, int np) {
	int i, j, last = 0, f = 0, k	;
	//printprocess(pro, np);
	//printpartition(par, npar);
   	for(i = 0; i < np; i++){
   		if(pro[i].status != -1)
   			continue;
   		//Here process is in process queue and is to be allocated if memory is available.	
       	
       	for(j = last, k = 0; k < npar ; j++, k++) {
         		if(par[j].size - par[j].avail >= pro[i].size){
         			pro[i].offset = par[j].avail;
         			pro[i].status = j;
         			par[j].avail += pro[i].size;
              		printf("Process allocated of size %f\n", pro[i].size);
              		printf("\tPartition Index = %d\n", pro[i].status);
              		printf("\tStarting offset = %f\n", pro[i].offset);
 				printf("\tMemory available in partiton = %f\n\n", par[j].size - par[j].avail);             		
 				f = 1;
 				last = j;
             		break;            
         		}
         		if(j + 1 == npar)
         			j = -1;  
     		}    
		
		if(!f)
			printf("Not enough memory for process number %d\n\n", i + 1);
			        
   	}
   	return;
}


int main(){
   	int np, npar, i, end;
   	float mainmemory, memory, x;  
   	
   	printf("Enter total amount of main memory:");
   	scanf("%f", &mainmemory);
      
      memory = mainmemory;  
   	
   	printf("Enter total number of partitions:");
   	scanf("%d", &npar);
 	
 	partition partition[npar];
 	
 	initpar(partition, npar);
 	for(i = 0; i < npar - 1; i++){
     		printf("Enter size of %d index partition :", i);
     		scanf("%f", &(partition[i].size));
     		memory -= partition[i].size;
   	}
 	printf("Size of %d index partition is : %f\n", i, memory);
 	partition[i].size = memory;
 	
   	printf("Enter number of processes: ");
   	scanf("%d", &np);
	
   	process process[np];
   	
   	initpro(process, np);
   	
   	for(i = 0; i < np; i++){
     		printf("Enter size of process %d:", i+1);
     		scanf("%f", &x);
     		addnewprocess(process, np, x);       
   	}
	/*
		Now main memery with available partition sizes is is fixed and 
		proccess pool is ready. So now each process can be allocated a 
		partition if available with one of the following algorithms
	*/
	
	int option;
	
	while(1) {
		printf("Choose Algorithm for process allocation:-\n\t1. First fit\n\t2. Best fit\n\t3. Worst fit\n\t4. Next fit\n\t5. Exit\n");
		scanf("%d", &option);
	
		//printprocess(process, np);
		//printpartition(partition, npar);
		switch(option) {
			case 1:
				usefirstfit(partition, npar, process, np);
				break;
			case 2:
				usebestfit(partition, npar, process, np);
				break;
			case 3:
				useworstfit(partition, npar, process, np);
				break;
			case 4:
				usenextfit(partition, npar, process, np);
				break;
			case 5:
				return 0;
			default:
				printf("Enter proper option\n");
		} 	   
		printprocess(process, np);
		printpartition(partition, npar);
		
		/*
		printf("Options :- \n");
		printf("\t1. Enter a new process into queue\n");
		printf("\t2. End a process\n");
		printf("\t3. Allocate process from queue\n");
		
		scanf("%d", &option);
		switch(option) {
			case 1:
				printf("Enter size of process :");
     				scanf("%f", &x);
     				addnewprocess(process, np, x);
     			case 2:
     				printf("Enter process number to be ended : ");
     				scanf("%d", &end);
     				endprocess(process, np, partiton, npar);
     			case 3:
     				       
		}
		*/
		
		//printf("Total internal fragmentation is : %f\n",  calinternalfrag(partition, npar));
		//printf("Total external fragmentation is : %f\n\n",  calexternalfrag(partition, npar, process, np));
		calexternalfrag(partition, npar, process, np);
		initpro(process, np);
		initpar(partition, npar);
		
		for(i = 0; i < np; i++)
	     		addnewprocess(process, np, process[i].size);       
		//printprocess(process, np);
		//printpartition(partition, npar);
	}
}
