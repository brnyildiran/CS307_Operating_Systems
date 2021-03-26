#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	FILE *LoremPtr;
	char ch;
	int counterA = 0;

	time_t end, begin;
	
    begin = time(NULL);

	LoremPtr = fopen("loremipsum.txt", "r");

	while ((ch = fgetc(LoremPtr)) != EOF)
	{
		if(ch == 'a')
			counterA++;
	}
	fclose(LoremPtr);

	end = time(NULL);

	printf("There are %d 'a' characters in the file.\n", counterA);

    // calculate elapsed time by finding difference (end - begin)
    printf("Time taken by program is : %d sec.\n", (end - begin));

	return 0;
}