#include<stdio.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h>
#include<errno.h>
int main(int argc, char *argv[]) {
	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "r");
	FILE *f3 = fopen(argv[3], "w");
	if(!f1 || !f2 || !f3) {
		perror("Open Failed\n");
		return errno;
	}
	char n1[12], n2[12];
	int i1, i2;
	while(fscanf(f1, "%s", n1) != EOF && fscanf(f2, "%s", n2) != EOF){
		if((i1 = atoi(n1)) <= (i2 = atoi(n2))) {
			fwrite(n1, sizeof(char) * strlen(n1), 1, f3);
			fwrite("\n", sizeof(char), 1, f3);
			fseek(f2, ftell(f2) - strlen(n1), SEEK_SET);
		}
		else {
			fwrite(n2, sizeof(char) * strlen(n2), 1, f3);
			fwrite("\n", sizeof(char), 1, f3);
			fseek(f1, ftell(f1) - strlen(n1), SEEK_SET);
		}
	}
	while(fscanf(f1, "%s", n1) != EOF) { 
		fwrite(n1, sizeof(char) * strlen(n1), 1, f3);
		fwrite("\n", sizeof(char), 1, f3);
	}
	while(fscanf(f2, "%s", n2) != EOF) {
		fwrite(n2, sizeof(char) * strlen(n2), 1, f3);
		fwrite("\n", sizeof(char), 1, f3);
	}
	return 0;
}
