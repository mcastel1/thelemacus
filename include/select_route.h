//
//  select_route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef select_route_h
#define select_route_h

#include <iostream>

using namespace std;

class ListFrame;


//this functor allows the user to select a Route in *parent frame
class SelectRoute{
    
public:
    
    SelectRoute(ListFrame*);
    
    //the ListFrame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};


#endif
