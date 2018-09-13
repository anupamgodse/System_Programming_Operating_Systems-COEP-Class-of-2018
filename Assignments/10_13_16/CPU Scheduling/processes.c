#include <stdio.h>
#include <stdlib.h>
#include<string.h>

/* structure node for one processes*/
struct node {
	char pid[10];
	int at;
	int bt;
	int p;
	struct node *next;
};
/* process for list */
struct list{
	struct node* head ;
	int length;
};

void init(struct list *l);
void insert (struct list *l,char* pid , int at, int bt , int p);
void print(struct list *l);
void fifo(struct list *l);
void sjf(struct list *l);
void rr(struct list *l);
void pnp(struct list *l);
void hrrn(struct list *l);
void pp(struct list *l);
void srtf(struct list *l);
struct list * copy(struct list *);


int main() {

	int  at, bt, p;
	char pid[10];
	/* create link list from process*/

	struct list * plist =(struct list *)malloc(sizeof(struct list));
	init(plist);
	
	/* Input from file to link list */

	FILE*  file = fopen("processes1", "r" ); 
	char line[256];
	char *token;
	while(fgets(line, sizeof(line) , file)) {
		token = strtok(line, "\t");
		strcpy(pid,token);
		token = strtok(NULL ,"\t");
		at = atoi(token);
		token = strtok(NULL ,"\t");
		bt = atoi(token);
		token = strtok(NULL ,"\t");
		p = atoi(token);
		insert(plist , pid, at, bt, p);
	}
	printf("P_id\tAT\tBT\tP\n");
	print(plist);
	
	/*call fifo */
	fifo(plist);
	rr(plist);
	sjf(plist);
	pnp(plist);
	hrrn(plist);
	pp(plist);
	srtf(plist);
	fclose(file);


	return 0;
}



/*Initialising process list */
void init(struct list *l) {
	l->head = NULL;
	l->length = 0;
}
/* insert into process list in appending manner*/
void insert (struct list *l,char* pid , int at, int bt , int p) {
	struct node *n = (struct node *)malloc(sizeof(struct node)) ;
	strcpy(n->pid,pid) ;
	n->at = at;
	n->bt = bt;
	n->p = p;
	n->next = NULL;
	struct node *a = l->head;
	if( a != NULL) {
		while(a->next != NULL) {
			a = a->next;
		}
		a->next = n;		
	} else {
		l->head = n;
	}


}
/*to delete from list */
void delete(struct list *l, struct node *s) {
	struct node *pre = l->head;
	if(pre == s) {
		l->head = l->head->next;
	}
	else {	
		while(pre->next != s) {
			pre = pre->next;	
		}
		if(pre) {
			pre->next = pre->next->next;	
		}
	}
}

struct list * copy(struct list * l) {
	struct list * new = (struct list *)malloc(sizeof(struct list));
	init(new);
	struct node * n = l->head;
	while(n != NULL) {
		insert(new, n->pid, n->at, n->bt , n->p);
		n = n->next;	
	}
	return new;
}

/*print process list */
void print(struct list *l){
	struct node *ptr = l->head ;
	printf("-\t-\t-\t-\t\n");
	while(ptr!= NULL) {
		printf("%s\t|%d\t|%d\t|%d\t|\n" ,ptr->pid,  ptr->at , ptr->bt, ptr->p);
		printf("-\t-\t-\t-\t\n");
		ptr = ptr->next;
	}

}


/*Function for fifo*/
void fifo(struct list *l) {
	struct node *n = l->head ;
	char quantt[100];
	int num[100];
	int i, len,j;
	int c = 0;
	quantt[c++] = '|';
	quantt[c] = '\0';
	printf("-----------------------------------------------\n");
	printf("--------------------FIFO-----------------------\n");
	printf("-----------------------------------------------\n");
	while(n != NULL) {
		strcat(quantt , n->pid);
		strcat(quantt, "\t|");
		n = n->next;		
	}
	printf("%s\n", quantt);
	n = l->head;
	int time = 0;
	printf("%d", time);
	while(n != NULL) {
		time = time + n->bt;
		printf("\t%d", time);
		n = n->next;
	}
	printf("\n-----------------------------------------------\n");
}

/* Function for short job first  */

void sjf(struct list *plist) {
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("--------------------SJF------------------------\n");
	printf("-----------------------------------------------\n");
	
	struct list *l = copy(plist);
	struct node *n = l->head ;
	struct node *a;
	struct node *p;
	struct node *s;
	int smbt;
	int time = 0;
	while(n != NULL) {
		a = n->next;
		while(a != NULL && a->at <= time ) {
			a =  a->next;
		}
		while(n != a) {
			p = n->next;
			s = n;
			while(p != a) {
				if(p->bt < s->bt) {
					s = p;
				}
				p = p->next;
			}
		
			printf("|(%d)%s\t",time , s->pid);
			time = time + s->bt;		

//			smbt = time;


			if(n !=s) {
				delete(l, s);
			} else {
				n = n->next;
				delete(l , s);
			}
		}		
	}
	printf("|(%d)\n-----------------------------------------------\n", time);
}		

/* Round robin algorithm */
void rr(struct list *plist) {	
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("--------------------RR-------------------------\n");
	printf("-----------------------------------------------\n");

	struct list * l = copy(plist);
	struct node *n = l->head ;
	int totaltime = 0, time;
	while(n != NULL) {
		totaltime += n->bt;
		n = n->next;
	}
	n = l->head;
	while(n->next != NULL) {
		n = n->next;
	}
	n->next = l->head;
	n = l->head ;
	time = 0;
	while(time < totaltime) {
		if(n->bt > 2 && n->at <= time) {
			printf("|(%d)%s\t", time , n->pid);		
			time += 2;
			n->bt -= 2;
			
		}else if(n->bt == 0) {
			delete(l, n);
		}else {
			printf("|(%d)%s\t", time , n->pid);		
			time += n->bt;
			n->bt = 0;
		}
		n = n->next;
				
	}
	printf("|(%d)\n-----------------------------------------------\n", time);
}

/* priority non primptive*/

void pnp(struct list *plist) {
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("--------------------pnp------------------------\n");
	printf("-----------------------------------------------\n");
	
	struct list *l = copy(plist);
	struct node *n = l->head ;
	struct node *a;
	struct node *p;
	struct node *s;
	int smbt;
	int time = 0;
	while(n != NULL) {
		a = n->next;
		while(a != NULL && a->at <= time ) {
			a =  a->next;
		}
		while(n != a) {
			p = n->next;
			s = n;
			while(p != a) {
				if(p->p < s->p) {
					s = p;
				}
				p = p->next;
			}
		
			printf("|(%d)%s\t",time , s->pid);
			time = time + s->bt;		
			if(n !=s) {
				delete(l, s);
			} else {
				n = n->next;
				delete(l , s);
			}
		}		
	}
	printf("|(%d)\n-----------------------------------------------\n", time);
}		

/*Function for highest response ratio next "*/


void hrrn(struct list *plist) {
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("--------------------hrrn-----------------------\n");
	printf("-----------------------------------------------\n");
	
	struct list *l = copy(plist);
	struct node *n = l->head ;
	struct node *a;
	struct node *p;
	struct node *s;
	int smbt;
	int rr;
	int time = 0;
	while(n != NULL) {
		a = n->next;
		while(a != NULL && a->at <= time ) {
			a =  a->next;
		}
		while(n != a) {
			p = n->next;
			s = n;
			while(p != a) {
				if(((float)(time - (p->at))/(p->bt))  > ((float)(time - (s->at))/(s->bt))) {
					s = p;
				}
				p = p->next;
			}
		
			printf("|(%d)%s\t",time , s->pid);
			time = time + s->bt;		



			if(n !=s) {
				delete(l, s);
			} else {
				n = n->next;
				delete(l , s);
			}
		}		
	}
	printf("|(%d)\n-----------------------------------------------\n", time);
}

/* function for priority preemptive */


void pp(struct list *plist) {
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("---------------------pp------------------------\n");
	printf("-----------------------------------------------\n");
	
	struct list *l = copy(plist);
	struct node *n = l->head ;
	struct node *a;
	struct node *p;
	struct node *s;
	int smbt;
	int rr;
	int time = 0;
	int totaltime = 0;
	int flag = 0;
	while(n != NULL) {
		totaltime += n->bt;
		n = n->next;
	}

	n = l->head ;
	struct node * pre  = NULL;
	while(time < totaltime) {
		a = n->next;
		while(n != a) {
			while(a != NULL && a->at <= time ) {
				a =  a->next;
			}

			p = n->next;
			s = n;
			flag = 0;		
			while(p != a) {
				if(p->p < s->p) {
					s = p;
				}
				p = p->next;
			}
			s->bt -= 1;
			if (pre != s) {
				printf("|(%d)%s\t",time , s->pid);
			} 
			time += 1;		
			if(n !=s  && s->bt == 0) {
				delete(l, s);
			} else if(s->bt == 0) {
				n = n->next;
				delete(l , s);
			}
			pre = s;
		}		
	}
	printf("|(%d)\n-----------------------------------------------\n", time);
}
void srtf(struct list *plist) {
	printf("\n\n");
	printf("-----------------------------------------------\n");
	printf("---------------------srtf----------------------\n");
	printf("-----------------------------------------------\n");
	
	struct list *l = copy(plist);
	struct node *n = l->head ;
	struct node *a;
	struct node *p;
	struct node *s;
	int smbt;
	int rr;
	int time = 0;
	int totaltime = 0;
	int flag = 0;
	while(n != NULL) {
		totaltime += n->bt;
		n = n->next;
	}

	n = l->head ;
	struct node * pre  = NULL;
	while(time < totaltime) {
		a = n->next;
		while(n != a) {
			while(a != NULL && a->at <= time ) {
				a =  a->next;
			}

			p = n->next;
			s = n;
			flag = 0;		
			while(p != a) {
				if(p->bt < s->bt) {
					s = p;
				}
				p = p->next;
			}
			s->bt -= 1;
			if (pre != s) {
				printf("|(%d)%s\t",time , s->pid);
			} 
			time += 1;		
			if(n !=s  && s->bt == 0) {
				delete(l, s);
			} else if(s->bt == 0) {
				n = n->next;
				delete(l , s);
			}
			pre = s;
		}		
	}
	printf("|(%d)\n-----------------------------------------------\n", time);	
}
