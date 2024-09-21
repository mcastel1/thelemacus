//
//  ask_remove_related_route.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "ask_remove_related_route.h"

#include <wx/listctrl.h>

#include "delete_route.h"
#include "delete_sight.h"
#include "show_question_frame.h"



AskRemoveRelatedRoute::AskRemoveRelatedRoute(ListFrame* parent_in) {

    parent = parent_in;

}



void AskRemoveRelatedRoute::operator()(wxCommandEvent& event) {

    int i_sight_to_remove;

    //set i_sight_to_remove equal to the currently relected Sight in listcontrol_sights
    i_sight_to_remove = ((int)(parent->listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    (parent->delete_sight->i_sight_to_remove) = i_sight_to_remove;


    if (((((parent->data->sight_list)[i_sight_to_remove]).related_route).value) != -1) {
        //if the sight which I am about to remove is related to a Route, I ask the user whether he wants to remove the related Route too by showing  question_frame

        ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>* print_question;

        (parent->delete_route_and_related_sight->i_route_to_remove) = (((parent->data->sight_list)[i_sight_to_remove]).related_route.value);

        //remove the route from the non-GUI object data
        //ask the user whether he/she wants to remove the related sight as well: if the answer is yes, then QuestionFrame calls the functor delete_sight_and_related_sight. If no, it calls the functor delete_sight.

        print_question = new ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>(parent, parent->delete_route_and_related_sight, parent->delete_sight, parent->unset_idling);

        print_question->SetAndCall(NULL, String(""), String("Do you want to remove the route related to this sight??"), String("Yes"), String("No"));


    }
    else {
        //if not, I simply delete teh sight

        (*(parent->delete_sight))(event);

    }

    parent->OnModifyFile();

    event.Skip(true);

}
