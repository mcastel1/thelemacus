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

/*
 compile on mac with
 
 g++ wrapper.cpp -o wrapper.o -Wall -Wno-c++11-extensions --std=c++17  -O3
 
 */


using namespace std;



int main(int argc, const char * argv[]) {
    
    // insert code here...
    unsigned int i;
    ifstream infile;
//    ofstream outfile;
    stringstream run_directory, library_directory, command, ins;
//    string line, dummy, result;
//    size_t position;
//
    
    
//    system("rm -rf path.dat;");
//
//    command.str("");
//    //get the path where the executable is running with ps aux command and write the result fo File outfile
//    command << "ps aux | grep wrapper >> path.dat";
//    system(command.str().c_str());
//
//    //read the result from outfile and write it in run_directory
//    infile.open("path.dat");
//
//    //given that ps aux may yield an output with multiple lines, I pick the line rleated to the app by selecting the line that contains "Thelemacus.app"
//    do{
//
//        line.clear();
//        getline(infile, line);
//
//    }while((line.find("wrapper")) == (string::npos));
//
//    infile.close();
//
//
//    //    getline((outfile.value), line);
//
//
//    ins << line;
//    //fetch the last column in the output of ps aux, where the path is located
//    for(i=0; i<10; i++){
//
//        dummy.clear();
//        ins >> dummy;
//
//    }
//
//    //I got to the last column, which constains the path. Because it may contain spaces, I put all of its words in dummy until the end of the column (ins) is reached
//    dummy.clear();
//    ins.clear();
//    //    ins.appendto(String("'"));
//    do{
//        ins >> dummy;
//        ins << dummy;
//        if(ins.tellg() != -1){ins << "\ ";}
//    }while(ins.tellg() != -1);
//
//    //get the part of the path preceeding Contents/MacOS/Thelemacus and write it in ins
//    position = (ins.str()).find("Contents/MacOS/Thelemacus");
//
//    if(position != string::npos){
//        result = ((ins.str()).substr(0, position));
//    }
//
//
//
    
    
    //*****************
    
//    //run pwd and get the current running path of this .o -> write it into path.dat so the app can read it later
//    infile.open("path.dat");
//    getline(infile, running_directory);
//    //    system("rm -rf path.dat");
//
//    cout << "Running directory = " << running_directory << "\n";
//
    library_directory.clear();
    library_directory << run_directory.str() << "/../Resources/Libraries/";
    
    cout << "Library directory = " << library_directory.str().c_str() << "\n";
    
    //export the libraries so the app knows where to find them later
    command.clear();
    command << "export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:"
    << library_directory.str().c_str()
    << "; echo \$DYLD_LIBRARY_PATH"
    << "; ./main.o";
    
    cout << "Command  = " << command.str().c_str() << "\n";
    
    
    
    system(command.str().c_str());
    

    
    return 0;
    
}
