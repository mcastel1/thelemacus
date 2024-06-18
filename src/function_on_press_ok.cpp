//
//  function_on_press_ok.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "function_on_press_ok.h"

template<class P> FunctionOnPressOk<P>::FunctionOnPressOk(P* parent_in) {

    parent = parent_in;

}

template<class P> void FunctionOnPressOk<P>::operator()(wxCommandEvent& event) {

    //do something

    event.Skip(true);

}
