//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex2.h"
#include "flight.h"


//************************************************************************************************************
//* Function name : is_valid_dest
//* Description   : the function checks if the destination of a givem flight has 3 big letters
//* Parameters    : dest - pointer to char with the destination that need to check if it is llegal
//* Return value  : the function returns TRUE if the destination is llegal and FALSE if don't
//************************************************************************************************************

BOOL is_valid_dest(char* dest)
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
//* Function name : createFlight
//* Description   : the function creates pointer to struct from type flight and insert all 
//*                 the parameters to this struct
//* Parameters    : flight_num - integer with the number of the flight that need to create
//*                 flight_type - variable of type FlightType with the type of the flight that need to create
//*                 dest - pointer to char with the destination that need ot create
//*                 is_emergency - variable of type BOOL tells whether the flight is regular or emergency
//* Return value  : the function returns p_flight, pointer to flight with all the parameters that were given
//************************************************************************************************************

p_flight createFlight(int flight_num, FlightType flight_type, char* dest, BOOL is_emergency)
{
	p_flight cur_flight;
//* check if all the inputs to this function are valid
	int is_dest = is_valid_dest(dest);
	if (flight_num > MAX_ID || flight_num <= 0 || (flight_type != 0 && flight_type != 1)
			|| (is_emergency != 0 && is_emergency != 1) || !is_dest)
	{
		return NULL;
	}
//* call malloc, check if malloc succeed, initialize the new struct
	else
	{
		cur_flight = (p_flight)malloc(sizeof(flight));
		if (cur_flight == NULL)
		{
			return NULL;
		}
		cur_flight->flight_num = flight_num;
		cur_flight->flight_type = flight_type;
		cur_flight->dest = (char*)malloc(sizeof(char)*4);
		if (cur_flight->dest == NULL) { return NULL;}
		strcpy(cur_flight->dest, dest);
		cur_flight->is_emergency = is_emergency;
		return cur_flight;
	}
}

//************************************************************************************************************
//* Function name : destroyFlight
//* Description   : the function free the struct of type pointer to flight that is given as parameter
//* Parameters    : flight - pointer to flight, the function free this struct
//* Return value  : None.
//************************************************************************************************************

void destroyFlight(p_flight flight)
{
	if (flight == NULL)
	{
		return;
	}
	free(flight->dest);
	free(flight);
	return;
}

//************************************************************************************************************
//* Function name : printFlight
//* Description   : the function prints the struct of type pointer to flight that is given as parameter
//* Parameters    : flight - pointer to flight, the function free this struct
//* Return value  : None.
//************************************************************************************************************

void printFlight(p_flight flight)
{
	if (flight == NULL)
		return;
//* according to the input create 2 variables for the print
	char f_type;
	char f_emer;
	if (flight->flight_type == DOMESTIC)
	{
		f_type = 'D';
	}
	else
	{
		f_type = 'I';
	}
	if (flight->is_emergency)
	{
		f_emer = 'E';
	}
	else
	{
		f_emer = 'R';
	}
//* print the line
	printf("Flight %d %c %s %c\n", flight->flight_num, f_type, flight->dest, f_emer);
}

