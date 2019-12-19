#ifndef _airport_h
#define _airport_h

typedef struct runway_elem
{
	p_runway cur_runway;
	struct runway_elem* next_runway;
} runway_elem;


Result addRunway(int runway_num, FlightType runway_type);
Result removeRunway(int runway_num);
Result addFlightToAirport(int flight_num, FlightType flight_type, char* dest, BOOL is_emergency);
Result departFromRunway(int runway_num, int num_flights);
Result changeDest(char* origin_dest, char* new_dest);
Result delay(char* dest);
void printAirport();
void destroyAirport();
#endif
