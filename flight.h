#ifndef _flight_h
#define _flight_h
typedef struct flight
{
	int flight_num;
	FlightType flight_type;
	char* dest;
	BOOL is_emergency;
} flight;
typedef struct flight* p_flight;
p_flight createFlight(int flight_num, FlightType flight_type, char* dest, BOOL is_emergency);
void destroyFlight(p_flight flight);
void printFlight(p_flight flight);
#endif


