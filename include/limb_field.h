//
//  limbfield.h
//  minimal_cocoa
//
//  Created by Michele on 11/06/2024.
//

#ifndef limb_field_h
#define limb_field_h

template<class P> class LimbField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    
    //this is the wxComboBox with the name of the limbs
    wxCheckListBox* name;
    wxArrayInt checked_items;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;
    CheckLimb<P>* check;
    OnChangeSelectionInLimbField<P>* change_selection;
    
    LimbField(wxPanel*, Limb*);
    void set(void);
    void Enable(bool);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEdit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};



#endif /* limbfield_h */

