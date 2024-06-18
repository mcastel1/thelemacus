//
//  to_do_at_end_of_transport.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef to_do_at_end_of_transport_h
#define to_do_at_end_of_transport_h

#include <iostream>

using namespace std;



//this functor does all the necessary tasks to be done at the end of an animated transporty: sets the non-GUI object *object_a  (for example, a Position, Route...) of type NON_GUI equal to *object_b, sets and redraws everything in the parent of type P, ...
template<class NON_GUI, class P> class ToDoAtEndOfTransport{
    
public:
    NON_GUI *object_a, *object_b;
    P* parent;

    ToDoAtEndOfTransport(NON_GUI*, NON_GUI*, P*);
    
    void operator()(void);
    
};


#endif
