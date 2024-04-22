//
//  main.cpp
//  get_polygons
//
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


using namespace std;

int main(int argc, const char * argv[]) {
    
    ifstream infile;
    stringstream infile_name;
    string line, t;
    size_t pos_start, pos_end;
    vector<double> lambda;
    vector<double> phi;

    
    infile_name.str("");
    infile_name << "/Users/michele/Documents/sight_reduction_program/Contents/Resources/Data/land_polygons/osm_land_polygons_simplifyGeom_0point005_areaGT1e6_aggregated.kml";
    infile.open(infile_name.str().c_str());
    
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
        }while(line.find("<coordinates>") == std::string::npos);
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


        cout << line << endl;
        
        
    }
    

    infile.close();
    
    return 0;
    
}
