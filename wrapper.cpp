#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <list>
#include <stdio.h>

//#include <boost/algorithm/string.hpp>
//#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll.hpp>

/*
 compile on mac with
 
 g++ wrapper.cpp -o wrapper.o -I/usr/local/include/  -Wall -Wno-c++11-extensions --std=c++17  -O3 -L/usr/local/bin  -lboost_filesystem -lboost_system

 */


using namespace std;



int main(int argc, const char * argv[]) {
    
    ifstream infile;
    ofstream outfile;
    string run_directory;
    stringstream library_directory, command, ins;
    
    cout << "Program location = " << boost::dll::program_location().parent_path() << "\n";
    
    ins.str("");
    ins << (boost::dll::program_location().parent_path());
    run_directory = ins.str();
    
    //remove " from run_directory
    run_directory.erase(std::remove(run_directory.begin(), run_directory.end(), '"'), run_directory.end());

    cout << "Run directory = " << run_directory << "\n";
    
    ins.str("");
    ins << run_directory << "/";
    
    run_directory.clear();
    run_directory = ins.str().c_str();
    
//    cout << "File = " << ins.str().c_str() << "\n";
//    outfile.open(ins.str().c_str());
//    outfile << "run directory = " << run_directory << "/";
//    outfile.close();

    library_directory.str("");
    library_directory << "\"" << run_directory << "../Resources/Libraries/\"";
    
    cout << "Library directory = " << library_directory.str().c_str() << "\n";
    
    //export the libraries so the app knows where to find them later
    command.str("");
    command << "export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:"
    << library_directory.str().c_str()
    << "; echo \$DYLD_LIBRARY_PATH"
    << "; " << run_directory << "/./main.o";
    
    cout << "Command  = " << command.str().c_str() << "\n";

    system(command.str().c_str());

    
    return 0;
    
}
