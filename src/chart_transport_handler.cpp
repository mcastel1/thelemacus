//
//  chart_transport_handler.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "chart_transport_handler.h"


#include "gsl_sf_psi.h"


#include "chart_frame.h"
#include "double.h"
#include "generic.h"
#include "highlight_object.h"


//constructor of ChartTransportHandler, which initializes *this with the Route transporting_route_in (used to to the transport) and with proposed zoom factor proposed _zoom_factor at end fo the transport.  This is a `proposed` zoom factor because, if such proposed zoom factor is < 1 or > zoom_factor_max, the actual zoom factor will be set to 1 and zoom_factor_max, respectively. Othersize, the actual zoom_factor will be equal to proposed_zoom_factor.
template<class F> ChartTransportHandler<F>::ChartTransportHandler(ChartFrame* chart_in, const Route& transporting_route_in, const Double& proposed_zoom_factor, F* f_in) : MotionHandler<F>(chart_in->parent, transporting_route_in, f_in){
    
    
    chart_frame = chart_in;
    
    if(proposed_zoom_factor.value < 1.0){
        zoom_factor = 1.0;
    }else{
        if(zoom_factor > wxGetApp().zoom_factor_max){
            zoom_factor = wxGetApp().zoom_factor_max;
        }else{
            zoom_factor = proposed_zoom_factor;
        }
    }
    
    
    
    switch (position_in_vector(chart_frame->projection, Projection_types)) {
            
        case 0:{
            //I am using Projection_types[0]
            
            PositionProjection temp;
            
            //compute projection_size_end from zoom_factor and *rectangle_observer_0
            chart_frame->parent->rectangle_observer_0->SizeMercator(&temp);
            projection_size_end = temp / zoom_factor;

            
            break;
            
        }
        
        case 1:{
            //I am using Projection_types[1]

            //compute omega_end from zoom_factor and circle_observer_0->omega
            omega_end.set((chart_frame->parent->circle_observer_0->omega.value) / (zoom_factor.value));

            break;
            
        }
            
            
    }
    
    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &ChartTransportHandler::OnTimer, this);

}

template class ChartTransportHandler<PrintMessage<ListFrame, UnsetIdling<ListFrame>>>;
template class ChartTransportHandler<UnsetIdling<ListFrame>>;
template class ChartTransportHandler<HighlightObject<ListFrame, DoNothing>>;
template class ChartTransportHandler<HighlightObject<ListFrame, DisconnectSight>>;
template class ChartTransportHandler<HighlightObject<ListFrame, UnsetIdling<ListFrame>>>;


//prompt the movement of the center of the chart from position a to position b
template<class F> void ChartTransportHandler<F>::operator()(void) {
//void ChartTransportHandler::MoveChart(const Position& a, const Position& b){
    
    if(!((MotionHandler<F>::parent)->idling)){
        //I start the animation only if *parent is not in idling mode
        
        //before I initiate the transport, I unbind DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
        chart_frame->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
        chart_frame->parent->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->draw_panel->Unbind(wxEVT_MOUSEWHEEL, &DrawPanel::OnMouseWheel, chart_frame->draw_panel);
        
        //because I am about to trigger an animation, I set all DrawPanels to idling mode. I do not set the ChartFrame to idling mode because I need the projection_field to be updated with the recently selected value of the projection, if needed
        (MotionHandler<F>::parent)->SetIdlingAllDrawPanels(true);
        
        //the animation transport starts here (only if the parent ChartFrame is not in idling mode)
        (MotionHandler<F>::timer)->Start(
                     /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
                     (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.value) - 1)) * 1000.0,
                     wxTIMER_CONTINUOUS);
        
    }
    
    
}

template void ChartTransportHandler<PrintMessage<ListFrame, UnsetIdling<ListFrame>>>::operator()();
template void ChartTransportHandler<UnsetIdling<ListFrame>>::operator()();
template void ChartTransportHandler<HighlightObject<ListFrame, DoNothing>>::operator()();
template void ChartTransportHandler<HighlightObject<ListFrame, DisconnectSight>>::operator()();
template void ChartTransportHandler<HighlightObject<ListFrame, UnsetIdling<ListFrame>>>::operator()();


//this method iterates the animation
template<class F> void ChartTransportHandler<F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
    
    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.value))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport and *parent is not in idling mode -> proceed with the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (chart_frame->dragging_chart) = true;
            chart_frame->EnableAll(false);

            (MotionHandler<F>::transporting_route_temp).set((MotionHandler<F>::transporting_route));
            
            (MotionHandler<F>::start)->set((*((MotionHandler<F>::transporting_route).reference_position)));

            (chart_frame->draw_panel->label_position) = String("");

    
            switch (position_in_vector(chart_frame->projection, Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_A, q_B;
//                    Position p_A, p_B;
                    
    
                    //write in p_NW and p_SE the two corner points of the projection and write in projection_size the size (in x,y) of the relative rectangle
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), (*(chart_frame->phi_max))));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), (*(chart_frame->phi_min))));
                    projection_size.x = chart_frame->draw_panel->x_span();
                    projection_size.y = (q_A.y) - (q_B.y);
                    projection_size_start.set(projection_size);
                    
                    
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0->p_NW->phi));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0->p_SE->phi));
                    (projection_size_end.x) = chart_frame->draw_panel->x_span_0;
                    projection_size_end.y = (q_A.y) - (q_B.y);
                    projection_size_end /= (zoom_factor.value);
               
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    //the Position where I start the animation (start) may not coincide with circle_observer.reference_position (for example, I may want to start the animaiton from the antipode of circle_observer.reference_position to show a nice turn of the earth during the animaiton): thus, to start the animation, I need to first set rotation to the rotation that brings circle_observer.reference_position to be centered on start -> to do this, I do
                    chart_frame->draw_panel->rotation->set(((*(chart_frame->draw_panel->rotation)) * Rotation((*(MotionHandler<F>::start)), (*(chart_frame->draw_panel->circle_observer->reference_position)))));

                    
                    chart_frame->draw_panel->rotation_start_drag->set((*(chart_frame->draw_panel->rotation)));
//                    (*(chart_frame->draw_panel->circle_observer->reference_position)) = (*(MotionHandler<F>::start));
                    chart_frame->draw_panel->circle_observer->reference_position->set((*(MotionHandler<F>::start)));
                    
//                    omega_start = chart_frame->draw_panel->circle_observer->omega;
                    omega_start.set(chart_frame->draw_panel->circle_observer->omega);
                    
                    break;
                    
                }
                    
            }
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk
            
            (MotionHandler<F>::transporting_route_temp).length->set(
                                                                    String(""),
                                                                    ((MotionHandler<F>::transporting_route).length->value) *
                                                                    (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value)))))
                                                                    ,
                                                                    String(""));
            
            
            switch (position_in_vector(chart_frame->projection, Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_center;
                    
                    //transport the starting point of the animation, start, according to transporting_route_temp, and store the result in p_center -> this yields the updated center of the chart
                    (MotionHandler<F>::start)->transport(&p_center, (MotionHandler<F>::transporting_route_temp), String(""));
                    //transform p_center into a PositionProjection
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->GeoToProjection))(p_center, &q_center, true);
                    
                    
                    //update projection_size
                    projection_size = projection_size_start + (projection_size_end - projection_size_start) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));

                    
                    //shift q_center to the NE and to the SW by projection_size/2 -> these will be the updated values of p_NE and p_SE
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center + projection_size/2.0, &p_NE);
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center - projection_size/2.0, &p_SW);

                    //set lambda / phi min/max according to p_NE and p_SW
                    (*(chart_frame->lambda_max)) = (p_NE.lambda);
                    (*(chart_frame->phi_max)) = (p_NE.phi);

                    (*(chart_frame->lambda_min)) = p_SW.lambda;
                    (*(chart_frame->phi_min)) = p_SW.phi;
          
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    (MotionHandler<F>::transporting_route_temp).compute_end(String(""));
                    
                    //conpute the new rotation: the new rotation of the earth is the old one, composed with the rotation which brings the old reference_position onto the new one
                    chart_frame->draw_panel->rotation->set(((*(chart_frame->draw_panel->rotation_start_drag)) * Rotation((*((MotionHandler<F>::transporting_route_temp).end)), (*(MotionHandler<F>::start)))));
                    
                    //                    (*(chart_frame->draw_panel->circle_observer->reference_position)) = (*((MotionHandler<F>::transporting_route_temp).end));
                    chart_frame->draw_panel->circle_observer->reference_position->set((*((MotionHandler<F>::transporting_route_temp).end)));

                    chart_frame->draw_panel->circle_observer->omega = omega_start.value + (omega_end.value - omega_start.value) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));
                    
                    
                    break;
                    
                }
                    
            }
            
            //re-draw the chart
            (chart_frame->draw_panel->*(chart_frame->draw_panel->PreRender))();
            chart_frame->draw_panel->MyRefresh();
            chart_frame->UpdateSlider();
            
            (MotionHandler<F>::t)++;

        }

    }else{
        //the transport  is over

        switch (position_in_vector(chart_frame->projection, Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]

                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
       

                chart_frame->draw_panel->rotation_end_drag->set((*(chart_frame->draw_panel->rotation)));

                break;
                
            }
                
        }
            
        (chart_frame->dragging_chart) = false;
        chart_frame->EnableAll(true);
        chart_frame->Fit();

        //bind back all the methoud that have been unbound at the beginnign of the transport
        chart_frame->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
        chart_frame->parent->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->draw_panel->Bind(wxEVT_MOUSEWHEEL, &DrawPanel::OnMouseWheel, chart_frame->draw_panel);

        
        (MotionHandler<F>::timer)->Stop();
        (MotionHandler<F>::parent)->SetIdlingAllDrawPanels(false);
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }
        
    }

}
