#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "BlockGroup.h"
#include "SchoolAttendanceZone.h"
using namespace std;

void intro();
BlockGroup* createBlockGroups(string nameOfRegion, int &numOfBlockGroups);
void calculateDiversityScores(BlockGroup* region, int numOfBlockGroups);
void findMostDiverseBlocks(BlockGroup* region, int numOfZones, int numOfBlockGroups);
vector<vector<SchoolAttendanceZone>> createPossibleConfigurations(BlockGroup* region, int numofZones, int numOfBlockGroups, int numOfSimulations);
void removeBadConfigurations(vector<vector<SchoolAttendanceZone>> &possibleConfigurations, BlockGroup* region, int numOfBlockGroups);
void findTopConfigurations(vector<vector<SchoolAttendanceZone>> &possibleConfigurations, BlockGroup* region, int numOfBlockGroups, int numberOfConfigurations);

int main()
{
    intro();

    cout << "What is the name of the region where the school attendance zones will be created?" << endl;
    string nameOfRegion;
    cin >> nameOfRegion;
    cout << "How many school attendance zones (number of schools) should be created?"<< endl;
    int numOfZones;
    cin >> numOfZones;

    int numOfBlockGroups;
    //creates an array with all of the BlockGroup
    BlockGroup* region = createBlockGroups(nameOfRegion, numOfBlockGroups);

    //establishes how well each block group reflects the demogrpahics of the overall region.
    calculateDiversityScores(region, numOfBlockGroups);

    //puts the top (numOfZones * 10) block groups with the best (lowest) diversity scores at the front of the array 'region'.
    //These (numOfZones * 10) block groups will be locations considered for the centers of the attendance zones.
    //View README file for how the overall algorithm works
    findMostDiverseBlocks(region, numOfZones, numOfBlockGroups);

    cout << endl << "How many different configurations of attendance zones would you like to receive?" << endl;
    int numberOfConfigurations;
    cin >> numberOfConfigurations;

    cout << endl << "How many simulations should be run to find the best " << numberOfConfigurations << " configurations?" << endl;
    cout << "The more simulations that are run, the more likely a better configuration will be found. However, more simulations requires more runtime." << endl;
    int numberOfSimulations;
    cin >> numberOfSimulations;

    cout << endl << "Please wait. Simulations are being run and the top " << numberOfConfigurations << " configurations with optimal school attendance zones will be found." << endl << endl;

    //Runs through multiple simulations and creates different configurations of attendance zones
    //View README file for info on how these configurations are created
    vector<vector<SchoolAttendanceZone>> possibleConfigurations = createPossibleConfigurations(region, numOfZones, numOfBlockGroups, numberOfSimulations);

    //Finds the top configurations of attendance zones that satisfy the desired properties:
    //1. Demographics of zones reflect demographics of overall region.
    //2. Comparable population sizes in each zone
    findTopConfigurations(possibleConfigurations, region, numOfBlockGroups, numberOfConfigurations);

    cout << "Top " << numberOfConfigurations << " optimal configurations have been found. What is the output file name to view these configurations?" << endl;
    string fileName;
    cin >> fileName;
    ofstream outFile (fileName, ios::out);

    //Finding demographics of overall region
    int regionTotal = 0;
    int regionWhite = 0;
    int regionBlack = 0;
    for(int x = 0; x < numOfBlockGroups; x++){
        regionTotal += region[x].total;
        regionWhite += region[x].white;
        regionBlack += region[x].black;
    }

    //Printing demographics and configurations to output file.
    outFile << nameOfRegion << " overall demographics" << endl;
    outFile << "Population: " << regionTotal << endl;
    outFile << "White percentage: " << (double) regionWhite / regionTotal * 100 << endl;
    outFile << "Black or African American percentage: " << (double) regionBlack /regionTotal * 100 << endl << endl;

    for(int x = 1; x <= numberOfConfigurations; x++){
        outFile << "Configuration " << x << ":"   << endl;
        for (int y = 1; y <= (int) possibleConfigurations[x-1].size(); y++){
            int totalPopulation = 0;
            int whitePopulation = 0;
            int blackPopulation = 0;
            outFile << "\tSchool Attendance Zone " << y << ":" << endl;
            for (int z = 0; z < (int) possibleConfigurations[x-1][y-1].blockGroups.size(); z++){
                outFile << "\t\t" << possibleConfigurations[x-1][y-1].blockGroups[z].GEO_ID;
                outFile << "\t" << possibleConfigurations[x-1][y-1].blockGroups[z].name << endl;
                totalPopulation += possibleConfigurations[x-1][y-1].blockGroups[z].total;
                whitePopulation += possibleConfigurations[x-1][y-1].blockGroups[z].white;
                blackPopulation += possibleConfigurations[x-1][y-1].blockGroups[z].black;
            }
            outFile << "\t\tPopulation: " << totalPopulation << endl;
            outFile << "\t\tPercent White: " << (double) whitePopulation / totalPopulation * 100 << endl;
            outFile << "\t\tPercent Black/African American: " << (double) blackPopulation / totalPopulation * 100 << endl;
        }
        outFile << endl << endl;
    }

    cout << endl << "Data has been saved to " << fileName  << "." << endl;

    delete[] region;
}


/*
 * Sets up the output console
 */
void intro(){
    cout << endl<< "This program will attempt to create desegregated school attendance zones for a given region using race demographics of the US census block groups in the region. ";
    cout << "Each block group will be assigned to a school in order to create school attendance zones with racial demographics that reflect the racial demographics of the overall region. ";
    cout << "Each school attendance zone will also attempt to contain similar populations and minimize traveling distances for each student. However, these last two criteria are valued substantially less, as a zone with more students can simply contain more schools" << endl;
    cout << endl;
}

/*
 * Reads the input text file that contains the data for the block groups in the required format. View the README file
 * for more info on the required format of this file. Creates an array of BlockGroup that represents all of the blockgroups
 * provided in the file.
 */
BlockGroup* createBlockGroups(string nameOfRegion, int &numOfBlockGroups){

    cout << endl << "Please enter the file that contains the block group data in the required format for " + nameOfRegion + "." << endl;
    string fileName;
    cin >> fileName;

    ifstream in(fileName);
    string str;
    int numOfLines = 0;
    //Counting number of block groups in file
    while (getline(in, str)){
        numOfLines++;
    }
    numOfBlockGroups = numOfLines - 1;
    in.clear();
    in.close();

    //Array of all block groups in the region
    BlockGroup* region = new BlockGroup[numOfLines-1];
    in.open(fileName);

    //Setting values of the the block group variables to the data in the file
    int count = 0;
    while (getline(in, str)){
        if(count != 0){
            int index = str.find("\t");
            region[count-1].GEO_ID = str.substr(0,index);
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].name = str.substr(0,index);
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].total = stoi(str.substr(0,index));
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].white = stoi(str.substr(0,index));
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].black = stoi(str.substr(0,index));
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].latitutde = stod(str.substr(0,index));
            str = str.substr(index + 1);

            index = str.find("\t");
            region[count-1].longitude = stod(str.substr(0,index));

            //To add more race/ethnicity groups to the program, adjust input file data and the
            //initialization of the BlockGroup variables accordingly here
        }
        count++;
    }
    return region;
}

/*
 * Calculates a "Diversity Score" for each block group that represents how
 * well each block group refelcts the demographics of the overall region.
 * For more informaiton on this score, view the README file. This score
 * can also be adjusted to include more race/ethnicty groups by editing this
 * function. .
 */

void calculateDiversityScores(BlockGroup* region, int numOfBlockGroups){

    //Finding racial demographics of the overall region
    int populationOfAll = 0;
    int populationWhite = 0;
    int populationBlack = 0;
    //edit here to add more race/ethnicity groups

    for(int x = 0; x < numOfBlockGroups; x++){
        populationOfAll += region[x].total;
        populationWhite += region[x].white;
        populationBlack += region[x].black;
        //edit here to add more race/ethnicity groups
    }

    //For each block group, the diversity score is based on how similar the
    //racial demogarphics are to the demographics of the overall region.
    //A lower diversity score is better.
    for(int x = 0; x < numOfBlockGroups; x++){
        if(region[x].total == 0){
            region[x].diversityScore = -1;
        }
        else{
            double whiteDifference = abs(((double)populationWhite/populationOfAll) - ((double)region[x].white/region[x].total));
            double blackDifference = abs(((double)populationBlack/populationOfAll) - ((double)region[x].black/region[x].total));
            region[x].diversityScore = whiteDifference * populationWhite + blackDifference * populationBlack;
            //edit here to add more race/ethnicity groups
        }
    }
}

/*
  * Finds the top (numOfZones * 10) block groups with the best (lowest) diversity scores using selection sort.
  * These block groups are then placed at the front of the array 'region'. View the README file for information
  * on why this process is done to optimize the algorithm.
  */
void findMostDiverseBlocks(BlockGroup* region, int numOfZones, int numOfBlockGroups){

    //Selection sort to find top (numOfZones * 10) block groups with the best (lowest) diversity scores
    for(int x = 0; x < numOfZones * 10; x++){
        int lowestDiversityIndex = x;
        for(int y = x; y < numOfBlockGroups; y++){
            if(region[y].diversityScore < region[lowestDiversityIndex].diversityScore && region[y].diversityScore != -1){
                lowestDiversityIndex = y;
            }
        }
        //Block group with next lowest diversity score placed at the front of the unsorted subarray.
        BlockGroup temp = region[x];
        region[x] = region[lowestDiversityIndex];
        region[lowestDiversityIndex] = temp;
    }
}

//Finds distance between two block groups in miles using the Haversine Formula
double distanceBtwnBlocks(BlockGroup &a, BlockGroup &b){
    double latitudeA = a.latitutde;
    double longitudeA = a.longitude;
    double latitudeB = b.latitutde;
    double longitudeB = b.longitude;

    //converting degrees to radians
    latitudeA = latitudeA * (M_PI) / 180;
    longitudeA = longitudeA * (M_PI) / 180;
    latitudeB = latitudeB * (M_PI) / 180;
    longitudeB = longitudeB * (M_PI) / 180;

    //Haversine Formula
    double dlong = longitudeA - longitudeB;
    double dlat = latitudeA - latitudeB;

    double ans = pow(sin(dlat / 2), 2) +
            cos(latitudeA) * cos(latitudeB) * pow(sin(dlong / 2), 2);
    ans = 2 * asin(sqrt(ans));

    //Radius of Earth in
    //miles, R = 3956
    double R = 3956;

    //Final result
    ans = ans * R;
    return ans;
}

//Given an integer vector representing the BlockGroups that the attendance zones will be centered around (center nodes), this function
//creates a configuration of attendance zones for the regions, represented by a vector of SchoolAttendanceZones. Each BlockGroup
//is assigned to the closest center node by distance, and this creates a type of Voronoi diagram. Each resulting region in this Voronoi diagram
//is an attendance zone. View README for more info and the justificaiton behind this algorithm.
vector<SchoolAttendanceZone> createSchoolAttendanceZones(BlockGroup* region, vector<int> &centerBlockGroups, int numOfBlockGroups){

    vector<SchoolAttendanceZone> attendanceZones;

    //Creating the SchoolAttendanceZones. The centerBlockGroup for
    //the SchoolAttendanceZones are initialized based on the
    //indexes in the centerBlockGroups vector.
    for(int x = 0; x < (int) centerBlockGroups.size(); x++){
        SchoolAttendanceZone newZone;
        newZone.centerBlockGroup = region[centerBlockGroups[x]];
        attendanceZones.push_back(newZone);
    }

    //Each BlockGroup is added to its 'closest'
    //schoolZttendanceZone
    for(int x = 0; x < numOfBlockGroups; x++){
        BlockGroup next = region[x];
        int closestZone = 0;
        for(int y = 0; y < (int) attendanceZones.size(); y++){
            if(distanceBtwnBlocks(next, attendanceZones[y].centerBlockGroup) < distanceBtwnBlocks(next, attendanceZones[closestZone].centerBlockGroup)){
                closestZone = y;
            }
        }
        attendanceZones[closestZone].blockGroups.push_back(next);
    }
    return attendanceZones;
}

/*
 * Given a vector of integers, the function removes a specified
 * amount of random elements in this vector. Function assumes that
 * amount <= startList.size().
 */
vector<int> removeNumbers(vector<int> startingList, int amount){
    for(int x = 0; x < amount; x++){
        int randomIndex =  rand() %  startingList.size();
        startingList.erase(startingList.begin() + randomIndex);
    }
    return startingList;
}


/*
 * This function creates a specificed number, provided by the user, of possible conifgurations of school attendance zones,
 * represented by a vector of vectors of SchoolAttendanceZoenes. The conifguration depends on which BlockGroups are chosen as
 * the center BlockGroups (center nodes), or the BlockGroups that the attendance zones will be built around. For more information
 * on which BLockGroups and how these BlockGroups are chosen to be center nodes, view the READMORE file.
 */
vector<vector<SchoolAttendanceZone>> createPossibleConfigurations(BlockGroup* region, int numOfZones, int numOfBlockGroups, int numOfSimulations){

    vector<vector<SchoolAttendanceZone>> possibleConfigurations;

    //Creating vector with integers from 0 to numOfZones*10
    //These are the most diverse block groups in 'region' that
    //were previously identified and will be the locations
    //considered for the centers of the attendance zones.
    vector<int> indexes;
    for(int x = 0; x < numOfZones * 10; x++){
        indexes.push_back(x);
    }

    //creating 'numOfSimulations' different configurations of attendance zones and adding them to possibleConfigurations
    for(int x = 0; x < numOfSimulations; x++){
        //randomly choose 'numOfZones' block groups from the numOfZones*10 most diverse block groups to be the centers nodes
        vector<int> centerBlockGroups = removeNumbers(indexes, (numOfZones * 9));
        possibleConfigurations.push_back(createSchoolAttendanceZones(region, centerBlockGroups, numOfBlockGroups));
    }

    return possibleConfigurations;
}

/*
 * Gives a score for each configuration representing how well the population sizes in the
 * school attendance zones are balanced. The score for a configuration is calculated by
 * dividing the greatest population of an attenance zone by the smallest population
 * of an attendance zone and then dividing this value by 10.
 */
vector<double> findPopulationScores(vector<vector<SchoolAttendanceZone>> &possibleConfigurations){

    //contains score for each configuration
    vector<double> populationScores;

    //loops through all configurations of attendance zones in 'possibleConfigurations'
    for(int x = 0; x < (int) possibleConfigurations.size(); x++){
        //finds the population for each attendance zone in the current configuration
        for(int y = 0; y < (int) possibleConfigurations[x].size(); y++){
            possibleConfigurations[x][y].findPopulations();
        }

        //finds the minimum and maximum population of an attendance zone in the current configuration
        int minPopulation = possibleConfigurations[x][0].total;
        int maxPopulation = possibleConfigurations[x][0].total;
        for(int y = 0; y < (int) possibleConfigurations[x].size(); y++){
            if(possibleConfigurations[x][y].total < minPopulation){
                minPopulation = possibleConfigurations[x][y].total;
            }
            if(possibleConfigurations[x][y].total > maxPopulation){
                maxPopulation = possibleConfigurations[x][y].total;
            }
        }

        populationScores.push_back(((double) maxPopulation / minPopulation) / 10);
    }

    return populationScores;
}

/*
 * Gives a score for each configuration representing how well the racial demographics in the
 * school attendance zones reflect the demographics of the overall region. The score for a configuration
 * is calculated by finding the average of the squared percent differences between the racial demographics of
 * each school attendance zone and the overall region and then dividing this value by 100.
 */
vector<double> findDemographicScores(vector<vector<SchoolAttendanceZone>> &possibleConfigurations, BlockGroup* region, int numOfBlockGroups){

    //racial demographics of entire region
    int populationOfAll = 0;
    int populationWhite = 0;
    int populationBlack = 0;

    for(int x = 0; x < numOfBlockGroups; x++){
        populationOfAll += region[x].total;
        populationWhite += region[x].white;
        populationBlack += region[x].black;
    }

    //contains score for each configuration
    vector<double> demographicScores;

    //loops through all configurations of attendance zones in 'possibleConfigurations'
    for(int x = 0; x < (int) possibleConfigurations.size(); x++){
        double sum = 0;
        for(int y = 0; y < (int) possibleConfigurations[x].size(); y++){
            double whitePercentDifference = 100 * abs(((double)possibleConfigurations[x][y].white/possibleConfigurations[x][y].total
                                                       - (double)populationWhite/populationOfAll)
                                                      / (((double) populationWhite/populationOfAll
                                                          + (double) possibleConfigurations[x][y].white/possibleConfigurations[x][y].total)/2));
            double blackPercentDifference = 100 * abs(((double)possibleConfigurations[x][y].black/possibleConfigurations[x][y].total
                                                       - (double)populationBlack/populationOfAll)
                                                      / (((double) populationBlack/populationOfAll
                                                          + (double) possibleConfigurations[x][y].black/possibleConfigurations[x][y].total)/2));
            sum = pow(((whitePercentDifference + blackPercentDifference) / 2), 2) + sum;
        }
        sum = sum / possibleConfigurations[x].size(); //average of squared percent differences
        demographicScores.push_back(sum / 100);
    }
    return demographicScores;
}

/*
 * After creating the possible configurations, the function goes through each of these configurations and
 * finds the top configurations that have attendance zones that are both reflective of the demographics of the region and have
 * comparable populations to each other. These top conifgurations are placed at the front of the vector 'possibleConfigurations'.
 */
void findTopConfigurations(vector<vector<SchoolAttendanceZone>> &possibleConfigurations, BlockGroup* region, int numOfBlockGroups, int numberOfConfigurations){



    vector<double> populationScores = findPopulationScores(possibleConfigurations);
    vector<double> demographicScores =  findDemographicScores(possibleConfigurations, region, numOfBlockGroups);

     //Score for a configuration is equal to its population score plus its demographic score.
    vector<double> overallScores;
    for(int x = 0; x < (int) populationScores.size(); x++){
        overallScores.push_back(populationScores[x] + demographicScores[x]);
    }

    //selection sort to find top configurations with the lowest (best) overall scores. These configurations will
    //be placed at the front of 'possibleConfigurations'
    for(int x = 0; x < numberOfConfigurations; x++){
        int lowestScoreIndex = x;
        for(int y = x; y < (int) possibleConfigurations.size(); y++){
            if(overallScores[y] < overallScores[lowestScoreIndex]){
                lowestScoreIndex = y;
            }
        }
        vector<SchoolAttendanceZone> temp1 = possibleConfigurations[x];
        possibleConfigurations[x] = possibleConfigurations[lowestScoreIndex];
        possibleConfigurations[lowestScoreIndex] = temp1;

        double temp2 = overallScores[x];
        overallScores[x] = overallScores[lowestScoreIndex];
        overallScores[lowestScoreIndex] = temp2;
    }
}



