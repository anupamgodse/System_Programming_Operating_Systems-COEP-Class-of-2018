#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>


int search(char *new) {
	int i =0;
	char sym[128];
	FILE *f = fopen("symtab", "r+");
	while(fscanf(f, "%s", sym) != EOF) {
		if(!i) {
			i = 1;
			if(strcmp(new, sym) == 0) { 
				return 1;
			}
		}
		else
			i = 0;
					
	}
	return 0;
}
int search_val(char *val) {
	int i =0;
	char v[128];
	FILE *f = fopen("symtab", "r+");
	while(fscanf(f, "%s", v) != EOF) {
		if(!i) {
			i = 1;
		}
		else {
			if(strcmp(v, val) == 0) { 
				return 1;
			}
			i = 0;
		}			
	}
	return 0;
}
int main() {
	int option, i, f, l, l1, d;
	char new[128], value[128], sym[128], x[128];
	FILE *fp = fopen("symtab", "r+");
	if(!fp) {
		perror("Error Opening File\n");
		return errno;
	}
  	while(1) {
  		i = 0;
  		f = 0;	
		fseek(fp, 0, SEEK_SET);
		printf("Options\n1.Insert\n2.Modify\n3.Search\n");
		scanf("%d", &option);
		switch(option) {
			case 1:
				printf("Enter <symbol> space <value>\n");
				scanf("%s%s", new, value);
				d = search(new);
				if(d) {
					printf("Duplicate\n");
					break;
				}	
				if(search_val(value)) {
					printf("Duplicate\n");
					break;
				}	
				fseek(fp, 0, SEEK_END);
				fwrite(new, strlen(new) * sizeof(char), 1, fp);
				fwrite("\t", sizeof(char), 1, fp);
				fwrite(value, strlen(value) * sizeof(char), 1, fp);		
				fwrite("                                                 \n", sizeof(char) * strlen("                                                 \n"), 1, fp);
				break;
				
			case 2:
				printf("Enter <symbol> space <newvalue>\n");
				scanf("%s%s", new, value);
				if(search_val(value)) {
					printf("Duplicate\n");
					break;
				}	
				while(fscanf(fp, "%s", sym) != EOF) {
					if(!i) {
						i = 1;
						if(strcmp(new, sym) == 0) { 
							f = 1;
							break;
						}
					}
					else
						i = 0;
					
				}
				if(f) {
					fscanf(fp, "%s", x);
					fseek(fp, ftell(fp) - strlen(x), SEEK_SET);
					if((l = strlen(x)) > (l1 = strlen(value))) {
						for(i = 0; i < l1; i++) 
							fwrite(value + i, sizeof(char), 1, fp);
						for(i = l1; i < l; i++)
							fwrite(" ", sizeof(char), 1, fp);
					}
					else 
						fwrite(value, strlen(value) * sizeof(char), 1, fp);
					printf("%s\t%s\n", new, value); 
				}
				else 
					printf("%s symbol not found\n", new); 
				break;
			case 3:
				printf("Enter <symbol> to find\n");
				scanf("%s", sym);
				while(fscanf(fp, "%s", new) != EOF) {
					if(!i) {
						i = 1;
						if(strcmp(new, sym) == 0) { 
							f = 1;
							break;
						}
					}
					else
						i = 0;
					
				}
				if(f) {
					fscanf(fp, "%s", value);
					printf("%s\t%s\n", sym, value); 
				}
				else 
					printf("%s symbol not found\n", sym); 
				break;
			case 4:
				return 0;			
			default:
				printf("Enter correct option\n");
		}
	}
}

