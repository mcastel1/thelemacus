//
//  multiple_item_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "multiple_item_field.h"



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


template<class P, class NON_GUI, class CHECK> template <typename EventTag, typename Method, typename Object> void MultipleItemField<P, NON_GUI, CHECK>::Bind(EventTag tag, Method method, Object object) {

    name->Bind(tag, method, object);

}


//set a tooltip with text text to *this
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::SetToolTip(const String& text) {

    name->SetToolTip(wxString(text.value));

}




template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}


//set the value in the GUI object name equal to the value in the non-GUI NON_GUI object 'object'
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::set(void) {
        
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue((MultipleItemField<P, NON_GUI, CHECK>::object->value));

    MultipleItemField<P, NON_GUI, CHECK>::ok = true;

}


template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::set(const NON_GUI& input) {
        
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue((input.value));

    MultipleItemField<P, NON_GUI, CHECK>::ok = true;

}


//set the value in the non-GUI object 'object' equal to the value in the GUI object name, with no abbreviations used (the value is copied as it is)
 template<class P, class NON_GUI, class CHECK> template<class E> void MultipleItemField<P, NON_GUI, CHECK>::Get(E& event) {
        
     
     if (MultipleItemField<P, NON_GUI, CHECK>::ok) {
              object->set(String((MultipleItemField<P, NON_GUI, CHECK>::name->GetValue()).ToStdString()));
      }

      event.Skip(true);

}


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
