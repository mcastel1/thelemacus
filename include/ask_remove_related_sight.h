//
//  ask_remove_related_sight.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef ask_remove_related_sight_h
#define ask_remove_related_sight_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

class ListFrame;



//this functor pops up a QuestionFrame and asks the user whether he wants to remove the a Sight related to a Route
class AskRemoveRelatedSight{
    
public:
    
    AskRemoveRelatedSight(ListFrame*);
    
    //the frame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};


#endif
