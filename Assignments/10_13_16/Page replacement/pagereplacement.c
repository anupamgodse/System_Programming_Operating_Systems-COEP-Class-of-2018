#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
typedef struct frames{
	int frame;
	int use;
	int modify;
	int lastused;
	int firstused;
}frames;

typedef struct node{
	int pageno;
	int use_times;
}node; 


typedef struct pages{
	node *n;
	int has;	
}pages; 

typedef struct least{
	int *l;
	int total;
}least;

int visitedall(int *visit, int n) {
	int i;
	for(i = 0; i < n; i++) {
		if(!visit[i]) 
			return 0;
	}
	return 1;
}

void init_min(least *min, int n) {
	min->l = (int *)malloc(n * sizeof(int));
	min->total = 0;
	return;			
} 

void init_pages(pages *p, int total) {
	int i;
	p->has = 0;
	p->n = (node *)malloc(sizeof(node) * total); 
	for(i = 0; i < total; i++) 
		(p->n)[i].use_times = 0;
	return;
}

int findindex(pages *p, int v) {
	int i;
	for(i = 0; i < p->has; i++) {
		if((p->n)[i].pageno == v)
			return i;
	}
	return -1;
}

void findleast_times(frames *f, int n, pages *p, least *min) {
	int i, j, minuse = INT_MAX, start;
	for(i = 0; i < n; i++) {
		for(j = 0; j < p->has; j++) {
			if(f[i].frame == p->n[j].pageno && p->n[j].use_times < minuse) {
				//min->l[min->total++] = i;
				start = j;
				minuse = p->n[j].use_times;
				break;
			}
		}
	}
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < p->has; j++) {
			if(f[i].frame == p->n[j].pageno && p->n[j].use_times == minuse) {
				min->l[min->total++] = i;
				break;
			}
		}
	}
	
	
	printf("\nleast used times array is(indices in frames) :  ");
	for(i = 0; i < min->total; i++) 
		printf("%d\t", min->l[i]);
	printf("\n\n\n");
	return;
}

void findmost_times(frames *f, int n, pages *p, least *min) {
	int i, j, maxuse = INT_MIN, start;
	for(i = 0; i < n; i++) {
		for(j = 0; j < p->has; j++) {
			if(f[i].frame == p->n[j].pageno && p->n[j].use_times > maxuse) {
				start = j;
				maxuse = p->n[j].use_times;
				break;
			}
		}
	}
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < p->has; j++) {
			if(f[i].frame == p->n[j].pageno && p->n[j].use_times == maxuse) {
				min->l[min->total++] = i;
				break;
			}
		}
	}
	
	
	printf("\nMAX used times array is(indices in frames)  :  ");
	for(i = 0; i < min->total; i++) 
		printf("%d\t", min->l[i]);
	printf("\n\n\n");
	return;
}

int findleast(frames *f, int n, int total) {
	int mini = total, i, min = INT_MAX;
	for(i = 0; i < n; i++) {
		if(f[i].lastused < min) {
			min = f[i].lastused;
			mini = i;
		}
	}
	return mini;
}

void copy(frames *temp, frames *f, int n) {
	int i;
	for(i = 0; i < n; i++) {
		temp[i].frame = f[i].frame;
		temp[i].use = f[i].use;
		temp[i].modify = f[i].modify;
		temp[i].lastused = f[i].lastused;
		temp[i].firstused = f[i].firstused;	
	}
	return;
}

int check_exists(frames *f, int n, int tocheck) {
	int check;
	for(check = 0; check < n; check++) {
		if(f[check].frame == tocheck) 
			return check;
	}	
	return -1;
}

void print_frames(frames *f, int n) {
	int i;
	printf("Current page numbers in memory\t\t");			
	for(i = 0; i < n ; i++) 
		printf("%d\t", f[i].frame);
	printf("\n\n");
	return;
}

int do_FIFO(int *list, int total, frames *f, int n, int filled) {
	int check;
	int npf = 0;
	int toreplace = 0;
	int i;
	
	while(filled != total) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		
		//check if already exists
		if(check_exists(f, n, list[filled]) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[toreplace++].frame = list[filled++];
		}
		if(toreplace == n)
			toreplace = 0;
					
	}
	
	printf("No more pages required\n\n");
	print_frames(f, n);
	return npf;
}




int do_Optimal(int *list, int total, frames *f, int n, int filled) {
	int npf = 0;
	int i, j;
	int notagain[total], k = 0;
	int x = filled;
	int check;		
	//firstly fill free frames for next coming page numbers not in memory 
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].lastused = filled;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].firstused = filled;
			f[x].lastused = filled;
			f[x++].frame = list[filled++];
		}			
	}
	//now no free frames are available then we have to replace frames for new pages 	
	
	while(filled != total) {
		k = 0;
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].lastused = filled;
			filled++;
		}
		
		//if not exists then replace page required latest in near future 
		//or the first come page when more than one pages are never required in future
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			int max = -1, maxj = -1, flag;
			for(i = 0; i < n; i++) {
				flag = 0;
				for(j = filled + 1; j < total; j++) {
					if(f[i].frame == list[j]) {
						if(j > maxj) {
							max = i;
							maxj = j;
						}
						flag = 1;
						break;
					}
				}
				if(!flag) {
					notagain[k++] = i;
					//printf("not = %d\nk=%d\n", i, k);
				}
			}
			//all pages are required in future
			if(!k) {
				//replace latest required page
				f[max].frame = list[filled];
				f[max].firstused = filled;
				f[max].lastused = filled++;
			}
			else{
				int min = total;
				//find first come page to replace
				while(k > 0) {
					//printf("x = %d", notagain[k-1]);
					if(f[notagain[k - 1]].firstused < min)
						min = notagain[k - 1];
					k--;
				}
				//replace first come page
				f[min].frame = list[filled];
				f[min].firstused = filled;
				f[min].lastused = filled++;	
			}
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	return npf;		
}


int do_LRU(int *list, int total, frames *f, int n, int filled) {
	int npf = 0;
	int x = filled;
	int check;		
	int least;
	//firstly fill free frames for next coming page numbers not in memory 
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].lastused = filled;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].lastused = filled;
			f[x++].frame = list[filled++];
		}			
	}
	//now no free frames are available then we have to replace frames for new pages 	
	
	while(filled != total) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].lastused = filled;
			filled++;
		}
		
		//if not exists then replace page least recently used 
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			least = findleast(f, n, total);
			f[least].lastused = filled;
			f[least].frame = list[filled++];
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	return npf;
}
 
 
int do_LFU(int *list, int total, frames *f, int n, int filled) {
	int npf = 0, j, i;
	int x = filled;
	int check;
	int found;
	
	least min;
	init_min(&min, n);
	
	pages p;
	init_pages(&p, total);	
	
	for(i = 0; i < x; i++) 
		p.n[i].use_times++;	
	
	//firstly fill free frames for next coming page numbers not in memory 
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		
		printf("Count for each page used\n");
		for(j = 0; j < p.has; j++) 
			printf("(%d, %d)\t", p.n[j].pageno, p.n[j].use_times);
		printf("\n");
		
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			((p.n)[findindex(&p, list[filled])]).use_times++;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			p.n[p.has].pageno = list[filled];
			(p.n[p.has++].use_times)++;
			f[x].firstused = filled;
			f[x++].frame = list[filled++];
		}	
	}
	//now no free frames are available then we have to replace frames for new pages 	
	
	while(filled != total) {
		int min_first = total, mini = n;
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Count for each page used\n");
		for(j = 0; j < p.has; j++) 
			printf("(%d, %d)\t", p.n[j].pageno, p.n[j].use_times);
		printf("\n");
		
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\t%d\n\n\n", list[filled]);
			printf("----------------------------------------------------------------\n\n");
			((p.n)[findindex(&p, list[filled])]).use_times++;
			filled++;
		}
		
		//if not exists then replace page used least number of times 
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			if((found = findindex(&p, list[filled])) != -1)
				((p.n)[found]).use_times++;
			else {
				p.n[p.has].pageno = list[filled];
				(p.n[p.has++].use_times)++;
			}
			
			findleast_times(f, n, &p, &min);
			//what if there are multiple minimun no of times used pages
			if(min.total > 1) {
				//find first come from array of min
				while(min.total > 0) {
					if(f[min.l[min.total - 1]].firstused < min_first) {
						min_first = f[min.l[min.total - 1]].firstused;
						mini = min.l[min.total - 1];
					}
					min.total--;
				}
				//printf("There are multiple minimun no of times used pages\n");
				//printf("So replacing the first come page out of these i.e %d\n\n", f[mini].frame);
				//replace the first come page
				f[mini].frame = list[filled];
				f[mini].firstused = filled++;
			}
			else {
				f[min.l[0]].firstused = filled;
				f[min.l[0]].frame = list[filled++];
				min.total = 0;
			}
			
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	return npf;
}

int do_MFU(int *list, int total, frames *f, int n, int filled) {
	int npf = 0, j, i;
	int x = filled;
	int check;
	int found;
	
	least min;
	init_min(&min, n);
	
	pages p;
	init_pages(&p, total);		
	
	for(i = 0; i < x; i++) 
		p.n[i].use_times++;	
	
	//firstly fill free frames for next coming page numbers not in memory 
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		printf("Count for each page used\n");
		for(j = 0; j < p.has; j++) 
			printf("(%d, %d)\t", p.n[j].pageno, p.n[j].use_times);
		printf("\n");
		
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			((p.n)[findindex(&p, list[filled])]).use_times++;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			p.n[p.has].pageno = list[filled];
			(p.n[p.has++].use_times)++;
			f[x].firstused = filled;
			f[x++].frame = list[filled++];
		}	
	}
	//now no free frames are available then we have to replace frames for new pages 	
	
	while(filled != total) {
		int min_first = total, mini = n;
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Count for each page used\n");
		for(j = 0; j < p.has; j++) 
			printf("(%d, %d)\t", p.n[j].pageno, p.n[j].use_times);
		printf("\n");
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			((p.n)[findindex(&p, list[filled])]).use_times++;
			filled++;
		}
		
		//if not exists then replace page used most number of times 
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			if((found = findindex(&p, list[filled])) != -1)
				((p.n)[found]).use_times++;
			else {
				p.n[p.has].pageno = list[filled];
				(p.n[p.has++].use_times)++;
			}
			
			findmost_times(f, n, &p, &min);
			//what if there are multiple maximun no of times used pages
			if(min.total > 1) {
				//find first come from array of max(called as min for code simplicity(internal purposes))
				while(min.total > 0) {
					//printf("(%d, %d)\t", min.l[min.total - 1], f[min.l[min.total - 1]].firstused);
					
					if(f[min.l[min.total - 1]].firstused < min_first) {
						min_first = f[min.l[min.total - 1]].firstused;
						mini = min.l[min.total - 1];
					}
					min.total--;
				}
				//printf("\nThere are multiple maximun no of times used pages\n");
				//printf("So replacing the first come page out of these i.e %d\n\n", f[mini].frame);
				//replace the first come page
				f[mini].frame = list[filled];
				f[mini].firstused = filled++;
			}
			else {
				f[min.l[0]].frame = list[filled];
				f[min.l[0]].firstused = filled++;
				min.total = 0;
			}
			
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	return npf;
}

int do_S_first(int *list, int total, frames *f, int n, int filled) {
	int npf = 0, j;
	int x = filled;
	int check;
	int found;
	int ptr = n - 1;
	
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\t%d\n\n\n", check);
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].use = 0;
			f[x++].frame = list[filled++];
		}	
	}	
	
	while(filled != total) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		//if not then look for 0 and while doing so set all bypassed frames use bit to 0
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			ptr++;
			if(ptr == n)
				ptr = 0;
			
			while(f[ptr].use == 1) {
				f[ptr].use = 0;
				ptr++;
				if(ptr == n)
					ptr = 0;
			}
			f[ptr].frame = list[filled++];
			f[ptr].use = 0;
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	printf("Use status                    \t\t");
	for(j = 0; j < n; j++)
		printf("%d\t", f[j].use);
	printf("\n\n");
	return npf;
}


int do_S_second(int *list, int total, frames *f, int n, int filled) {
	int npf = 0, j;
	int x = filled;
	int check;
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\t%d\n\n\n", check);
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].use = 0;
			f[x].firstused = filled;
			f[x++].frame = list[filled++];
		}	
	}	
	
	while(filled != total) {
		int visit[n];
		int i;
		int doagain = 1;
		for(i = 0; i < n; i++)
			visit[i] = 0;
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		//if not then look for first come and if first come's use bit is 1 make it 0 and bypass it
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			while(doagain) {
				int min_first = total;
				int mini = n;
				/*printf("visit\n");
				for(i = 0; i < n; i++) {
					printf("%d\t", visit[i]);
				}*/
				printf("\n\n");
				for(j = 0; j < n; j++) {
					if(visit[j] != 1 && f[j].firstused < min_first) {
						min_first = f[j].firstused;
						mini = j;
					}
				}
				//replace the first come page
				if(f[mini].use == 0) {
					f[mini].frame = list[filled];
					f[mini].firstused = filled++;
					doagain = 0;
				}
				else {
					f[mini].use = 0;
					visit[mini] = 1; 
				}
				if(visitedall(visit, n)) {
					printf("visited all\n");
					for(i = 0; i < n; i++)
						visit[i] = 0;
				}
			}
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	printf("Use status                    \t\t");
	for(j = 0; j < n; j++)
		printf("%d\t", f[j].use);
	printf("\n\n");
	return npf;
}

int do_S_third(int *list, int total, frames *f, int n, int filled) {
	int npf = 0, j;
	int x = filled;
	int check;
	int found;
	int ptr = n - 1;
	
	for(j = 0; j < x; j++)
		f[j].use = 1;
	
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required\n\n");
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\t%d\n\n\n", check);
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].use = 1;
			f[x++].frame = list[filled++];
		}	
	}	
	
	while(filled != total) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			filled++;
		}
		
		//if not then look for 0 and while doing so set all bypassed frames use bit to 0
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			ptr++;
			if(ptr == n)
				ptr = 0;
			
			while(f[ptr].use == 1) {
				f[ptr].use = 0;
				ptr++;
				if(ptr == n)
					ptr = 0;
			}
			f[ptr].frame = list[filled++];
			f[ptr].use = 1;
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	printf("Use status                    \t\t");
	for(j = 0; j < n; j++)
		printf("%d\t", f[j].use);
	printf("\n\n");
	return npf;
}

int do_second_enhanced(int *list, int total, frames *f, int n, int filled, int *list_modify) {
	int npf = 0, j;
	int x = filled;
	int check;
	int found;
	int ptr = n - 1;
	
	for(j = 0; j < x; j++)
		f[j].use = 1;
	
	while(x < n) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required\n\n");
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		printf("Modify status                 \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].modify);
		printf("\n\n");
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\t%d\n\n\n", check);
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			if(list_modify[filled])
				f[check].modify = 1;
			else
				f[check].modify = 0;
			filled++;
		}
		
		else {
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			f[x].use = 1;
			if(list_modify[filled])
				f[x].modify = 1;
			else
				f[x].modify = 0;
			f[x++].frame = list[filled++];
		}	
	}	
	
	while(filled != total) {
		printf("----------------------------------------------------------------\n\n");
		printf("Next page number required = %d\n\n", list[filled]);
		
		print_frames(f, n);
		printf("Use status                    \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].use);
		printf("\n\n");
		printf("Modify status                 \t\t");
		for(j = 0; j < n; j++)
			printf("%d\t", f[j].modify);
		printf("\n\n");
		//check if already exists
		if((check = check_exists(f, n, list[filled])) != -1) {
			printf("No page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			f[check].use = 1;
			if(list_modify[filled])
				f[check].modify = 1;
			else
				f[check].modify = 0;
			filled++;
		}
		
		//if not then look for 0 0 if found then replace
		//if not then look for 0 1 and replace and while doing so set all bypassed frame's used bit to 0 
		else {
			int dowhile = 1;
			printf("Page fault\n\n\n");
			printf("----------------------------------------------------------------\n\n");
			npf++;
			
			ptr++;
			if(ptr == n)
				ptr = 0;
			
			while(dowhile) {
				int flag = 0;
				
				
				int temp = ptr;
					
				//looking for 0 0 
				while(f[ptr].use == 1 || (f[ptr].use == 0 && f[ptr].modify == 1)) {
					ptr++;
					if(ptr == n)
						ptr = 0;
					if(ptr == temp) {
						flag = 1;
						break;
					}
				}
								
				//if flag is set means there is no 0 0  so now look for 0 1
				//and while doing so set all bypassed frames use bit to 0
				if(flag) {
					flag = 0;
					while(f[ptr].use == 1) {
						f[ptr].use = 0;
						ptr++;
						if(ptr == n)
							ptr = 0;
						if(ptr == temp) {
							flag = 1;
							break;
						}
					}
					//if flag is set means that there is no 0 1 and as frames are bypassed
					//we will definitely find 0 0 or 0 1 in next loop
					if(flag)
						dowhile = 1;
				}
				//if flag is not set means 0 0 or 0 1 is found and hence repace it
				if(flag == 0)  {
					dowhile = 0;
					if(list_modify[filled])
						f[ptr].modify = 1;
					else
						f[ptr].modify = 0;
						
					f[ptr].frame = list[filled++];
					f[ptr].use = 1;
				}
			}
		}
	}
	printf("No more pages required\n\n");
	print_frames(f, n);
	printf("Use status                    \t\t");
	for(j = 0; j < n; j++)
		printf("%d\t", f[j].use);
	printf("\n\n");
	printf("Modify status                 \t\t");
	for(j = 0; j < n; j++)
		printf("%d\t", f[j].modify);
	printf("\n\n");
	return npf;
}



void init(frames *f, int n) {
	int i;
	for(i = 0; i < n; i++) {
		f[i].frame = -1;
		f[i].use = 0;
		f[i].modify = 0;
		f[i].lastused = -1;
		f[i].firstused = -1;
	}
	return; 
}

int main() {
	int n;
	int list[32];
	int list_modify[32];
	int i = 0, pageno, filled;
	int found;
	int fifo = -1;
	int optimal = -1;
	int lru = -1;
	int lfu = -1;
	int mfu = -1;
	int first = -1;
	int second = -1;
	int third = -1;
	int enhanced = -1;
	//maximum frames allocated for a process input(n)
	printf("Enter the size of frames allocated for a process : ");
	scanf("%d", &n);
	
	frames f[n], temp[n];
	
	init(f, n);
	
	int total = 0;
	
	printf("Enter the page numbers required by the process (enter -1 to stop)\n");
	
	for(i = 0; i < 32; i++) {
		scanf("%d", &pageno);
		if(pageno == -1)
			break;
		list[i] = pageno;
		total++;
	}
	
	for(i = 0; i < total; i++) 
		list_modify[i] == 0;
	
	printf("Enter the corrosponding modify status for each page\n");
	for(i = 0; i < total; i++) 
		scanf("%d", &list_modify[i]);	

	//Enter pages already in main memory
	printf("Enter the page numbers which are already in memory (enter -1 to stop):\n");
	
	i = 0;
	
	while(1) {
		if(i == n) {
			printf("Limit reached\n");
			break;
		}
		scanf("%d", &pageno);
		if(pageno == -1)
			break;
		
		f[i].frame = pageno;
		f[i].firstused = i;
		f[i].lastused = i;
		
		
		if(list_modify[i]) 
			f[i].modify = 1;
		else
			f[i].modify = 0;
		
		f[i++].use = 0;
	}
	
	filled = i; 
	int k;
	
	int option = 0;
	while(option != 10) {
		copy(temp, f, n);
		printf("Enter the page replacement algorithm to be used.\n");
		printf("1.FIFO\n2.Optimal\n3.LRU\n4.LFU\n5.MFU\n6.Second chance: First method\n7.\t\tSecond method\n");
		printf("8.\t\tThird method\n9.Enhanced Second chance\n10.Stop\n");
		scanf("%d", &option);
		switch(option) {
			case 1:
				printf("Total pagefaults = %d\n\n\n", fifo = do_FIFO(list, total, temp, n, filled));
				break;
			case 2:
				printf("Total pagefaults = %d\n\n\n", optimal = do_Optimal(list, total, temp, n, filled));
				break;
			case 3:
				printf("Total pagefaults = %d\n\n\n", lru = do_LRU(list, total, temp, n, filled));
				break;
			case 4:
				printf("Total pagefaults = %d\n\n\n", lfu = do_LFU(list, total, temp, n, filled));
				break;
			case 5:
				printf("Total pagefaults = %d\n\n\n", mfu = do_MFU(list, total, temp, n, filled));
				break;
			case 6:
				printf("Total pagefaults = %d\n\n\n", first = do_S_first(list, total, temp, n, filled));
				break;
			case 7:
				printf("Total pagefaults = %d\n\n\n", second = do_S_second(list, total, temp, n, filled));
				break;
			case 8:
				printf("Total pagefaults = %d\n\n\n", third = do_S_third(list, total, temp, n, filled));
				break;
			case 9:
				printf("Total pagefaults = %d\n\n\n", enhanced = do_second_enhanced(list, total, temp, n, filled, list_modify));
				break;
			case 10:
				break;
			default:
				printf("Enter proper option\n");
		}
	}
	printf("Summary : Total page faults for\n");
	if(fifo != -1)
		printf("FIFO                   :\t%d\n\n", fifo);
	if(optimal != -1)
		printf("Optimal                :\t%d\n\n", optimal);
	if(lru != -1)
		printf("LRU                    :\t%d\n\n", lru);
	if(lfu != -1)
		printf("LFU                    :\t%d\n\n", lfu);
	if(mfu != -1)
		printf("MFU                    :\t%d\n\n", mfu);
	if(first != -1)
		printf("Second chance: first   :\t%d\n\n", first);
	if(second != -1)
		printf("Second chance: second   :\t%d\n\n", second);
	if(third != -1)
		printf("Second chance: third   :\t%d\n\n", third);
	if(enhanced != -1)
		printf("Enhanced second chance :\t%d\n\n", enhanced);
	return 0;
}
