//
//  numerical_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef numerical_field_h
#define numerical_field_h

#include <iostream>
#include <vector>


using namespace std;


//template<class P, class NON_GUI, class CHECK> class MultipleItemField{
//
//public:
//
//    P* parent;
//    //the non-GUI object related to *this: for example, if I create a  class BodyNameField derived from MultipleItemField, then *object will be a Body object
//    NON_GUI* object;
//    //a pointer to the functor that will be used to check whether *this is valid
//    CHECK* check;
//    wxBoxSizer *sizer_h, *sizer_v;
//    wxArrayString  /*this is equal to a standard list of the available types*/items, /*a fixed catalog of the items in this MultipleChoiceField*/catalog;
//    //I store the value in name, before name starts being edited, in value_before_editing
//    wxString value_before_editing;
//    //this is the wxComboBox with the name of the projections
//    wxComboBox* name;
//    //this points to an external vector<int> where the recent items are stored: the Fill() method  reads from *recent_items
//    vector<int>* recent_items;
//    bool ok, /*this is true if *this is being edited, false otherwise*/editing;
////    template<class T> void (MultipleItemField::*OnChooseItem)(T&);
//
//
//    MultipleItemField(wxPanel*, NON_GUI*, const vector<String>&, vector<int>*);
//    void Fill(void);
//    void set(void);
//    void set(const NON_GUI&);
//    template<class E> void Get(E&);
//    void Enable(bool);
//    bool is_ok(void);
//    void CheckInCatalog(bool*, unsigned int *);
//    template<class E> void Check(E&);
//    template<class T> void InsertIn(T*);
//    template<class T> void InsertIn(T*, wxSizerFlags&);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
//    void SetToolTip(const String&);
//
//};

#endif
