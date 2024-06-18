//
//  function_on_press_ok.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef function_on_press_ok_h
#define function_on_press_ok_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

//this class defines the functor () used to ...
template<class P> class FunctionOnPressOk{
    
public:
    
    FunctionOnPressOk(P*);
    
    //the frame which called this struct
    P* parent;
    
    void operator()(wxCommandEvent&);
    
};



#endif
