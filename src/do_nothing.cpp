//
//  do_nothing.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "do_nothing.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


DoNothing::DoNothing(void) {


}


template<class E> void DoNothing::operator()(E& event) {

    event.Skip(true);

}


template void DoNothing::operator()<wxCommandEvent>(wxCommandEvent&);
