#include "SchoolAttendanceZone.h"


/*
 * Function that calculate the demographic populations
 * for the SchoolAttendanceZone by going through
 * each block group in the SchoolAttendanceZone.
 */
void SchoolAttendanceZone::findPopulations(){

    total = 0;
    white = 0;
    black = 0;

    for(int x = 0; x < (int) blockGroups.size(); x++){
        total += blockGroups[x].total;
        white += blockGroups[x].white;
        black += blockGroups[x].black;
    }
}
