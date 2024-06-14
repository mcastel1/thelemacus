//
//  list_control.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef list_control_h
#define list_control_h

#include <iostream>

#include <vector>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/textctrl.h>
#include <wx/listctrl.h>



using namespace std;


//my own derived class of wxListCtrl, which contains objects of type S
template<class S> class ListControl : public wxListCtrl{
    
public:
    
    //a vector containing pointers to the buttons of *this (modify button, etc ...) which may be disabled if some event occurs
    vector<wxButton*> disableable_buttons;
    //header_width[i] = width of the header of the i-th column
    vector<int> header_width;
    
    ListControl(wxWindow*, vector<wxButton*>, const wxPoint&, const wxSize&, long);
    
    void SetColumns(vector<wxString>);
    void PushBackColumn(wxString);
    void DeselectAll(void);
    void set(vector<S>, bool);
    void EnableButtons(bool);
    void Resize(vector<S>);
    void GetSelectedItems(vector<long>*);
    
};


#endif
