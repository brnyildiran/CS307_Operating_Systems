/* Airline Reservation System */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
using namespace std;

int M[2][50]={ };
bool flagofthreads = true;
int turn = 0;
int countSeat = 100;

int seat_loc1(int ss) //Determine 2nd index of the matrix
{
	int result_ss;
	if(ss <= 50)
	{
		result_ss = ss-1;
	}
	else
	{
		result_ss = ss-51;
	}
	return result_ss;
}

int seat_loc0(int s) //Determine 2nd index of the matrix
{
	int result_s;
	
	if(s <= 50)//Write to the 1st row
	{
		result_s = 0;
	}
	else //Write to the 2nd row
	{
		result_s = 1;
	}
	return result_s;
}

void *reservation_sys1(void * ) //TRAVEL AGENCY 1
{
    while(flagofthreads)
    {   
        //BUSY WAIT
        while(turn != 0)
        {} 
        
        //CRITICAL REGION
        printf("Agency 1 Entered Critical Region. \n");

        bool isEmptySeat = false;
        int seatnumber = rand() % 100 + 1; //Random number between 1-100
        if(M[seat_loc0(seatnumber)][seat_loc1(seatnumber)] == 0) //If seatnumber is empty
        {
            while(!isEmptySeat && flagofthreads)
            {
                isEmptySeat = true;
                M[seat_loc0(seatnumber)][seat_loc1(seatnumber)]= 1; //Reserve the seat
                printf("Seat Number %d is reserved by Agency 1. \n", seatnumber);
                countSeat--;
            }
        }
        printf("Agency 1 Exit Critical Region. \n\n"); //CRITICAL REGION EXIT
        turn = 1;
        
    }
}

void *reservation_sys2(void * )  //TRAVEL AGENCY 2
{          
    while(flagofthreads) 
    {
        //BUSY WAIT
        while(turn != 1) 
        {} 
        
        //CRITICAL REGION
        printf("Agency 2 Entered Critical Region. \n");
        int seatnumber = rand() % 100 + 1; //Random number between 1-100
        bool isEmptySeat = false;
        if(M[seat_loc0(seatnumber)][seat_loc1(seatnumber)] == 0) //If seatnumber is empty
        {
            while(!isEmptySeat && flagofthreads)
            {
                isEmptySeat = true;
                M[seat_loc0(seatnumber)][seat_loc1(seatnumber)]= 2; //Reserve the seat
                printf("Seat Number %d is reserved by Agency 2. \n", seatnumber);
                countSeat--;
            }
        }
        printf("Agency 2 Exit Critical Region. \n\n"); //CRITICAL REGION EXIT
        turn = 0;  
    }  
}

int main()
{
    srand(time(NULL));
	pthread_t TravelAgency1, TravelAgency2;
	
	pthread_create(&TravelAgency1,NULL,reservation_sys1, NULL);
	pthread_create(&TravelAgency2,NULL,reservation_sys2, NULL);
	
	while(flagofthreads)  // While executing
	{
        if (countSeat == 0) // Seats are full
        {
            flagofthreads = false; // Stop execution
        }
    }
    printf("No seats left. \n\n");
    
    pthread_join(TravelAgency1,NULL);
	pthread_join(TravelAgency2,NULL);

    //PRINT THE SEATS
    printf("Plane is full: \n");
    for (int i = 0; i < 2; i++) 
    { 
        for (int j = 0; j < 50; j++) 
        { 
            cout << M[i][j] << " "; 
        } 
        cout << endl; 
    }
	return 0;
}