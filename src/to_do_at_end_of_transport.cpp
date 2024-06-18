//
//  to_do_at_end_of_transport.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "to_do_at_end_of_transport.h"




template<class NON_GUI, class P> ToDoAtEndOfTransport<NON_GUI, P>::ToDoAtEndOfTransport(NON_GUI* object_a_in, NON_GUI* object_b_in, P* parent_in){
    
    object_a = object_a_in;
    object_b = object_b_in;
    parent = parent_in;

}


//set *object_a equal to *object_b if object_a != NULL, and to the other tasks to be done at the end of a transport
template<class NON_GUI, class P> void ToDoAtEndOfTransport<NON_GUI, P>::operator()(void){
    
    if(object_a){
        (*object_a) = (*object_b);
    }
    
    if((parent->i_object_to_disconnect) != -1){
        
        //print an info message
        parent->print_info_message->SetAndCall(NULL, String("Warning"), String("The transported route  was related to a sight! The route has been disconnected from the sight."), (wxGetApp().path_file_info_icon));
        
       (parent->i_object_to_disconnect) = -1;
        
    }
    
    parent->listcontrol_sights->set((parent->data->sight_list), false);
    parent->listcontrol_routes->set((parent->data->route_list), false);
    parent->Resize();
    parent->PreRenderAll();

}
