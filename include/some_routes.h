//
//  some_routes.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef some_routes_h
#define some_routes_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

class ListFrame;


//this class defines a functor () used to select some specific Routes to compute the astronomical position
class SomeRoutes{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    SomeRoutes(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


#endif
