#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
int main(int argc, char *argv[]){
	FILE *fp;
	int i = 0, option, f = 0, l, l1, x;
	char arr[128], name[128], telephone[12], query[128], query1[12];
	fp = fopen(argv[1], "r+");
	if(!fp) {
		perror("Error opening File\n");
		return errno;
	}
	while(1) {
		fseek(fp, 0, SEEK_SET);
		i = 0, f = 0;
		printf("Options\n1.Find telephone number? Enter name.\n2.Find name? Enter telephone number\n3.Update telephone number? Enter name and new number\n4.Exit\n");
		scanf("%d", &option);
		
		switch(option){
			case 1:
				scanf("%s", query);
				while(fscanf(fp, "%s", name) != EOF) {
					if(!i) {
						i = 1;
						if(strcmp(query, name) == 0) { 
							f = 1;
							break;
						}
					}
					else
						i = 0;
					
				}
				if(f) {
					fscanf(fp, "%s", telephone);
					printf("Telephone number for %s is %s\n", name, telephone); 
				}
				else 
					printf("No entry for %s\n", name); 
				break;
			case 2:
				scanf("%s", query);
				while(1) {
					if(!i) { 
						if(fscanf(fp, "%s", name) == EOF) break;
						i = 1;
					}
					else {
						if(fscanf(fp, "%s", telephone) == EOF) break;
					
						if(strcmp(query, telephone) == 0) {
							f = 1;
							break;
						}
						i = 0;
					}
					
				}
				if(f)
					printf("Name having %s as telephone number is %s\n", telephone, name);
				else
					printf("No entry for %s\n", telephone); 				
				break;
			case 3:
				scanf("%s%s", query, query1);
				while(fscanf(fp, "%s", name) != EOF) {
					if(!i) {
						i = 1;
						if(strcmp(query, name) == 0) { 
							f = 1;
							break;
						}
					}
					else
						i = 0;
					
				}
				if(f) {
					fscanf(fp, "%s", telephone);
					fseek(fp, x = ftell(fp) - (l = strlen(telephone)), SEEK_SET);
					fwrite(query1, sizeof(char) * l, 1,  fp);
					printf("Changed telephone number of %s from %s to %s\n", name, telephone, query1); 
				}
				else 
					printf("No entry for %s\n", name); 
				
				break;
				
				case 4:
					return 0;
				
				default:
					printf("Enter proper option\n");			
		}
	}
	return 0;
}
