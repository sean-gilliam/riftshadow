#ifndef ENTITY_TIME_INFO_DATA_H
#define ENTITY_TIME_INFO_DATA_H

#include "fwd.h"

//
// The in-game world clock (hour/day/month/season/year). Plain POD value type;
// held by value both by the global `time_info` and by each room track.
//
struct time_info_data
{
	bool half;
	int hour;
	int day;
	int month;
	int season;
	int year;
};

#endif /* ENTITY_TIME_INFO_DATA_H */
