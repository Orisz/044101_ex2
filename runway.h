#ifndef _runway_h
#define _runway_h

typedef struct flight_next
{
	p_flight cur_flight;
	struct flight_next* next_flight;
} flight_next;
//typedef struct flight_next* p_flight_next;
typedef struct runway
{
	int runway_num;
	FlightType runway_type;
	flight_next* first_flight;
} runway;
typedef struct runway* p_runway;
p_runway createRunway(int runway_num, FlightType runway_type);
void destroyRunway(p_runway runway);
BOOL isFlightExists(p_runway runway, int flight_num);
int getFlightNum(p_runway runway);
Result addFlight(p_runway runway, p_flight flight);
Result removeFlight(p_runway runway, int flight_num);
Result depart(p_runway runway);
void printRunway(p_runway runway);
#endif
