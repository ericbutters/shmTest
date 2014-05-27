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

int main () {
	int fd = shm_open("dkshm", O_RDWR, S_IRUSR | S_IWUSR);
	if(fd < 0)
		return -1;
		
	if (ftruncate(fd, sizeof(struct region)) == -1)
		return -2;
		
	rptr = mmap(NULL, sizeof(struct region),
		   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (rptr == MAP_FAILED)
		goto dexit;
		
	printf("Press enter to continue \n");
	getchar();
	printf("READ-REGION: len=%u data: %s\n",rptr->len,rptr->buf);
dexit:
	printf("EXIT NOW \n");
	munmap(rptr,sizeof(struct region));
	shm_unlink("dkshm");
	return 0;
}
