#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex2.h"
#include "flight.h"
#include "runway.h"

//************************************************************************************************************
//* Function name : createRunway
//* Description   : the function creates the struct of type pointer to runway with all the given parameters
//* Parameters    : runway_num - the number of the new runway
//*				    runway_type - the type of the new runway
//* Return value  : the function returns p_runway, pointer to runway with all the parameters that were given
//************************************************************************************************************

p_runway createRunway(int runway_num, FlightType runway_type)
{
	p_runway cur_runway;
// check if all the inputs are valid
	if (runway_num > MAX_ID || runway_num <= 0 || (runway_type != DOMESTIC && runway_type != INTERNATIONAL))
	{
		return NULL;
	}
//call to malloc, check if malloc succeed, initialize the new struct
	cur_runway = (p_runway)malloc(sizeof(runway));
	if (cur_runway == NULL)
	{
		return NULL;
	}
	cur_runway->runway_num = runway_num;
	cur_runway->runway_type = runway_type;
	cur_runway->first_flight = NULL;
	return cur_runway;
}

//************************************************************************************************************
//* Function name : destroyRunway
//* Description   : the function destroys the runway that is given and all the flights in it
//* Parameters    : runway - the runway to destroy
//* Return value  : None.
//************************************************************************************************************

void destroyRunway(p_runway runway)
{
	if (runway == NULL)
	{
		return;
	}
//* if the runway have no flights, destroy only the runway struct
	if (runway->first_flight == NULL)
	{
		free(runway);
		return;
	}
//* if there are flights in the runway, in the loop we destroy each flight with destroyFlight, and at the end free the runway
	flight_next* tmp = runway->first_flight;
	while (tmp != NULL)
	{
		
		tmp = tmp->next_flight;
		destroyFlight(runway->first_flight->cur_flight);
		free(runway->first_flight);
		runway->first_flight = tmp;
//		printf("delete flight from %d\n", runway->runway_num);
	}
	free(runway);
	return;
}

//************************************************************************************************************
//* Function name : isFlightExists
//* Description   : the function function check if in this runway there is flight with this number
//* Parameters    : runway - the runway to check if there is the flight in it
//*				    flight_num - the number of the flight to found
//* Return value  : BOOL - if there is the flight_num return TRUE all other cases return FALSE
//************************************************************************************************************

BOOL isFlightExists(p_runway runway, int flight_num)
{
//* check if inputs are valid, flight number and if there is any flight in the runway
	if (runway == NULL || runway->first_flight == NULL || runway->first_flight->cur_flight == NULL || flight_num <= 0 || flight_num > MAX_ID)
	{
		return FALSE;
	}
//* the loop checks if in this runway there is flight with this number if yes return true, if the loop finish return false
	flight_next* tmp = runway->first_flight;
	while (tmp != NULL)
	{
		if (tmp->cur_flight->flight_num == flight_num)
		{
			return TRUE;
		}
		else
		{
			tmp = tmp->next_flight;
		}
	}
	return FALSE;
}

//************************************************************************************************************
//* Function name : getFlightNum
//* Description   : the function counts the number of flights in the given runway
//* Parameters    : runway - the runway to count the number of flights in it
//* Return value  : int - the number of flights in the runway
//************************************************************************************************************

int getFlightNum(p_runway runway)
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
		counter++;
		tmp = tmp->next_flight;
	}
	return counter;
}

//************************************************************************************************************
//* Function name : addFlight
//* Description   : the function adds a flight with the parameters given in the flight pointer to the runway 
//* Parameters    : runway - the runway to add the flight
//*					flight - pointer to flight with all the parameters to add the new flight
//* Return value  : Result - if the insert succeeded return SUCCESS, else return FAILURE
//************************************************************************************************************

Result addFlight(p_runway runway, p_flight flight)
{
//* check valid input
	if (runway == NULL || flight == NULL || flight->flight_type != runway->runway_type || isFlightExists(runway, flight->flight_num))
	{
		return FAILURE;
	}
//* create two new types flight and flight_next link them together 
	p_flight new_flight = createFlight(flight->flight_num, flight->flight_type, flight->dest, flight->is_emergency);
	if (new_flight == NULL)
	{
		return FAILURE;
	}
	flight_next* new_flight_next = (flight_next*)malloc(sizeof(flight_next));
	if (new_flight_next == NULL)
	{
		destroyFlight(new_flight);
		return FAILURE;
	}
	new_flight_next->cur_flight = new_flight;
	new_flight_next->next_flight = NULL;
//* first case - empty runway, all we need to do is to take the flight and it in runway at first_flight
	if (runway->first_flight == NULL)
	{
		runway->first_flight = new_flight_next;
		return SUCCESS;
	}
	flight_next* tmp = runway->first_flight;
//* second case - the runway is not empty and the input flight is regular, all we need to do is to put the flight at the end of the runway
	if (new_flight->is_emergency == FALSE)
	{
		while (tmp != NULL)
		{
			if (tmp->next_flight == NULL)
			{
				tmp->next_flight = new_flight_next;
				return SUCCESS;
			}
			tmp = tmp->next_flight;
		}
	}
//* third case - the runway is not empty input flight is emergency and the first flight is not emergency
	if (runway->first_flight->cur_flight->is_emergency == FALSE)
	{
		new_flight_next->next_flight = runway->first_flight;
		runway->first_flight = new_flight_next;
		return SUCCESS;
	}
//* fourth case - the runway is not empty input flight is emergency the first flight is also emergency, separate to two cases:
//* 1. all the flights in runway are emergency - in this case we'll put the new flight at the end
//* 2. in the runway we have both emregency and regular flights and we need to find the last emergency flight
	if (runway->first_flight->cur_flight->is_emergency == TRUE)
	{
		while (tmp != NULL)
		{
			if (tmp->cur_flight->is_emergency == TRUE && tmp->next_flight == NULL)
			{
				tmp->next_flight = new_flight_next;
				return SUCCESS;
			}
			if (tmp->cur_flight->is_emergency == TRUE && tmp->next_flight->cur_flight->is_emergency == FALSE)
			{
				flight_next* first_regular = tmp->next_flight;
				new_flight_next->next_flight = first_regular;
				tmp->next_flight = new_flight_next;
				return SUCCESS;
			}
			tmp = tmp->next_flight;
		}
	}
	return FAILURE;
}

//************************************************************************************************************
//* Function name : removeFlight
//* Description   : the function removes the flight with the flight number that is given, if the flight exists
//* Parameters    : runway - the runway to remove the flight
//*					flight_num- the number of the flight to remove
//* Return value  : Result - if the remove succeeded return SUCCESS, else return FAILURE
//************************************************************************************************************

Result removeFlight(p_runway runway, int flight_num)
{
//* check if the inputs are valid or not NULL
	if (runway == NULL || runway->first_flight == NULL || flight_num > MAX_ID || flight_num <= 0)
	{
		return FAILURE;
	}
//*first the case that the flight is the first flight in the runway
	flight_next* tmp = runway->first_flight;
	if (tmp->cur_flight->flight_num == flight_num)
	{
		runway->first_flight = tmp->next_flight;
		destroyFlight(tmp->cur_flight);
		free(tmp);
		return SUCCESS;
	}
//* loop that checks if the flight_num is exists in this runway, if yes remove the flight
	while (tmp->next_flight != NULL)
	{
		if (tmp->next_flight->cur_flight->flight_num == flight_num)
		{
			flight_next* delete_flight = tmp->next_flight;
			tmp->next_flight = delete_flight->next_flight;
			destroyFlight(delete_flight->cur_flight);
			free(delete_flight);
			return SUCCESS;
		}
		tmp = tmp->next_flight;
	}
//* if the loop ends, it means that no flight with this number at the runway
	return FAILURE;
}

//************************************************************************************************************
//* Function name : depart
//* Description   : the function remove the first flight from the runway, and free all the structs
//* Parameters    : runway - the runway to depart a flight from
//* Return value  : Result - if the depart succeeded return SUCCESS else return FAILURE
//************************************************************************************************************

Result depart(p_runway runway)
{
	if (runway == NULL || runway->first_flight == NULL)
	{
		return FAILURE;
	}
	flight_next* tmp = runway->first_flight;
	runway->first_flight = runway->first_flight->next_flight;
	destroyFlight(tmp->cur_flight);
	free(tmp);
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : printRunway
//* Description   : the function prints the struct of type pointer to runway that is given as parameter and all his flights
//* Parameters    : runway - the runway to print
//* Return value  : None.
//************************************************************************************************************

void printRunway(p_runway runway)
{
	if (runway == NULL)
	{
		return;
	}
	char* type = "";
//* according to the input create variable for the print
	if (runway->runway_type == INTERNATIONAL)
	{
		type = "international";
	}
	if (runway->runway_type == DOMESTIC)
	{
		type = "domestic";
	}
	int num_flights = getFlightNum(runway);
//* print the two lines of the runway
	printf("Runway %d %s\n%d flights are waiting:\n", runway->runway_num, type, num_flights);
//* loop to print all the flights in the runway
	flight_next* tmp = runway->first_flight;
	while(tmp != NULL)
	{
		printFlight(tmp->cur_flight);
		tmp = tmp->next_flight;
	}
	return;
}

