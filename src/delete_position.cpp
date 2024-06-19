//
//  delete_position.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "delete_position.h"

#include "on_change_selection_in_list_control.h"


DeletePosition::DeletePosition(ListFrame* f_in) {

    f = f_in;

}

//delete the  selected position in the GUI object f->listcontrol_position and in the non-GUI object f->data. If no position is selected, it does nothing.
void DeletePosition::operator()(wxCommandEvent& event) {

    long i;

    i = (f->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (i != -1) {

        (f->data)->remove_position(((unsigned int)i), String(""));
        (f->listcontrol_positions)->set((f->data)->position_list, false);

        //given that I called set in listcontrol_positions, now no item is selected in listcontrol_positions-> I call:
        (*(f->on_change_selection_in_listcontrol_positions))(event);
        f->Resize();
        f->OnModifyFile();

    }

    event.Skip(true);

}
