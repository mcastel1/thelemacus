//
//  delete_route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef delete_route_h
#define delete_route_h

#include <iostream>

#include "answer.h"

using namespace std;

class Answer;
class ListFrame;


//this class defines the functor () used to remove a Route from the non-GUI object data
class DeleteRoute{
    
public:
    
    DeleteRoute(ListFrame*, Answer);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the route to be removed
    long i_route_to_remove;
    //this is equal to 'y' if the sight related to the removed route has to be removed too, and 'n' otherwise
    Answer remove_related_sight;
    
    void operator()(wxCommandEvent&);
    
};



#endif
