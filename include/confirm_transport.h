//
//  confirm_transport.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef confirm_transport_h
#define confirm_transport_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

//this functor is called when the user has chosen that he/she wants to condfirm transporting an object
template<class P> class ConfirmTransport{
    
public:
    
    ConfirmTransport(P*);
    
    //the frame which called this struct
    P* parent;
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};



#endif
