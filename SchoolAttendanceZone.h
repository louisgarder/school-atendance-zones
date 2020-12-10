#ifndef SCHOOLATTENDANCEZONE_H
#define SCHOOLATTENDANCEZONE_H

#include "BlockGroup.h"
#include <vector>
using namespace std;


/*
 * Type representing a school attendance zone,
 * which is a subset of the overall region. Each student
 * in the region is assigned to 1 school attendance zone,
 * and this determines where they will go to school. For this
 * program, a school attendance zone is made up of
 * census block groups, represented by a vector of BlockGroup,
 * as well as a center blockGroup that is an element of this vector
 * and is used to determine the "center" of the school attendance zone.
 */
struct SchoolAttendanceZone{
    BlockGroup centerBlockGroup; //"Center" of the school attendance zone
    vector<BlockGroup> blockGroups; //Block groups that are in the school attendance zone
    int total; //Total population of the school attendance zone
    int white; //Total white population of the school attendance zone
    int black; //Total black or African American population of the school attendance zone

//    int hispanic;
//    int native;
//    int asian;
//    int pacific;
// For possible implementations of other race/ethnicity demographics

    void findPopulations();
};

#endif // SCHOOLATTENDANCEZONE_H
