//
//  existing_route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef existing_route_h
#define existing_route_h

#include <iostream>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

using namespace std;


class ListFrame;

//this class defines a functor () used to modify an existing Route
class ExistingRoute{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    ExistingRoute(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


#endif
