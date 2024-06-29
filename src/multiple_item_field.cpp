//
//  multiple_item_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "multiple_item_field.h"
#include "my_string.h"
#include "my_app.h"
#include "route_frame.h"
#include "sight_frame.h"

#include <sstream>

using namespace std;

//constructor of a MultipleItemField object, which is into *panel_of_parent. The list of items in *this is stored into catalog_in. All items that are general enough to be common to all classes which are inherited from MultipleItemField are initialized here. Items that are specific to the inherited classes will be initialized in the inherited-class constructors.
template<class P, class NON_GUI, class CHECK> MultipleItemField<P, NON_GUI, CHECK>::MultipleItemField(wxPanel* panel_of_parent, NON_GUI* object_in, const vector<String>& catalog_in, vector<int>* recent_items_in){
    
    unsigned int i;

    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited
    editing = false;
    //to be conservative, I set
    ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    //set catalog equal to catalog_in
    for(catalog.Clear(), i=0; i<catalog_in.size(); i++){
        catalog.push_back(wxString(catalog_in[i].value));
    }
    recent_items = recent_items_in;

    items = catalog;

    name = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(name);
    Fill();
    name->SetValue(items[0]);
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = name->GetValue();
    AdjustWidth(name);
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call Check to check whether what has been entered in name is valid
    //    name->Bind(wxEVT_COMBOBOX, &MultipleItemField::OnEdit<wxCommandEvent>, this);
    //    name->Bind(wxEVT_KEY_UP, &MultipleItemField::OnEdit<wxKeyEvent>, this);
    //    name->Bind(wxEVT_KILL_FOCUS, *check);
    name->Bind(wxEVT_KILL_FOCUS, &MultipleItemField<P, NON_GUI, CHECK>::Check<wxFocusEvent>, this);
    //THIS LINE CAUSES AN ERROR
    //    name->Bind(wxEVT_COMBOBOX, &MultipleItemField<P, NON_GUI, CHECK>::Check<wxCommandEvent>, this);

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}

template class MultipleItemField<ChartFrame, Projection, void>;
template class MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>;
template class MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>;
template class MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>;
template class MultipleItemField<SightFrame, Body, CheckBody<SightFrame>>;



template<class P, class NON_GUI, class CHECK> template <typename EventTag, typename Method, typename Object> void MultipleItemField<P, NON_GUI, CHECK>::Bind(EventTag tag, Method method, Object object) {

    name->Bind(tag, method, object);

}

template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (LengthField<RouteFrame>::*)(wxCommandEvent&), DynamicLengthField<RouteFrame>*>(wxEventTypeTag<wxCommandEvent>, void (LengthField<RouteFrame>::*)(wxCommandEvent&), DynamicLengthField<RouteFrame>*);
template void MultipleItemField<ChartFrame, Projection, void>::Bind<wxEventTypeTag<wxKeyEvent>, void (ChartFrame::*)(wxKeyEvent&), ChartFrame*>(wxEventTypeTag<wxKeyEvent>, void (ChartFrame::*)(wxKeyEvent&), ChartFrame*);
template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (LengthField<RouteFrame>::*)(wxKeyEvent&), DynamicLengthField<RouteFrame>*>(wxEventTypeTag<wxKeyEvent>, void (LengthField<RouteFrame>::*)(wxKeyEvent&), DynamicLengthField<RouteFrame>*);
template  void MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (LengthFormatField<RouteFrame>::*)(wxKeyEvent&), LengthFormatField<RouteFrame>*>(wxEventTypeTag<wxKeyEvent>, void (LengthFormatField<RouteFrame>::*)(wxKeyEvent&), LengthFormatField<RouteFrame>*);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (LengthFormatField<RouteFrame>::*)(wxCommandEvent&), LengthFormatField<RouteFrame>*>(wxEventTypeTag<wxCommandEvent>, void (LengthFormatField<RouteFrame>::*)(wxCommandEvent&), LengthFormatField<RouteFrame>*);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (SpeedField<RouteFrame>::*)(wxKeyEvent&), SpeedField<RouteFrame>*>(wxEventTypeTag<wxKeyEvent>, void (SpeedField<RouteFrame>::*)(wxKeyEvent&), SpeedField<RouteFrame>*);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (SpeedField<RouteFrame>::*)(wxCommandEvent&), SpeedField<RouteFrame>*>(wxEventTypeTag<wxCommandEvent>, void (SpeedField<RouteFrame>::*)(wxCommandEvent&), SpeedField<RouteFrame>*);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*>(wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*);
template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*>(wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*);
template void MultipleItemField<SightFrame, LengthUnit, CheckLengthUnit<SightFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (LengthField<SightFrame>::*)(wxKeyEvent&), DynamicLengthField<SightFrame>*>(wxEventTypeTag<wxKeyEvent>, void (LengthField<SightFrame>::*)(wxKeyEvent&), DynamicLengthField<SightFrame>*);
template void MultipleItemField<SightFrame, LengthUnit, CheckLengthUnit<SightFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (LengthField<SightFrame>::*)(wxCommandEvent&), DynamicLengthField<SightFrame>*>(wxEventTypeTag<wxCommandEvent>, void (LengthField<SightFrame>::*)(wxCommandEvent&), DynamicLengthField<SightFrame>*);
template void MultipleItemField<SightFrame, Body, CheckBody<SightFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);
template void MultipleItemField<ChartFrame, LengthUnit, CheckLengthUnit<ChartFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (LengthField<ChartFrame>::*)(wxKeyEvent&), StaticLengthField<ChartFrame>*>(wxEventTypeTag<wxKeyEvent>, void (LengthField<ChartFrame>::*)(wxKeyEvent&), StaticLengthField<ChartFrame>*);
template void MultipleItemField<ChartFrame, LengthUnit, CheckLengthUnit<ChartFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (StaticLengthField<ChartFrame>::*)(wxKeyEvent&), StaticLengthField<ChartFrame>*>(wxEventTypeTag<wxKeyEvent>, void (StaticLengthField<ChartFrame>::*)(wxKeyEvent&), StaticLengthField<ChartFrame>*);
template void MultipleItemField<ChartFrame, LengthUnit, CheckLengthUnit<ChartFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (LengthField<ChartFrame>::*)(wxCommandEvent&), StaticLengthField<ChartFrame>*>(wxEventTypeTag<wxCommandEvent>, void (LengthField<ChartFrame>::*)(wxCommandEvent&), StaticLengthField<ChartFrame>*);
template void MultipleItemField<ChartFrame, LengthUnit, CheckLengthUnit<ChartFrame>>::Bind<wxEventTypeTag<wxCommandEvent>, void (StaticLengthField<ChartFrame>::*)(wxCommandEvent&), StaticLengthField<ChartFrame>*>(wxEventTypeTag<wxCommandEvent>, void (StaticLengthField<ChartFrame>::*)(wxCommandEvent&), StaticLengthField<ChartFrame>*);


//set a tooltip with text text to *this
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::SetToolTip(const String& text) {

    name->SetToolTip(wxString(text.value));

}

template void MultipleItemField<ChartFrame, Projection, void>::SetToolTip(String const&);


template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template void MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::InsertIn<wxBoxSizer>(wxBoxSizer*);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void MultipleItemField<SightFrame, Body, CheckBody<SightFrame>>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);


template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}

template void MultipleItemField<ChartFrame, Projection, void>::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);


//set the value in the GUI object name equal to the value in the non-GUI NON_GUI object 'object'
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::set(void) {
        
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue((MultipleItemField<P, NON_GUI, CHECK>::object->value));

    MultipleItemField<P, NON_GUI, CHECK>::ok = true;

}

template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::set();
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::set();
template void MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::set();
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::set();


template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::set(const NON_GUI& input) {
        
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue((input.value));

    MultipleItemField<P, NON_GUI, CHECK>::ok = true;

}

template void MultipleItemField<ChartFrame, LengthUnit, CheckLengthUnit<ChartFrame>>::set(LengthUnit const&);


//set the value in the non-GUI object 'object' equal to the value in the GUI object name, with no abbreviations used (the value is copied as it is)
 template<class P, class NON_GUI, class CHECK> template<class E> void MultipleItemField<P, NON_GUI, CHECK>::Get(E& event) {
        
     
     if (MultipleItemField<P, NON_GUI, CHECK>::ok) {
              object->set(String((MultipleItemField<P, NON_GUI, CHECK>::name->GetValue()).ToStdString()));
      }

      event.Skip(true);

}

template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Get<wxCommandEvent>(wxCommandEvent&);


// if the content of the GUI field *this  matches one of the items in catalog, write true in *is_present and write the number of the corresponding entry in catalog in *i. If not, write false in *is_present and catalog.size() in *i
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::CheckInCatalog(bool* is_present, unsigned int * i) {
    

    for ((*is_present) = false, (*i) = 0; ((*i)<catalog.size()) && (!(*is_present)); (*i)++) {
        if ((name->GetValue()) == catalog[(*i)]) {
            (*is_present) = true;
        }
    }
    
    if((*is_present)){
        (*i)--;
    }else{
        (*i) = ((unsigned int)(catalog.size()));
    }
    
}

template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::CheckInCatalog(bool*, unsigned int*);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::CheckInCatalog(bool*, unsigned int*);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::CheckInCatalog(bool*, unsigned int*);
template void MultipleItemField<SightFrame, LengthUnit, CheckLengthUnit<SightFrame>>::CheckInCatalog(bool*, unsigned int*);


//this method is called whenever the user kills the focus on the GUI field in order to check the content of the GUI field and do the necessary operations
template<class P, class NON_GUI, class CHECK> template<class E> void MultipleItemField<P, NON_GUI, CHECK>::Check(E& event) {


    //I proceed only if the progam is not is indling mode
    if (!(parent->idling)) {

        unsigned int i;
        bool is_present;
        
        CheckInCatalog(&is_present, &i);

        if (is_present || (((name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((name->GetValue()).ToStdString())) == String("")))) {
            //the GUI field  contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            if (is_present) {
                //the content of the GUI field matches one of the items in catalog, i.e., it is valid -> I insert it into recent_items, which points to a suitable location (initialized when *this was constructed)

                //insert item #i into data->recent_bodies
                wxGetApp().list_frame->data->insert_recent_item(i, recent_items);

                //I update p->name according to the content of recent_itmes
                Fill();

            }


            //if check is true (false) -> set ok to true (false)
            ok = is_present;
            //the background color is set to wxGetApp().foreground_color and the font to default_font, because in this case there is no erroneous value in name. I call Reset to reset the font colors of the items in the list to their default values
            name->SetForegroundColour(wxGetApp().foreground_color);
            name->SetFont(wxGetApp().default_font);
            Reset(name);
            
            //if the value written in name is correct, I store it in value_before_editing
            if(ok){value_before_editing = (name->GetValue());}

        }else{
            //the GUI field  does not contain a valid text,  it is not empty and with a red background color-> I prompt an error message frame

            stringstream temp;

            temp.str("");
            temp << "Item must be one of the following: ";
            for (i = 0; i < (catalog.GetCount()); i++) {
                temp << (catalog[i]).ToStdString() << (i < (catalog.GetCount()) - 1 ? ", " : ".");
            }

            parent->print_error_message->SetAndCall(name, String("Item not found in list of suitable items!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            ok = false;

        }

        if (!ok) {
            //the entered value is not valid: I set the value back to the value before the editing process had started
            name->SetValue(value_before_editing);
            ok = true;
            name->SetForegroundColour(wxGetApp().foreground_color);
            name->SetFont(wxGetApp().default_font);
            Reset(name);
        }
        parent->AllOk();

    }
    
    event.Skip(true);

}

template void MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::Check<wxFocusEvent>(wxFocusEvent&);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Check<wxFocusEvent>(wxFocusEvent&);
template void MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::Check<wxCommandEvent>(wxCommandEvent&);


//update the GUI dropdown menu of MultipleItemField according to MultipleItemField<P, NON_GUI>::recent_items in such a way that the recent items appear on top
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Fill(void) {

    unsigned int i, j;
    wxArrayString items_temp;
    wxString name_temp;
    bool is_present;

    //save the current value of name in name_temp
    name_temp = (MultipleItemField<P, NON_GUI, CHECK>::name->GetValue());
    //create the temporary list of projections projections_temp from catalog
    for (items_temp.Clear(), i = 0; i < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount(); i++) {
        items_temp.Add((MultipleItemField<P, NON_GUI, CHECK>::catalog)[i]);
    }

    //I first add to the items vector the recently selected items written in (MultipleItemField<P, NON_GUI>::recent_items)
    for (MultipleItemField<P, NON_GUI, CHECK>::items.Clear(), i = 0; i < (MultipleItemField<P, NON_GUI, CHECK>::recent_items->size()); i++) {

        MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[(*(MultipleItemField<P, NON_GUI, CHECK>::recent_items))[i]]);

    }

    //then, I fill the items vector with the remaining items
    for (i=0; i < items_temp.GetCount(); i++) {

        for (is_present = false, j = 0; (j < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount()) && (!is_present); j++) {

            if (MultipleItemField<P, NON_GUI, CHECK>::items[j] == items_temp[i]) {
                is_present = true;
            }

        }

        if (!is_present) {
            MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[i]);
        }

    }

    MultipleItemField<P, NON_GUI, CHECK>::name->Set(MultipleItemField<P, NON_GUI, CHECK>::items);
    //because name->Set(projections clears the value of name, I set the value of name back to name_temp
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue(name_temp);
    //given that I just filled name with a valid item, I store this item in value_before_editing
    value_before_editing = name->GetValue();
    
    
    items_temp.Clear();

}

//return true(false) is *this is ok (not ok), i.e., if this->ok = true(false)
template<class P, class NON_GUI, class CHECK> bool MultipleItemField<P, NON_GUI, CHECK>::is_ok(void) {

    return(ok);

}

template bool MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::is_ok();
template bool MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame>>::is_ok();
template bool MultipleItemField<SightFrame, Body, CheckBody<SightFrame>>::is_ok();


//this function enables/disable the LengthFormatField
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Enable(bool is_enabled) {

    name->Enable(is_enabled);

}

template void MultipleItemField<ChartFrame, Projection, void>::Enable(bool);
template void MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame>>::Enable(bool);
template void MultipleItemField<RouteFrame, LengthUnit, CheckLengthUnit<RouteFrame>>::Enable(bool);
template void MultipleItemField<RouteFrame, SpeedUnit, CheckLengthUnit<RouteFrame>>::Enable(bool);
