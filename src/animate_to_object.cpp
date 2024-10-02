//
//  animate_to_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "animate_to_object.h"

#include "chart_frame.h"
#include "chart_transport_handler.h"
#include "generic.h"
#include "list_frame.h"


template<class T, class F> AnimateToObject<T, F>::AnimateToObject(ListFrame* parent_in, T* object_in, F* f_in) : parent(parent_in), object(object_in), f(f_in) {
    
}

template class AnimateToObject<Position, HighlightObject<ListFrame, DoNothing>>;
template class AnimateToObject<Route, UnsetIdling<ListFrame>>;
template class AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame>>>;
template class AnimateToObject<Position, PrintMessage<ListFrame, UnsetIdling<ListFrame>>>;
template class AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>;
template class AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>;
template class AnimateToObject<Route, HighlightObject<ListFrame, UnsetIdling<ListFrame>>>;
template class AnimateToObject<Position, HighlightObject<ListFrame, UnsetIdling<ListFrame>>>;


//trigger the animation
template<class T, class F> template<class E> void AnimateToObject<T, F>::operator()(E& event) {
    
    unsigned int i;
    //the Position where the chart will be centered by the animation triggered when the user presses ok
    Position target_position;
    //the aperture angle of circle_observer at the end of the animation
    Angle target_omega;
    //the transport handlers used to transport the chart: there is one ChartTransportHandler per ChartFrame
    vector< ChartTransportHandler<F>* > chart_transport_handlers;
    Length d;
    
    chart_transport_handlers.resize(parent->chart_frames.size());
    
    //bring all charts to front to show the animation
    wxGetApp().ShowCharts();
    
    for(i=0; i<parent->chart_frames.size(); i++){
        
        switch (position_in_vector((((parent->chart_frames)[i])->projection), Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]
                
                //the center of the Mercator projection
                Position center;
                //the size of the object to which the animation is directed, in units of the coordinates x, y of the Mercator projection
                PositionProjection target_size, size_rectangle_observer_0;
                Double zoom_factor;
                
                if(std::is_same<T, Route>::value){
                    //*object is a Route
                    
                    Route* route_object;
                    
                    route_object = (Route*)object;
                    
                    if(route_object->type == Route_types[2]){
                        //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                        
                        target_position = (*(route_object->reference_position));
                        
                    }else{
                        //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                        
                        Length length_saved;
                        
                        if((route_object->length_format) == LengthFormat_types[0]){
                            //length_format = LengthFormat_types[0] -> compute length from time and speed and have it in units LengthUnit_types[0] because this is the standard unit used to draw Routes
                            
                            route_object->set_length_from_time_speed();
                            
                        }else{
                            //length_format = LengtFormat_types[1] -> save *length into length_saved and convert the unit of measure of *length to LengthUnit_types[0] because this is the standard unit used to draw Routes
                            
                            length_saved.set((*(route_object->length)));
                            route_object->length->convert_to(LengthUnit_types[0]);
                            
                        }
                        
                        
                        route_object->compute_end(((*(route_object->length))/2.0), String(""));
                        target_position = (*(route_object->end));
                        
                        //write back length_saved into *length
                        route_object->length->set(length_saved);
                        
                    }
                    
                    //compute the size of the route_object to which the animation is directed and store it into target_size
                    route_object->size_Mercator(&target_size);
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //object is a Position
                    
                    Position* position_object;
                    
                    position_object = (Position*)object;
                    
                    //the target Position of the animation is *position_object
                    target_position = (*position_object);
                    //Positions do not have a size such as Routes -> create a Route of type Route_types[2] which has target_position as a reference_position and which has an aprture angle equal to angle_zoom_to_position -> compute its size in the mercator projection and write it into target_size
                    Route(Route_types[2], target_position, wxGetApp().angle_zoom_to_position).size_Mercator(&target_size);
                    
                }
                
                center.lambda.set((((*(((parent->chart_frames)[i])->lambda_min)) + (*(((parent->chart_frames)[i])->lambda_max))).value)/2.0);
                center.phi.set(mean_pm_pi((*(((parent->chart_frames)[i])->phi_min)), (*(((parent->chart_frames)[i])->phi_max))));
                
                parent->rectangle_observer_0->SizeMercator(&size_rectangle_observer_0);
                
                zoom_factor.value = (wxGetApp().chart_transport_zoom_factor_coefficient.value) * min((size_rectangle_observer_0.x)/(target_size.x), (size_rectangle_observer_0.y)/(target_size.y));
                
                
                chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                           ((parent->chart_frames)[i]),
                                                                           Route(
                                                                                 Route_types[0],
                                                                                 center,
                                                                                 target_position
                                                                                 ),
                                                                           zoom_factor,
                                                                           f
                                                                           );
                
                //trigger the animation
                (chart_transport_handlers[i])->operator()();
                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
                
                //the aperture angle of circle_observer at the end of the animation
                Angle omega_end;
                
                
                if(std::is_same<T, Route>::value){
                    //object is a Route
                    
                    //I introduce the Route* route_object and set route_object = object by casting objec into a Route pointer. This is necessary to make this method work with multiple types T (T=Position, T=Route, ...)
                    Route* route_object;
                    
                    route_object = (Route*)object;
                    
                    if(route_object->type == Route_types[2]){
                        //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                        
                        target_position = (*(route_object->reference_position));
                        omega_end = route_object->omega;
                        
                        
                    }else{
                        //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                        
                        route_object->set_length_from_time_speed();
                        
                        route_object->compute_end(((*(route_object->length))/2.0), String(""));
                        target_position = (*(route_object->end));
                        
                        omega_end = (route_object->length->value)/2.0/(wxGetApp().Re.value);
                        
                    }
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //*object is a Position
                    
                    Position* position_object;
                    
                    position_object = (Position*)object;
                    
                    //the target Position of the animation is *object
                    target_position = (*position_object);
                    
                    //Positions do not have a size such as Routes -> set omega_end equal to  angle_zoom_to_position projection and write it into target_size
                    omega_end.set(wxGetApp().angle_zoom_to_position);
                    
                }
                
                //compute the distance between the start and end poisition of the proposed andimation and store it in d
                target_position.distance(*(((parent->chart_frames)[i])->draw_panel->circle_observer->reference_position), &d, String(""), String(""));
                
                //I do the animaiton only if the start and end position of the animation are large enough, in order to avoid NaNs in the transporting_route
                if (d > (wxGetApp().minimal_animation_distance_over_size_of_observer_region.value) * ((wxGetApp().Re.value)*((parent->chart_frames)[i])->draw_panel->circle_observer->omega.value)) {
                    
                    chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                               ((parent->chart_frames)[i]),
                                                                               Route(
                                                                                     Route_types[1],
                                                                                     (*(((parent->chart_frames)[i])->draw_panel->circle_observer->reference_position)),
                                                                                     target_position
                                                                                     ),
                                                                               Double( ((wxGetApp().chart_transport_zoom_factor_coefficient.value) *  (parent->circle_observer_0->omega.value) / (omega_end.value) ) ),
                                                                               f
                                                                               );
                    
                    //trigger the animation
                    (chart_transport_handlers[i])->operator()();
                    
                    
                }
                
                break;
                
            }
                
        }
        
    }
    
    
    
    event.Skip(true);
    
}

template void AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void AnimateToObject<Position, HighlightObject<ListFrame, DoNothing>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void AnimateToObject<Route, HighlightObject<ListFrame, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void AnimateToObject<Position, HighlightObject<ListFrame, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);


//this is the same as template<class T, class F> template<class E> void AnimateToObject<T, F>::operator()(E& event) but without the event argument
template<class T, class F> void AnimateToObject<T, F>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}

template void AnimateToObject<Route, UnsetIdling<ListFrame>>::operator()();
template void AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame>>>::operator()();
template void AnimateToObject<Position, PrintMessage<ListFrame, UnsetIdling<ListFrame>>>::operator()();
template void AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>::operator()();
template void AnimateToObject<Route, HighlightObject<ListFrame, UnsetIdling<ListFrame>>>::operator()();
