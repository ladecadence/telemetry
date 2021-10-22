#include "base40.h"

uint32_t encode_mission_id(char* mission_id)
{
	uint32_t x;
	char* c;
	
	/* point c at the end of the mission_id, maximum of 6 characters */
	for(x = 0, c = mission_id; x < MAX_MISSION_ID && *c; x++, c++);
	
	/* encode it backwards */
	x = 0;
	for(c--; c >= mission_id; c--)
	{
		x *= 40;
		if(*c >= 'A' && *c <= 'Z') x += *c - 'A' + 14;
		else if(*c >= 'a' && *c <= 'z') x += *c - 'a' + 14;
		else if(*c >= '0' && *c <= '9') x += *c - '0' + 1;
	}
	
	return(x);
}

char* decode_mission_id(char* mission_id, uint32_t code)
{
	char* c;
    char s;
	
	*mission_id = '\0';
	
	/* is mission_id valid? */
	if(code > 0xF423FFFF) return(mission_id);
	
	for(c = mission_id; code; c++)
	{
		s = code % 40;
		if(s == 0) *c = '-';
		else if(s < 11) *c = '0' + s - 1;
		else if(s < 14) *c = '-';
		else *c = 'A' + s - 14;
		code /= 40;
	}
	*c = '\0';
	
	return(mission_id);
}

