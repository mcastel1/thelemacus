//
//  connect_disconnect.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "connect_disconnect.h"

#include "list_frame.h"


//constructor of the ConnectDisconnect functor: parent_in is the ListFrame parent of *this, sight_id_in is the # of the Sight to be connected / disconnected to Route # route_id_in
ConnectDisconnect::ConnectDisconnect(ListFrame* parent_in, const int& sight_id_in, const int& route_id_in) : parent(parent_in), sight_id(sight_id_in), route_id(route_id_in) {


}



void ConnectDisconnect::operator()(wxCommandEvent& event) {


    event.Skip(true);

}

//this is the same as template<class P> void ConnectDisconnect<P>::operator()(void){ but without the event argument
void ConnectDisconnect::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}
