#include <iostream>
#include <fstream>
#include <string>
#include "time.h"
using namespace std;

int main()
{
	fstream lorem;
	char ch;
	int counter_A = 0;
	lorem.open("loremipsum.txt");

	time_t end, begin;
	
    begin = time(NULL);

	while(lorem.get(ch))
	{
		if(ch == 'a')
			counter_A++;

	}
	lorem.close();

	// Recording the end clock tick. 
    end = time(NULL);

	cout << "There are " << counter_A << " 'a' characters in the file." << endl;

	// Calculating total time taken by the program. 
    double time_taken = double(end - begin); 
    cout << "Time taken by program is : " << time_taken << " sec." << endl; 

	return 0;
}