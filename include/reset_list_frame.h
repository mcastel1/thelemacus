//
//  reset_list_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef reset_list_frame_h
#define reset_list_frame_h

#include <iostream>

#include "list_frame.h"

using namespace std;

class ListFrame;


class ResetListFrame{
    
public:
    
    //the frame to be reset
    ListFrame* p;
    ResetListFrame(ListFrame*);
    
    template<class E> void operator()(E&);
    
};


#endif
