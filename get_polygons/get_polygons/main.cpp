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
    string line;
    size_t pos;

    
    infile_name.str("");
    infile_name << "/Users/michele/Documents/sight_reduction_program/Contents/Resources/Data/land_polygons/osm_land_polygons_simplifyGeom_0point005_areaGT1e6_aggregated.kml";
    infile.open(infile_name.str().c_str());
    
    if(!infile){
        cout << "\nError opening file " << infile_name.str().c_str()  << ".";
        return 0;
    }
    
    while(!infile.eof()){
        
        //read the file until </coordinates> is found
        do{
            getline(infile, line);
        }while(line.find("<coordinates>") == std::string::npos);
        getline(infile, line);
        line = line.substr(7, line.size());
        
        pos = 0;
        pos = line.find("\t");

        cout << line << endl;
        
        
    }
    

    infile.close();
    
    return 0;
    
}
