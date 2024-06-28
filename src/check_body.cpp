//
//  check_body.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_body.h"
#include "sight_frame.h"
#include "my_app.h"

template<class P> CheckBody<P>::CheckBody(BodyField<P>* p_in) {

    p = p_in;

}

template class CheckBody<SightFrame>;


template<class P> template<class T>void CheckBody<P>::operator()(T& event) {

    SightFrame* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        //I check whether the name in the GUI field body matches one of the body names in catalog
        for (check = false, i = 0; (i < (p->catalog->list).size()) && (!check); i++) {
            if (String((p->name->GetValue().ToStdString())) == (((p->catalog->list)[i]).name)) {
                check = true;
            }
        }
        i--;

        if (check || (((p->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->name->GetValue()).ToStdString())) == String("")))) {
            //p->check either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            if (check) {

                if (((p->catalog->list)[i].name == String("sun")) || ((p->catalog->list)[i].name == String("moon"))) {
                    //in this case, the selected body is a body which has a limb -> I enable the limb field

                    f->limb->name->Enable(true);

                }
                else {
                    //in this case, the selected body is a body which has no limb -> I disable the limb field and set limb->ok to true (because the limb is unumportant here, so it can be considered to be ok)

                    f->limb->name->Enable(false);
                    (f->limb->ok) = true;

                }

                //insert body #i into data->recent_bodies
//                wxGetApp().list_frame->data->insert_recent_body(i);
                wxGetApp().list_frame->data->insert_recent_item(i, &(wxGetApp().list_frame->data->recent_bodies));
                //I update p->name according to the content of data->recent_bodies file
                p->Fill();

            }

            //if check is true (false) -> set ok to true (false)
            (p->ok) = check;
            //the background color is set to wxGetApp().foreground_color and the font to default_font, because in this case there is no erroneous value in name. I call Reset to reset the font colors of the items in the list to their default values
            p->name->SetForegroundColour(wxGetApp().foreground_color);
            p->name->SetFont(wxGetApp().default_font);
            Reset(p->name);

        }
        else {

            f->print_error_message->SetAndCall(p->name, String("Error"), String("Body not found in catalog! Body must be in catalog."), (wxGetApp().path_file_error_icon));

            (p->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckBody<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckBody<SightFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckBody<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
