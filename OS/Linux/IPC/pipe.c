#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define PIPE_PATH "/tmp/myFifo"
#define MAX_SIZE 4096

int main(int argc, char* argv[]) {
	if(argc>1) {
		int res = mkfifo(PIPE_PATH, 0666);
		if (res == 0) {
        	printf("FIFO created\n");
    	} else {
    		perror("Create FIFO ");
    		exit(-1);
    	}
	}

	size_t sizeTmp = -1;
	char bufRead[MAX_SIZE];
	int pipeFd = open(PIPE_PATH, O_RDWR);
	if(pipeFd < 0) {
		perror("Open pipe ");
		exit(-1);
	}

    while(1) {
    	if(argc>1) {
    		printf("#Please input message : ");
    		fflush(stdout);

    		sizeTmp = read(0, bufRead, sizeof(bufRead));

    		if(sizeTmp < 0) {
    			printf("Get message from treamal failed!\n");
    			exit(-1);
    		} else if(sizeTmp == 0) {
    			printf("Read closed!\n");
    			return 1;
    		} else {
    			bufRead[sizeTmp] = '\0';
    			write(pipeFd, bufRead, strlen(bufRead));
    		}
    	} else {
    		fflush(stdout);
    		sizeTmp = read(pipeFd, bufRead, sizeof(bufRead));
    		bufRead[sizeTmp] = '\0';
    		printf("#Read the message : %s", bufRead);
    	}

    	if(!(strcmp(bufRead, "quit\n"))) {
    		return 0;
    	}
    }
    exit(EXIT_SUCCESS);
}