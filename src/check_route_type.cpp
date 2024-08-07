//
//  check_route_type.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_route_type.h"

#include "constants.h"
#include "route_frame.h"


template<class P> CheckRouteType<P>::CheckRouteType(RouteTypeField<P>* p_in) {

    p = p_in;


}

template class CheckRouteType<RouteFrame>;


//this functor checks the wxComboBox containing the Route type, and if it is equal to loxodrome or orthodrome, it enables length, Z and start_* fields in RouteFrame (the latter if for_transport = false, becasue I don't need a start position if I am using the Route for transport). If it is equal to circle of equal altitude, it enables only the GP and omege fields.
template<class P> template<class T> void CheckRouteType<P>::operator()(T& event) {

    P* f = (p->parent);
//    bool enable;
    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field

    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode
        
        f->EnableDisableFields(event);

//        unsigned int i;
//        bool check;
//
//        //I check whether the RouteTypeField *p of the GUI field matches one of the route types in catalog
//        p->CheckInCatalog(&check, &i);
//
//        if (check) {
//            //the Route type is valid
//
//
//            //enable/disable the related fields in RouteFrame f
//            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));
//
//            f->Z->Enable(enable);
//
//            //for start_phi/lambda to be enabled, not only the Route has to be of tyoe Route_types[0] or Route_types[1], but, in addition, it must not be a Route for transport (if it were, there would be no need to indicate its starting Position to do the transport )
//            f->start_phi->Enable(enable && (!(f->for_transport)));
//            f->start_lambda->Enable(enable && (!(f->for_transport)));
//
//            f->GP_phi->Enable(!enable);
//            f->GP_lambda->Enable(!enable);
//            f->omega->Enable(!enable);
//
//        }else{
//            //the Route type is not valid
//
//            f->Z->Enable(false);
//            f->start_phi->Enable(false);
//            f->start_lambda->Enable(false);
//
//
//            f->GP_phi->Enable(false);
//            f->GP_lambda->Enable(false);
//            f->omega->Enable(false);
//
//        }

        f->EnableDisableLengthFormatFields();

    }
    
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}

template void CheckRouteType<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckRouteType<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
