//
//  static_text.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef static_text_h
#define static_text_h

//my own derived class of wxStaticText
class StaticText : public wxStaticText{
    
public:
    
    
    StaticText(wxWindow*, const wxString&, const wxPoint&, const wxSize&, long);
    
};

#endif
