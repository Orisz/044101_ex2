//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex2.h"
#include "flight.h"
#include "runway.h"
#include "airport.h"

#define MAX_LINE_SIZE 255

int main()
{
//* define variables, line to get the line that is inserted, delimiter to cut the line into fields
	char line[MAX_LINE_SIZE];
	char* delimiter = " \t\n";
//* one variable for the command and and 4 parametes for the other input parameters
	char* command;
	char* param0;
	char* param1;
	char* param2;
	char* param3;
//* the while loop end when there is no new lines 
	while (fgets(line, MAX_LINE_SIZE, stdin))
	{
//* first, for each line we break the line with the delimiters to the command and four parameters
		command = strtok(line, delimiter);
		param0 = strtok(NULL, delimiter);
		param1 = strtok(NULL, delimiter);
		param2 = strtok(NULL, delimiter);
		param3 = strtok(NULL, delimiter);
//* second, we check the command name, if insert we need to have 2 parameters that are not null
		if (!strcmp(command, "Insert"))
		{
			if (param0 == NULL || param1 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
//* if we got here we have the parameters, and according to the second parameter we call addRunway
			else
			{
				if (!strcmp(param1, "D"))
				{
					if (!addRunway(atoi(param0), DOMESTIC))
					{
						fprintf(stderr, "%s execution failed.\n", command);
					}
				}
				else if (!strcmp(param1, "I"))
				{
					if (!addRunway(atoi(param0), INTERNATIONAL))
					{
						fprintf(stderr, "%s execution failed.\n", command);
					}
				}
//* if the second parameter is not good we do the else
				else
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* remove needs only one parameter, so we check it's not null and call removeRunway 
		else if (!strcmp(command, "Remove"))
		{
			if (param0 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
			else
			{
				if (!removeRunway(atoi(param0)))
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* add needs all 4 parameter, and according to the param1 and param3 we call addFlightToAirport with the parameters
		else if (!strcmp(command, "Add"))
		{
			char* dest = param2;
			if (param0 == NULL || param1 == NULL || param2 == NULL || param3 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
			else
			{
				if (!strcmp(param1, "D"))
				{
					if (!strcmp(param3, "R"))
					{
						if (!addFlightToAirport(atoi(param0), DOMESTIC, dest, FALSE))
						{
							fprintf(stderr, "%s execution failed.\n", command);
						}
					}
					else if (!strcmp(param3, "E"))
					{
						if (!addFlightToAirport(atoi(param0), DOMESTIC, dest, TRUE))
						{
							fprintf(stderr, "%s execution failed.\n", command);
						}
					}
					else
					{
						fprintf(stderr, "%s execution failed.\n", command);
					}
				}
				else if (!strcmp(param1, "I"))
				{
					if (!strcmp(param3, "R"))
					{
						if (!addFlightToAirport(atoi(param0), INTERNATIONAL, dest, FALSE))
						{
							fprintf(stderr, "%s execution failed.\n", command);
						}
					}
					else if (!strcmp(param3, "E"))
					{
						if (!addFlightToAirport(atoi(param0), INTERNATIONAL, dest, TRUE))
						{
							fprintf(stderr, "%s execution failed.\n", command);
						}
					}
					else
					{
						fprintf(stderr, "%s execution failed.\n", command);
					}
				}
				else
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* depart needs 2 parameters, if the return from atoi is not zero we call depart
		else if (!strcmp(command, "Depart"))
		{
			if (param0 == NULL || param1 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
			else
			{
				int num_flights = atoi(param1);
				if (num_flights != 0)
				{
					if (!departFromRunway(atoi(param0), num_flights))
					{
						fprintf(stderr, "%s execution failed.\n", command);
					}
				}
//* if atoi return 0 it means that the number of flights to remove is 0 or that the input wasn't a number
				else if (!strcmp(param1, "0"))
				//zero flights to depart
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
				else
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* change needs 2 parameters, and we just call changeDest with those
		else if (!strcmp(command, "Change"))
		{
			if (param0 == NULL || param1 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
			else
			{
				if (!changeDest(param0, param1))
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* delay needs 1 parameter, and we just call delay with it
		else if (!strcmp(command, "Delay"))
		{
			if (param0 == NULL)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", command);
			}
			else
			{
				if (!delay(param0))
				{
					fprintf(stderr, "%s execution failed.\n", command);
				}
			}
		}
//* print does not need parameter, so we just call printAirport
		else if (!strcmp(command, "Print"))
		{
			printAirport();
			printf("\n");
		}
//* exit does not need parameter, so we just call destroy Airport and then return from the main
		else if (!strcmp(command, "Exit"))
		{
			destroyAirport();
			return 0;
		}
//* if command is not one of this options we print the error
		else
		{
			fprintf(stderr, "Command not found.\n");
		}
	}
//* if we got here it means that there was no Exit as input, and we need to destroyAirport and return from main
	destroyAirport();
	return 0;
}
