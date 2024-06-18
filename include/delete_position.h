//
//  delete_position.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef delete_position_h
#define delete_position_h

#include <iostream>


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "list_frame.h"

using namespace std;

class ListFrame;



//this class defines the functor () used to remove a Position from the non-GUI object data
class DeletePosition{
    
public:
    
    DeletePosition(ListFrame*);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the Position to be removed
    long i_position_to_remove;
    
    
    void operator()(wxCommandEvent&);
    
};


#endif
