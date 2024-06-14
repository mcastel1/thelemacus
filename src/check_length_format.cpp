//
//  check_length_format.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_length_format.h"

#include "constants.h"
#include "length_format_field.h"



template<class P> CheckLengthFormat<P>::CheckLengthFormat(LengthFormatField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void CheckLengthFormat<P>::operator()(T& event) {
    
    
    P* f = (p->parent);
    bool enable;

    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field
    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode

        unsigned int i;
        bool check;
      
        //I check whether the name in the GUI field body matches one of the route types  in catalog
        p->CheckInCatalog(&check, &i);

        if (check) {
            //the length format is valid

            //enable/disable the related fields in RouteFrame f
            //if the Route type is a loxodrome or orthodrome, enable = true. Otherwise, enable = false
            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));

            f->Z->Enable(enable);

            f->start_phi->Enable(!(f->for_transport));
            f->start_lambda->Enable(!(f->for_transport));

            f->GP_phi->Enable(!enable);
            f->GP_lambda->Enable(!enable);
            f->omega->Enable(!enable);

        }else {
            //the length format is not valid

            f->Z->Enable(false);
            f->start_phi->Enable(false);
            f->start_lambda->Enable(false);

            f->GP_phi->Enable(false);
            f->GP_lambda->Enable(false);
            f->omega->Enable(false);

        }

        f->OnChooseLengthFormatField();
        
    }
    
     
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}
