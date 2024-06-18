//
//  new_route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef new_route_h
#define new_route_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

class ListFrame;


//this class defines a functor () used to create a new Route
class NewRoute{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    NewRoute(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


#endif
