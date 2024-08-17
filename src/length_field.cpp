//
//  length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "dynamic_length_field.h"
#include "generic.h"
#include "length.h"
#include "length_field.h"
#include "sight_frame.h"



//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(wxPanel* panel_of_parent, Length* object_in, Length* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >(panel_of_parent, object_in, recent_value_in, recent_units_in){

    wxSizerFlags flags;

    flags.Center();

    

  
  
 
}
