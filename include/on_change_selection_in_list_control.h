//
//  on_change_selection_in_list_control.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef on_change_selection_in_list_control_h
#define on_change_selection_in_list_control_h

#include "list_control.h"
#include "my_string.h"


#include <iostream>

using namespace std;


template<class P> class ListControl;
class String;



//if an item is selected/deselected in caller, enable/disable the disableable buttons in caller
template<class S> class OnChangeSelectionInListControl{
    
public:
    
    ListControl<S>* caller;
    //the type of listcontrol: "sight" if caller = listcontrol_sights, "position" if caller = listcontrol_positions, "route" if caller = listcontrol_routes
    String type;
    
    OnChangeSelectionInListControl(ListControl<S>*, String);
    template<class T> void operator()(T&);
    
};

#endif
