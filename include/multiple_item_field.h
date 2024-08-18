//
//  multiple_item_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef multiple_item_field_h
#define multiple_item_field_h

#include <iostream>
#include <vector>

#include "my_string.h"

using namespace std;


//this class is the parent of all classes for GUI fields which include a list of multiple items to  be selected (such as ProjectionField, LengthFormatField, ...). All members and methods which are common to these children classes are written in this parent class. P is the type of the frame that hosts *this. NON_GUI is the type of the non-GUI field related to *this: For example, if I create a  class BodyNameField derived from MultipleItemField, then NON_GUI = Body; if I create a derived class LengthFormatField derived from MultipleItemField, NON_GUI = LengthFormat, etc. If there is no non-GUI object associated with *this, I set NON_GUI = void and call the constructor of MultipleItemField with second argument equal to NULL. CHECK is the type of the functor that will be used to check whetehr *this is valid: for example, for a BodyNameField CHECK = CheckBodyName<P>. If the check functor is not defined -> CHECK = void
template<class P, class NON_GUI, class CHECK> class MultipleItemField{
    
public:
    
    P* parent;
    //the non-GUI object related to *this: for example, if I create a  class BodyNameField derived from MultipleItemField, then *object will be a Body object
    NON_GUI* object;
    //a pointer to the functor that will be used to check whether *this is valid
    CHECK* check;
    wxBoxSizer *sizer_h, *sizer_v;
    wxArrayString  /*this is equal to a standard list of the available types*/items, /*a fixed catalog of the items in this MultipleChoiceField*/catalog;
    //I store the value in name, before name starts being edited, in value_before_editing
    wxString value_before_editing;
    //this is the wxComboBox with the name of the projections
    wxComboBox* name;
    //this points to an external vector<int> where the recent items are stored: the FillInRecentItems() method  reads from *recent_items
    vector<int>* recent_items;
    bool ok, /*this is true if *this is being edited, false otherwise*/editing;
//    template<class T> void (MultipleItemField::*OnChooseItem)(T&);


    MultipleItemField(wxPanel*, NON_GUI*, const vector<NON_GUI>&, vector<int>*);
    void FillInRecentItems(void);
    void set(void);
    void set(const NON_GUI&);
    template<class E> void Get(E&);
    void Enable(bool);
    bool is_ok(void);
    void CheckInCatalog(bool*, unsigned int *);
    template<class E> void Check(E&);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    void SetToolTip(const String&);
        
};

#endif
