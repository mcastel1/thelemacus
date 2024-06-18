//
//  all_routes.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef all_routes_h
#define all_routes_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

class ListFrame;





//this class defines a functor () used to select all Routes to compute the astronomical position
class AllRoutes{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    AllRoutes(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


#endif
