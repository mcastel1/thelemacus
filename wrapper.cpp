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
    stringstream library_directory, command, ins;
    string line, dummy;
    size_t position;
    File temp;
    String new_prefix, run_directory;
    
    
    //*****************
    //writes into this->run_directory the path where the executable is currently running
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    temp.set_name(String("output.dat"));
    temp.remove(String(""));
    
    command.str("");
    //get the path where the executable is running with ps aux command and write the result fo File temp
    command << "ps aux | grep wrapper >> " << ((temp.name).value);
    system(command.str().c_str());
    
    //read the result from temp and write it in run_directory
    temp.open(String("in"), new_prefix);
    
    //given that ps aux may yield an output with multiple lines, I pick the line rleated to the app by selecting the line that contains "Thelemacus.app"
    do{
        
        line.clear();
        getline(temp.value, line);
        
    }while((line.find("wrapper")) == (string::npos));
    
    //    getline((temp.value), line);
    
    
    ins << line;
    //fetch the last column in the output of ps aux, where the path is located
    for(i=0; i<10; i++){
        
        dummy.clear();
        ins >> dummy;
        
    }
    
    //I got to the last column, which constains the path. Because it may contain spaces, I put all of its words in dummy until the end of the column (ins) is reached
    dummy.clear();
    (run_directory.value).clear();
    //    run_directory.appendto(String("'"));
    do{
        ins >> dummy;
        run_directory.appendto(dummy);
        if(ins.tellg() != -1){run_directory.appendto(String("\ "));}
    }while(ins.tellg() != -1);
    
    //get the part of the path preceeding Contents/MacOS/Thelemacus and write it in run_directory
    position = (run_directory.value).find("Contents/MacOS/Thelemacus");
    
    if(position != string::npos){
        run_directory.set(String("Run directory"), String((run_directory.value).substr(0, position)), new_prefix);
    }
    
    
    
    
    
    //*****************
    
    //run pwd and get the current running path of this .o -> write it into path.dat so the app can read it later
    system("rm -rf path.dat; pwd >> path.dat;");
    infile.open("path.dat");
    running_directory.clear();
    getline(infile, running_directory);
    infile.close();
    //    system("rm -rf path.dat");
    
    cout << "Running directory = " << running_directory << "\n";
    
    library_directory.clear();
    library_directory << running_directory << "/../Resources/Libraries/";
    
    cout << "Library directory = " << library_directory.str().c_str() << "\n";
    
    //export the libraries so the app knows where to find them later
    command.clear();
    command << "export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:"
    << library_directory.str().c_str()
    << "; echo \$DYLD_LIBRARY_PATH"
    << "; ./main.o";
    
    cout << "Command  = " << command.str().c_str() << "\n";
    
    
    
    system(command.str().c_str());
    
    temp.close(new_prefix);
    temp.remove(new_prefix);

    
    return 0;
    
}
