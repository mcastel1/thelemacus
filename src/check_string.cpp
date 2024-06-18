//
//  check_string.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_string.h"





template<class P> CheckString<P>::CheckString(StringField<P>* p_in) {

    p = p_in;

}

//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T& event) {

    P* f = (p->parent);

    f->AllOk();

    event.Skip(true);

}
