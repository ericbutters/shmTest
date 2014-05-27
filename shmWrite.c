#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LEN 1024
struct region {        /* Defines "structure" of shared memory */
    int len;
    char buf[MAX_LEN];
};
struct region *rptr;

char pWrite[]="Hello Shared Memory!";

int main () {
	int fd = shm_open("dkshm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd < 0)
		return -1;
		
	if (ftruncate(fd, sizeof(struct region)) == -1)
		return -2;
		
	rptr = mmap(NULL, sizeof(struct region),
		   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (rptr == MAP_FAILED)
		goto dexit;
		
	printf("REGION: len=%u  strlen is %d\n",rptr->len,strlen(pWrite));

	printf("Press key to continue \n");
	getchar();
	sprintf(rptr->buf,"%s",pWrite);
	rptr->len = strlen(pWrite);
	
	printf("REGION: len=%u \n",rptr->len);
	
dexit:
	sleep(10);
	printf("EXIT NOW \n");
	munmap(rptr,sizeof(struct region));
	shm_unlink("dkshm");
	return 0;
}
