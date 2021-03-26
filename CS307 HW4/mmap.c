#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main()
{
	int fd = open ("loremipsum.txt", O_RDONLY); /* Open the file for reading. */
	struct stat s;
	size_t size;
	int status = fstat(fd, &s);
	size = s.st_size;

	int counterAA = 0;
	int i = 0;

	time_t end, begin;
    begin = time(NULL);

	char *ptr = mmap(0,size, PROT_READ,MAP_PRIVATE, fd,0);
	if(ptr == MAP_FAILED)
	{
		printf("Mapping Failed\n");
		return 1;
	}

	//***************************
	else if(ptr != MAP_FAILED)
	{
		for(i; i < size+1 ; i++)
		{
			if (ptr[i] == 'a')
			{
				counterAA++;
			}
		}
	}
	end = time(NULL);

	printf("There are %d 'a' characters in the file.\n", counterAA);

	// calculate elapsed time by finding difference (end - begin)
    printf("Time taken by program is : %d sec.\n", (end - begin));
	//***************************
	
	close(fd);	
	return 0;
}