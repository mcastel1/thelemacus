//
//  show_all.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef show_all_h
#define show_all_h

#include <iostream>

#include "list_frame.h"

using namespace std;

class ListFrame;


//class which defines a functor which shows ListFrame and all ChartFrames
class ShowAll{
    
public:
    
    ListFrame* frame;
    ShowAll(ListFrame*);
    
    template<class T> void operator()(T&);
    
};


#endif
