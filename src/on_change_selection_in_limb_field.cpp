//
//  on_change_selection_in_limb_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "on_change_selection_in_limb_field.h"



template<class P> OnChangeSelectionInLimbField<P>::OnChangeSelectionInLimbField(LimbField<P>* caller_in) {

    caller = caller_in;

}

//when an item is selected/deselcted in *caller, make sure that only one item stays selected in *calller
template<class P> template<class T>void OnChangeSelectionInLimbField<P>::operator()(T& event) {

    wxArrayInt temp;
    long i, j;

    temp.Clear();
    caller->name->GetCheckedItems(temp);


    if ((temp.GetCount()) <= 1) {
        //only one item is selected -> write temp into caller->selected_items

        caller->checked_items.Clear();
        (caller->checked_items) = temp;

    }
    else {
        //multiple items are selecte -> check for the newly selected item, and write it as the only element  into checked_items

        for (i = 0; i < temp.GetCount(); i++) {

            for (j = 0; (j < (caller->checked_items).GetCount()) && ((caller->checked_items.Item(j)) != (temp.Item(i))); j++) {}

            if (j == (caller->checked_items).GetCount()) {
                //the item temp.Item(i) is not present in (caller->checked_items) -> resize caller->checked_items to size one and write into its only element temp.Item(i)

                caller->checked_items.Clear();
                caller->checked_items.Add(temp.Item(i), 1);

                break;

            }
        }

    }

    //update the checked items in caller->name
    if ((caller->checked_items.GetCount()) == 0) {
        //checked_items is empty->uncheck everythig in name

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), false);
        }

    }
    else {
        //checked_items is not empty->check, in name, only the first element in checked_items

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), (i == (caller->checked_items.Item(0))));
        }

    }



    (caller->ok) = ((caller->checked_items.GetCount()) == 1);

    if (caller->ok) {

        caller->name->SetForegroundColour(wxGetApp().foreground_color);
        caller->name->SetFont(wxGetApp().default_font);

    }

    //tries to enable button_reduce
    caller->parent_frame->AllOk();

    event.Skip(true);

}

