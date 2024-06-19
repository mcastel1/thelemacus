//
//  delete_sight.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef delete_sight_h
#define delete_sight_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "answer.h"

using namespace std;

class Answer;
class ListFrame;


//this class defines the functor () used to remove a sight from the non-GUI object data
class DeleteSight{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the sight to be removed
    long i_sight_to_remove;
    //this is equal to 'y' if the route related to the removed sight has to be removed too, and 'n' otherwise
    Answer remove_related_route;
    
    DeleteSight(ListFrame*, Answer);
    void operator()(wxCommandEvent&);
    
};


#endif
