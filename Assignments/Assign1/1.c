#include<stdio.h>
#include<unistd.h>
#include<errno.h>
int main(int argc, char *argv[]){
	FILE *fp;
	int i = 0;
	char arr[128];
	fp = fopen(argv[1], "r");
	if(!fp) {
		perror("Error opening File\n");
		return errno;
	}
	
	printf("Name\t\t\tTelephone\n");
	
		
	while(fscanf(fp, "%s", arr) != EOF) {
		if(!i) {
			printf("%s\t\t\t", arr);
			i = 1;
		}
		else {
			printf("%s\n", arr);
			i = 0;
		}
		
	}	
	return 0;
}
