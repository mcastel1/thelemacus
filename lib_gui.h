//
//  lib_h.h
//  
//
//  Created by MacBook Pro on 16/02/2022.
//

class angle_field{
    
    
    public:
    wxComboBox* deg;
    wxTextCtrl *min;
    wxStaticText* text_deg, *text_min;
    wxBoxSizer* sizer_h, *sizer_v;
    
    angle_field(wxPanel*);
    void insert(wxGridSizer*);
    
    
};

angle_field::angle_field(wxPanel* panel){
    
    deg = new wxComboBox(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    deg->SetValue("");

    text_deg = new wxStaticText(panel, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS("0.000000"))));
    //min->Bind(wxEVT_KILL_FOCUS, wxFocusEventHandler(MyFrame::CheckHsMinutes), this);

    text_min = new wxStaticText(panel, wxID_ANY, wxT("'"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_h->Add(deg);
    sizer_h->Add(text_deg);
    sizer_h->Add(min);
    sizer_h->Add(text_min);
    
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS("0.000000"))));

    
}

void angle_field::insert(wxGridSizer* host){
    
    host->Add(sizer_h);
    
}

