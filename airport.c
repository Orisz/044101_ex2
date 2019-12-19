//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex2.h"
#include "flight.h"
#include "runway.h"
#include "airport.h"

runway_elem* airport;//our global pointer for airport

//************************************************************************************************************
//* Function name : init
//* Description   : the function initials the runway_elem* airport and points it's pointers to null
//* Parameters    : None.
//* Return value  : None.
//************************************************************************************************************

void init()
{
	airport = (runway_elem*)malloc(sizeof(runway_elem));
	if (airport == NULL)
	{
		return;
	}
	airport->cur_runway = NULL;
	airport->next_runway = NULL;
	return;
}

//************************************************************************************************************
//* Function name : is_valid_dest2
//* Description   : the function checks if the destination of a givem flight has 3 big letters
//* Parameters    : dest - pointer to char with the destination that need to check if it is llegal
//* Return value  : the function returns TRUE if the destination is llegal and FALSE if don't
//************************************************************************************************************

BOOL is_valid_dest2(char* dest)
{
	int i = 0;
	int len = strlen(dest);

	if (len != 3)
	{
		return FALSE;
	}
	for (i = 0; i < len; i++)
	{
		if (dest[i] < 65 || dest[i] > 90)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//************************************************************************************************************
//* Function name : numOfFlights2Dest
//* Description   : the function counts the number of flights in the given runway which their destination 
//* is dest according to emergency  
//* Parameters    : runway - the runway we count the number of flights in it
//*					dest - count the number of flight with destinaion dest
//*					emergency - the type of the flights we want to count
//* Return value  : the function return int with the number of flights we count
//************************************************************************************************************

int numOfFlights2Dest(p_runway runway, char* dest , BOOL emergency)//inner helper function 4 the delay function
{
	if (runway == NULL)
	{
		return -1;
	}
//* if the runway pointer is null return -1 as in the instructions
	int counter = 0;
	flight_next* tmp = runway->first_flight;
//* the loop counts the number of flights 
	while (tmp != NULL)
	{
		if (!strcmp(tmp->cur_flight->dest, dest) && tmp->cur_flight->is_emergency == emergency)
		{
			counter++;
		}
		tmp = tmp->next_flight;
	}
	return counter;
}

//************************************************************************************************************
//* Function name : addRunway
//* Description   : the function recives the number of the runway and the type of it we want to add to airport
//*					if it will succeed we shall return 'SUCCESS' and if not we return 'FAILURE'
//* Parameters    : 'runway_num' - number of the runway to add 
//*                 'runway_type' the type of the runway(domestic/international)
//* Return value  : if the runway wass added will return succes else will return failure
//************************************************************************************************************

Result addRunway(int runway_num, FlightType runway_type)
{
//*  check if the input is valid
	if (runway_num > MAX_ID || runway_num <= 0 || (runway_type != DOMESTIC && runway_type != INTERNATIONAL))
	{
		return FAILURE;
	}
	if (airport == NULL)//check if the airport struct exist if not we need to initialize 'airport'
	{
		init();
	}
	if (airport->cur_runway == NULL)//check if airport exist but its empty so we need to create the runway
	{
		p_runway new_runway = createRunway(runway_num, runway_type);
		if (new_runway == NULL)
		{
			return FAILURE;
		}
		airport->cur_runway = new_runway;
		return SUCCESS;
	}
	runway_elem* tmp = airport;//pointer to runway element (runways linked list elements)
	while (tmp->next_runway != NULL)//run till the end of the list
	{
		if (tmp->cur_runway->runway_num == runway_num)//check if the runway num is already exists
		{
			return FAILURE;
		}
		tmp = tmp->next_runway;
	}
	if (tmp->cur_runway->runway_num == runway_num)//check the last runway number
	{
		return FAILURE;
	}
	p_runway new_runway = createRunway(runway_num, runway_type);//creating the new runway
	if (new_runway == NULL)
	{
		return FAILURE;
	}
	runway_elem* new_runway_elem = (runway_elem*)malloc(sizeof(runway_elem));//creating the runway elem linked list struct to put new runway in him
	if (new_runway_elem == NULL)
	{
		destroyRunway(new_runway);
		return FAILURE;
	}
	tmp->next_runway = new_runway_elem;//connect the runway elem to the list
	new_runway_elem->cur_runway = new_runway;//filing the list elem with the new runway
	new_runway_elem->next_runway = NULL;//now he is the last one on the list
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : removeRunway
//* Description   : the function will search for the runway with the corresponding input number
//*					if we found it we will return 'SUCCESS' else we will return 'FAILURE'
//* Parameters    :'runway_num' - the number of the runway we want to remove
//* Return value  : Result - 'SUCCESS' if we took out the runway and 'FAILURE' if somwthing went wrong 
//************************************************************************************************************

Result removeRunway(int runway_num)
{	//check if runway number is invalid 
	if (runway_num > MAX_ID || runway_num <= 0)
	{
		return FAILURE;
	}
	runway_elem* tmp = airport;//we need tmp so we dont lose the pointer to the prev runway
	if (tmp == NULL) // airport is empty
	{
		return FAILURE;
	}
	runway_elem* to_remove = tmp->next_runway;//check if this is the runway to remove
	if (tmp->cur_runway->runway_num == runway_num)//edge case remove the 1st runway
	{
		airport = tmp->next_runway;
		destroyRunway(tmp->cur_runway);
		free(tmp);
		return SUCCESS;
	}
	//loop over the runways list and if 'to_remove' is the runway we look for
	//we remove it and update the pointers using 'tmp'
	while (to_remove->next_runway != NULL)
	{
		if (to_remove->cur_runway->runway_num == runway_num)
		{
			tmp->next_runway = to_remove->next_runway;//updating the pointers
			destroyRunway(to_remove->cur_runway);
			free(to_remove);
			return SUCCESS;
		}
		tmp = tmp->next_runway;
		to_remove = to_remove->next_runway;
	}
	//the loop dosn't check if the last runway_elem is the runway we seek so we check it manually 
	//here at the end of the loop
	if (to_remove->cur_runway->runway_num == runway_num)
	{
		tmp->next_runway = NULL;
		destroyRunway(to_remove->cur_runway);
		free(to_remove);
		return SUCCESS;
	}
	return FAILURE;
}

//************************************************************************************************************
//* Function name : addFlightToAirport
//* Description   : the function gets all the paramaters for a new flight creat it and then 
//*					puts it inside the airport according to the rules 
//* Parameters    : 'flight_num' - number of the flight to add , 'flight_type' - the type of the flight
//*                 'dest' - the destenation of the flight , 'is_emergency' - classification of the flight emergency or regular
//* Return value  : Result - 'SUCCESS' if the flight wass added else we retun 'FAILURE'
//************************************************************************************************************

Result addFlightToAirport(int flight_num, FlightType flight_type, char* dest, BOOL is_emergency)
{
//check if the input is valid
	if (airport == NULL || airport->cur_runway == NULL || flight_num > MAX_ID || flight_num <= 0 || (flight_type != DOMESTIC && flight_type != INTERNATIONAL))
	{
		return FAILURE;
	}
	int num_of_flights = MAX_ID + 1;
	int num_of_runway = MAX_ID + 1;
	runway_elem* tmp = airport;//loop pointer
	runway_elem* best_runway = NULL;//pointer to the currnt best candidact runway to insert the flight to
	while (tmp != NULL)//loop over the entire runways
	{
		if (isFlightExists(tmp->cur_runway, flight_num))//check if the flight already there
		{
			return FAILURE;
		}
		if (tmp->cur_runway->runway_type == flight_type)//same type flight and runway(D/I)?
		{
			int cur_flights = getFlightNum(tmp->cur_runway);//number of flights in the runway
			if (cur_flights != -1 && cur_flights < num_of_flights)//check if this runway has less flights in it then the last one
			{//pick the new best runway and updating the "best candidact runway" features
				num_of_flights = cur_flights;
				num_of_runway = tmp->cur_runway->runway_num;
				best_runway = tmp;
			}
			else if (cur_flights >= 0 && cur_flights == num_of_flights)//if we have the same number of flights in 2 runways
			{//pick the one with the lower runway number
				if (tmp->cur_runway->runway_num < num_of_runway)
				{
					num_of_runway = tmp->cur_runway->runway_num;
					best_runway = tmp;
				}
			}
		}
		tmp = tmp->next_runway;
	}
	if (best_runway == NULL)//we didnt find a runway to insert the flight in it
	{
		return FAILURE;
	}
	char* destination = dest;
	p_flight new_flight = createFlight(flight_num, flight_type, destination, is_emergency);//creating the flight struct
	if (new_flight == NULL)
	{
		return FAILURE;
	}
	if (addFlight(best_runway->cur_runway, new_flight) == FAILURE)//adding the flight to the list and check for inner malloc success
	{
		destroyFlight(new_flight);
		return FAILURE;
	}
	destroyFlight(new_flight);//destroy the copy of the flight the airport has
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : departFromRunway
//* Description   : the function gets a number of runway and a number of flights we want to desatch from him
//*					if we did it return 'SUCCESS' else 'FAILURE'
//* Parameters    : 'runway_num - the number of the runway , 'num_flights' - how many flights we wish to dispatch  
//* Return value  : Result 'SUCCESS' if we took off the number of flights needed. else 'FAILURE'
//************************************************************************************************************

Result departFromRunway(int runway_num, int num_flights)
{
	int i = 1;
	if(airport == NULL)//we have no airport
	{
		return FAILURE;
	}
	if (airport->cur_runway == NULL)//we have no runways
	{
		return FAILURE;
	}
	runway_elem* tmp = airport;
	while (tmp != NULL)//loop over the runways
	{
		if (tmp->cur_runway->runway_num == runway_num)//find the right runway
		{
			if (num_flights <= getFlightNum(tmp->cur_runway))//can we remove as many flights as needed?
			{
				for (i = 1; i <= num_flights; i++)//loop over the flights and remove the number of them needed
				{
					depart(tmp->cur_runway);
				}
				return SUCCESS;
			}
			return FAILURE;
		}
		tmp = tmp->next_runway;
	}
	return FAILURE;
}

//************************************************************************************************************
//* Function name : changeDest
//* Description   : the function gets a destination , search for all the flights to this destination and change it 
//*					to the new destination.retun  Result.
//* Parameters    : 'origin_dest' - the current flight destination , 'new_dest' - the new flight destination
//* Return value  : Result 'SUCCESS' if we changed the dest as need or if there were no flights to origin dest. else 'FAILURE'
//************************************************************************************************************

Result changeDest(char* origin_dest, char* new_dest)
{
	if (!(is_valid_dest2(origin_dest)) || !(is_valid_dest2(new_dest)))//valid input?
	{
		return FAILURE;
	}
	if (airport == NULL || airport->cur_runway == NULL)//airport exist or empty?
	{
		return SUCCESS;
	}
	runway_elem* tmp_runway = airport;
	flight_next* tmp_flight = airport->cur_runway->first_flight;
	while (tmp_runway != NULL)//loop over the runways
	{
		while (tmp_flight != NULL)//loop over the flights in each runway
		{
			if (!strcmp(tmp_flight->cur_flight->dest, origin_dest))//found flight that holds the condition
			{//put in the dest field the new destination(as a string)
				free(tmp_flight->cur_flight->dest);
				tmp_flight->cur_flight->dest = (char*)malloc(sizeof(char) * 4);
				if (tmp_flight->cur_flight->dest == NULL) { return FAILURE; }
				strcpy(tmp_flight->cur_flight->dest, new_dest);
			}
			tmp_flight = tmp_flight->next_flight;
		}
		tmp_runway = tmp_runway->next_runway;
		if (tmp_runway != NULL)//runway ended go to the first flight in the next runway
		{
			tmp_flight = tmp_runway->cur_runway->first_flight;
		}
	}
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : delay
//* Description   : the function gets a destination , search for all the flights to this destination and
//*					delaying the flights according to the rules
//* Parameters    : 'dest' - the destination we want flights heading to , to delay
//* Return value  : Result 'SUCCESS' if we delayed all the flights to dest as needed or if there were no flights to this dest. else 'FAILURE'
//************************************************************************************************************

Result delay(char* dest)
{
	if (airport == NULL)//check if airport exist
	{
		return SUCCESS;
	}
	if (!(is_valid_dest2(dest)))//checking if valid input
	{
		return FAILURE;
	}
	runway_elem* tmp_runway = airport;//runway ptr
	flight_next* tmp_flight = airport->cur_runway->first_flight;//flight ptr
	flight_next* continue_from_here = airport->cur_runway->first_flight;//so we remember what is the next flight to check 
	int flightCounter = 0;//number of flights in each runway for the loops
	int toMoveEmer = 0;//if we have 'n' emergency flights to move there can not be more then 'n' emergency flights to move
	int j = 0;
	int toMoveReg = 0;//if we have 'n' regular flights to move there can not be more then 'n' regular flights to move
	int k = 0;

	Result memCheck = SUCCESS;//for all the the called upon functions that returns 'Result' type
	while (tmp_runway != NULL)// RUNWAYS LOOP !
	{
		tmp_flight = tmp_runway->cur_runway->first_flight;//init flight ptr
		flightCounter = getFlightNum(tmp_runway->cur_runway);//number of flights in currnet runway
		toMoveEmer = numOfFlights2Dest(tmp_runway->cur_runway, dest , TRUE);//how many emergency flights to dest we have
		toMoveReg = numOfFlights2Dest(tmp_runway->cur_runway, dest, FALSE);//how many regular flights to dest we have
		if (flightCounter == -1 || toMoveEmer == -1 || toMoveReg == -1) { return FAILURE; }//from some reason getFlightNum failed meaning ptr to runway isnt good
		if ((toMoveEmer + toMoveReg) == 0)//have no flights to move
		{
			tmp_runway = tmp_runway->next_runway;
			continue; //if there are no flight to the desired dest we should move to the next runway
		}
		if ((toMoveEmer + toMoveReg) == 1 && flightCounter == 1)//runway should remain the same only 1 flight inside
		{
			tmp_runway = tmp_runway->next_runway;
			continue; 
		}
		j = 0;
		k = 0;

		while (tmp_flight != NULL && (k < toMoveReg || j < toMoveEmer))// FLIGHTS LOOP !
		{
			if (!strcmp(tmp_flight->cur_flight->dest, dest))//desired dest
			{
				if (j < toMoveEmer && tmp_flight->cur_flight->is_emergency == TRUE)
				{	//we still have emergency flights to move

					continue_from_here = tmp_flight->next_flight;//next flight after we move tmp
					p_flight copy_flight = createFlight(tmp_flight->cur_flight->flight_num, tmp_flight->cur_flight->flight_type, tmp_flight->cur_flight->dest, tmp_flight->cur_flight->is_emergency);
					if (copy_flight == NULL)
					{
						return FAILURE;//checking we got the copy as needed else return failure
					}
					//take out 'tmp flight' from the list
					removeFlight(tmp_runway->cur_runway, tmp_flight->cur_flight->flight_num);
					//put it ('copy_flight')back in according to the rules
					memCheck = addFlight(tmp_runway->cur_runway, copy_flight);
					if (memCheck == FAILURE) { return FAILURE; }
					destroyFlight(copy_flight);//destroy the copy we made to release the memory
											   // moving the pointers and counters
					tmp_flight = continue_from_here;
					j++;//we moved a desired flight.with 'j' and to 'toMoveEmer' we make sure we wont move it again
				}
				else if (j == toMoveEmer && tmp_flight->cur_flight->is_emergency == TRUE)//an emergency flight we already moved
				{
					tmp_flight = tmp_flight->next_flight;//move on
				}
				else if (k < toMoveReg && tmp_flight->cur_flight->is_emergency == FALSE)//still have regular flights to move
				{
					continue_from_here = tmp_flight->next_flight;//next flight after we move tmp
					p_flight copy_flight = createFlight(tmp_flight->cur_flight->flight_num, tmp_flight->cur_flight->flight_type, tmp_flight->cur_flight->dest, tmp_flight->cur_flight->is_emergency);
					if (copy_flight == NULL)
					{
						return FAILURE;//checking we got the copy as needed else return failure
					}
					//take out 'tmp flight' from the list
					removeFlight(tmp_runway->cur_runway, tmp_flight->cur_flight->flight_num);
					//put it ('copy_flight')back in according to the rules
					memCheck = addFlight(tmp_runway->cur_runway, copy_flight);
					if (memCheck == FAILURE) { return FAILURE; }
					destroyFlight(copy_flight);//destroy the copy we made to release the memory
											   // moving the pointers and counters
					tmp_flight = continue_from_here;
					k++;//we moved a desired flight.with 'k' and to 'toMoveReg' we make sure we wont move it again
				}
				else//we moved all the flights we needed just move on to the end
				{
					tmp_flight = tmp_flight->next_flight;
				}
			}
			else//not a desired destination flight
			{
				tmp_flight = tmp_flight->next_flight;
			}
		}
		tmp_runway = tmp_runway->next_runway;//go to the next runway
	}
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : printAirport 
//* Description   : the function prints the airport and all its runways in it 
//* Parameters    : None.
//* Return value  : None.
//************************************************************************************************************

void printAirport()
{
	printf("Airport status:\n");
	runway_elem* tmp_runway = airport;
	while (tmp_runway != NULL)
	{
		printRunway(tmp_runway->cur_runway);
		tmp_runway = tmp_runway->next_runway;
	}
	return;
}

//************************************************************************************************************
//* Function name : destroyAirport 
//* Description   : the function destroys all the runways that are still in the airport and then destroys the airport
//* Parameters    : None.
//* Return value  : None.
//************************************************************************************************************

void destroyAirport()
{
//* if there are no flights in the airport we need just to free the airport
	if (airport == NULL)
	{
		free(airport);
		return;
	}
//* if there are runways in the airport we keep the current runway and the next runway,
//*	destroy each runway with a loop, free the runway_elem and continue to the next runway 
	runway_elem* tmp_runway = airport->next_runway;
	runway_elem* tmp_runway2 = airport->next_runway;
	while (tmp_runway != NULL)
	{
		destroyRunway(tmp_runway->cur_runway);
		tmp_runway2 = tmp_runway->next_runway;
		free(tmp_runway);
		tmp_runway = tmp_runway2;
	}
//* the loop destroy all the runways except the first runway, so when the loop ends,
//*	we need to destroy the first runway which the airport points to, and then free the airport
	destroyRunway(airport->cur_runway);
	free(airport);
	return;
}

