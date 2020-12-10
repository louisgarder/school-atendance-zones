#ifndef BLOCKGROUP_H
#define BLOCKGROUP_H
#include <string>
using namespace std;

struct BlockGroup {
    string GEO_ID; //Geo_ID of the block group
    string name; //Other information about the block group, including the census tract number, block group number, and county name
    int total; // Total population of the block group
    int white; //Total white population of the block group
    int black; //Total black or African American population of the block group

//    int native;
//    int asian;
//    int pacific;
//    int other;
// Other race/ethnicity demographics that can be implemented

    double latitutde; //Latitude of the block group
    double longitude;  //Longitude of the block group
    double diversityScore; //how well the block group reflects diversity of overall region. Closer to 0 is better.
};



#endif // BLOCKGROUP_H
