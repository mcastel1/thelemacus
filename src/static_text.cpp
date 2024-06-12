//
//  static_text.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "static_text.h"


//construct a StaticText object from a wxStaticText object, and sets its color
StaticText::StaticText(wxWindow* parent, const wxString& label, const wxPoint& pos, const wxSize& size, long style) : wxStaticText(parent, wxID_ANY, label, pos, size, style, wxT("")) {

    //SetColor(this);

}
