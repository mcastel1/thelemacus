//
//  ask_remove_related_route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef ask_remove_related_route_h
#define ask_remove_related_route_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "list_frame.h"

using namespace std;

class ListFrame;


//this functor pops up a QuestionFrame and asks the user whether he wants to remove the a Route related to a Sight
class AskRemoveRelatedRoute{
    
public:
    
    AskRemoveRelatedRoute(ListFrame*);
    
    //the frame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};

#endif
