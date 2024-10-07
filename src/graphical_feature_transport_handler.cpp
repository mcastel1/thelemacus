//
//  graphical_feature_transport_handler.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "graphical_feature_transport_handler.h"

#include <gsl_sf_psi.h>

#include "generic.h"
#include "motion_handler.h"
#include "my_app.h"
#include "on_change_selection_in_list_control.h"
#include "on_select_route_in_list_control_for_transport.h"
#include "to_do_at_end_of_transport.h"





//constructor of GraphicalFeatureTransportHandler: f_in is the functor to be provided if something is supposed to be executed at the end of the transport (e.g., do another transport, show a MessageFrame, etc...). If nothing is supposed to be executed, set f_in = NULL
template<class NON_GUI, class F> GraphicalFeatureTransportHandler<NON_GUI, F>::GraphicalFeatureTransportHandler(ListFrame* parent_in, NON_GUI* object_in,  const String& type_of_transported_object_in, const Route& transporting_route_in, F* f_in) : MotionHandler<F>(parent_in, transporting_route_in, f_in){

    transported_object = object_in;
    type_of_transported_object = type_of_transported_object_in;

    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &GraphicalFeatureTransportHandler::OnTimer, this);

}

template class GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>;
template class GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>>;
template class GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>;
template class GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>>;
template class GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame>>;
template class GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>;


//this method triggers the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::operator()(void) {
    
    //the animation transport starts here
    (MotionHandler<F>::timer)->Start(
        /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
        (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.get()) - 1)) * 1000.0,
        wxTIMER_CONTINUOUS);
    
}

template void GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>>::operator()();
template void GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>>>::operator()();
template void GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame>>::operator()();
template void GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame>>::operator()();


//this method iterates the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {

    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.get()))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (*((MotionHandler<F>::parent)->set_idling))();
            
            ((MotionHandler<F>::parent)->transporting) = true;

//            (MotionHandler<F>::transporting_route_temp) = (MotionHandler<F>::transporting_route);
            (MotionHandler<F>::transporting_route_temp).set((MotionHandler<F>::transporting_route));
            
            //during the transport, I disconnect DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
            for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
                (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
            }
            (MotionHandler<F>::parent)->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

            

            if (type_of_transported_object == String("position")) {

                //store the starting position in *geo_position_start
                //                (*(MotionHandler<F>::start)) = (*((Position*)transported_object));
                (MotionHandler<F>::start)->set((*((Position*)transported_object)));
                
                
                //highlight the Position that is being transported
                (MotionHandler<F>::parent)->highlighted_position_now.set(address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list));
                
            }else {

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    //store the starting reference position in *geo_position_start
                    //                    (*(MotionHandler<F>::start)) = (*(((Route*)transported_object)->reference_position));
                    (MotionHandler<F>::start)->set((*(((Route*)transported_object)->reference_position)));
                    
                    
                    //highlight the Position that is being transported
                    (MotionHandler<F>::parent)->highlighted_route_now.set(address_position_in_vector<Route>(((Route*)transported_object), (MotionHandler<F>::parent)->data->route_list));

                }
                
                //                (*(MotionHandler<F>::start)) = (*(((Route*)transported_object)->reference_position));
                (MotionHandler<F>::start)->set((*(((Route*)transported_object)->reference_position)));

            }

            //            ((MotionHandler<F>::transporting_route_temp).reference_position) = (MotionHandler<F>::start);
            (MotionHandler<F>::transporting_route_temp).reference_position->set((*(MotionHandler<F>::start)));

            //I brind all ChartFrames to front to show the animation
            wxGetApp().ShowCharts(event);
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk
            
            (MotionHandler<F>::transporting_route_temp).length->set(
                                                                    String(""),
                                                                    ((MotionHandler<F>::transporting_route).length->value) *
                                                                    (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.get()) + 1))))
                                                                    ,
                                                                    String(""));


            if (type_of_transported_object == String("position")) {

                //                (*((Position*)transported_object)) = (*(MotionHandler<F>::start));
                ((Position*)transported_object)->set((*(MotionHandler<F>::start)));
                
                ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                (MotionHandler<F>::parent)->TabulatePositionsAll();
                
            }else{

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    //                    (*(((Route*)transported_object)->reference_position)) = (*(MotionHandler<F>::start));
                    ((Route*)transported_object)->reference_position->set((*(MotionHandler<F>::start)));
                    
                    ((Route*)transported_object)->reference_position->transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                }

                (MotionHandler<F>::parent)->TabulateRoutesAll();
                
            }

            (MotionHandler<F>::parent)->MyRefreshAll();
            //            cout << "\t\t t= " << t << "\n";
            
            (MotionHandler<F>::t)++;

        }

    }else {
        //the transport  is over

        if (type_of_transported_object == String("position")) {
            
            //do the whole transport rather than combining many little transports, to avoid rounding errors
            //            (*((Position*)transported_object)) = (*(MotionHandler<F>::start));
            ((Position*)transported_object)->set((*(MotionHandler<F>::start)));
            
            //un-highlight the Position that is being transported
            (MotionHandler<F>::parent)->highlighted_position_now.set(-1);
            ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route), String(""));


            //update labels
            (((Position*)transported_object)->label) = ((Position*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));
            
            //update the Position information in f
            ((Position*)transported_object)->update_ListControl(
                                                               address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list),
                                                               (MotionHandler<F>::parent)->listcontrol_positions
                                                               );


        }else{

            if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                String new_label;
                
                //un-highlight the Route that is being transported
                (MotionHandler<F>::parent)->highlighted_route_now.set(-1);

                //do the whole transport rather than combining many little transports, to avoid rounding errors
                //                (*(((Route*)transported_object)->reference_position)) = (*(MotionHandler<F>::start));
                ((Route*)transported_object)->reference_position->set((*(MotionHandler<F>::start)));
                ((Route*)transported_object)->reference_position->transport_to((MotionHandler<F>::transporting_route), String(""));


                //update labels

                //the new label which will be given to the transported Route
                new_label = ((Route*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));

                //set back listcontrol_routes to route_list, in order to include all Routes (not only those which are not related to a Sight)
                (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);

                if ((type_of_transported_object == String("sight")) || ( ((type_of_transported_object == String("route")) && ((((Route*)transported_object)->related_sight.get()) != -1)) )) {
                    //I am transporting a Sight (i.e., Route related to a Sight) or I am transporting a Route that is connected to a Sight -> disconnect the Route from the sight

                    ((MotionHandler<F>::parent)->disconnect_sight->sight_id) = (((Route*)transported_object)->related_sight.get());
                    (MotionHandler<F>::parent)->disconnect_sight->operator()(event);

                }
                
            

                //change the label of *object by appending to it 'translated with [label of the translating Route]'
                (((Route*)transported_object)->label) = new_label;

            }

        }
        
        //set parameters back to their original value and reset listcontrol_routes to the original list of Routes

        (MotionHandler<F>::parent)->listcontrol_sights->set(((MotionHandler<F>::parent)->data->sight_list), false);
        (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);
        (MotionHandler<F>::parent)->Resize();
        //re-draw everything
        //(MotionHandler<F>::parent)->PreRenderAndFitAll();
        
        //re-bind DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement once the transport is over
        for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
            (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
        }
        (MotionHandler<F>::parent)->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

        
        //re-bind listcontrol_routes to &ListFrame::OnChangeSelectionInListControl
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_DESELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));


        if (((MotionHandler<F>::parent)->transporting_with_selected_route)) {
            //I am transporting with an existing, selected Route

            //the transport is over -> I reverse the Bind/Unbind(s) made before the transport started
            ((MotionHandler<F>::parent)->transporting_with_selected_route) = false;
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_LIST_ITEM_ACTIVATED, *((MotionHandler<F>::parent)->on_select_route_in_listcontrol_routes_for_transport));
            (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, (MotionHandler<F>::parent));

        }

        if (((MotionHandler<F>::parent)->transporting_with_new_route)) {
            //I am tranporting with a new Route

            ((MotionHandler<F>::parent)->transporting_with_new_route) = false;

        }

        (MotionHandler<F>::timer)->Stop();
        //        (*((MotionHandler<F>::parent)->unset_idling))();
        ((MotionHandler<F>::parent)->transporting) = false;
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }

    }

}

