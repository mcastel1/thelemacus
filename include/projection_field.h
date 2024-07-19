//
//  projection_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef projection_field_h
#define projection_field_h

#include <iostream>
#include <vector>

#include "multiple_item_field.h"


using namespace std;

class Projection;


template<class P> class ProjectionField: public MultipleItemField<P, Projection, void>{
    
public:
    
//    P* parent;
//    wxBoxSizer *sizer_h, *sizer_v;
//    wxArrayString  /*this is equal to a standard list of the available graphical types*/items, /*same as projections, but it is fixed and never wrote to: it is a fixed catalog*/catalog;
    //this is the wxComboBox with the name of the projections
//    wxComboBox* name;
//    vector<int> recent_items;
//    bool ok;
//    CheckProjection<P>* check;

    ProjectionField(wxPanel*, vector<int>*);
//    void Fill(void);
//    void read_recent_projections(void);
//    template<class T> void InsertIn(T*, wxSizerFlags&);
    template<class E> void OnEdit(E&);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};



#endif
