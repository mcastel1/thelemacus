#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

class File{

 public:
  fstream value;
  string name;
  unsigned int number_of_lines;
  
  
};


void print_filename(File& file){

  cout << "Name of file is " << file.name << "\n";
  
}


int main(void){

  File file;
  file.name = "ciao";
  print_filename(file);
  

  cout << "\n";
  return(0);
	
}
