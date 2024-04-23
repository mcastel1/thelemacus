//
//  main.cpp
//  get_polygons
/* this code reads the input file sm_land_polygons_simplifyGeom_0point005_areaGT1e6_aggregated.kml and extracts from it, for each polygon, the latitude and longitude of each point in the polygon, and writes the lat and long to the outfile polygon.dat in a clearly readable format  -> polygons.dat will then be read by Thelemacus
 
The format of the output is
[number of points in polygon]: [longitude of first point] [latitude of first point] [longitude of second point] [latitude of second point] ...
[number of points in polygon]: [longitude of first point] [latitude of first point] [longitude of second point] [latitude of second point] ...
...
 
 where latitude and longitude are expressed in radians and the longitude is positive in the West direction
 */
//  Created by Michele on 19/04/2024.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <strstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <list>
#include <stdio.h>

#define k (2.0*M_PI/360.0)


using namespace std;

int main(int argc, const char * argv[]) {
    
    ifstream infile;
    ofstream outfile;
    stringstream infile_name, outfile_name;
    string line, t;
    size_t pos_start, pos_end;
    vector<double> lambda;
    vector<double> phi;

    
    infile_name.str("");
    infile_name << "/Users/michele/Documents/sight_reduction_program/Contents/Resources/Data/land_polygons/osm_land_polygons_simplifyGeom_0point005_areaGT1e6_aggregated.kml";
    infile.open(infile_name.str().c_str());  
    infile.precision(16);
    
    outfile_name.str("");
    outfile_name << "/Users/michele/Documents/sight_reduction_program/Contents/Resources/Data/land_polygons/polygons.dat";
    outfile.open(outfile_name.str().c_str());
    outfile.precision(16);
    
    if(!infile){
        cout << "\nError opening file " << infile_name.str().c_str()  << ".";
        return 0;
    }
    
    while(!infile.eof()){
        //I am focusing on a given polygon
        
        lambda.clear();
        phi.clear();
        
        //read the file until </coordinates> is found
        do{
            getline(infile, line);
        }while((line.find("<coordinates>") == std::string::npos) && (!infile.eof()));
        
        if(!infile.eof()){
            
            getline(infile, line);
            line = line.substr(7, line.size());
            
            pos_start = 0;
            do{
                //go through the points of the polygon
                
                //read the longitude
                pos_end = line.find(",", pos_start);
                t = line.substr(pos_start, pos_end-pos_start);
                lambda.push_back(stod(t));
                
                //read the latitude
                pos_start = pos_end + 1;
                pos_end = line.find(",", pos_start);
                t = line.substr(pos_start, pos_end-pos_start);
                phi.push_back(stod(t));
                
                pos_start = line.find(" ", pos_start) +1;
                
            }while(line.find(",", pos_start) !=  string::npos);
            
            outfile << lambda.size() << ":";
            for(unsigned long long int i=0; i<lambda.size(); ++i){
                outfile << " " << -k*lambda[i] << " " << k*phi[i];
            }
            outfile << endl;
            
            
            cout << line << endl;
            
        }
        
        
    }
    

    infile.close();
    outfile.close();
    
    return 0;
    
}
