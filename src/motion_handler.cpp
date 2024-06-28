//
//  motion_handler.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "motion_handler.h"




template<class F> MotionHandler<F>::MotionHandler(ListFrame* parent_in, const Route& transporting_route_in, F* f_in){
    
    start = new Position;
    timer = new wxTimer();

    parent = parent_in;
    transporting_route = transporting_route_in;
    f = f_in;

    t = 0;
//    timer->Bind(wxEVT_TIMER, &GraphicalObjectTransportHandler::OnTimer, this);

}

template class MotionHandler<UnsetIdling<ListFrame>>;
template class MotionHandler<PrintMessage<ListFrame, UnsetIdling<ListFrame>>>;
