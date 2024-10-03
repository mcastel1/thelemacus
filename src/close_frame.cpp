//
//  close_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "close_frame.h"

#include "all_routes.h"
#include "animate_to_object.h"
#include "destroy_frame.h"
#include "list_frame.h"
#include "position_frame.h"
#include "question_frame.h"
#include "reset_list_frame.h"
#include "save_and_reset.h"
#include "some_routes.h"
#include "sight_frame.h"


template<class F> CloseFrame<F>::CloseFrame(F* frame_in) {

    frame = frame_in;

}

//explicit instantiations
template class CloseFrame<ListFrame>;
template class CloseFrame<QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>;
template class CloseFrame<QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<MyApp, ShowAll, CloseApp, CloseApp>>;


//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()([[maybe_unused]] T& event) {

    DestroyFrame<F>* destroy_frame;

    destroy_frame = new DestroyFrame<F>(frame);

    //destroys frame
    frame->CallAfter(*destroy_frame);

    event.Skip(true);

}

//explicit instantiations
template void CloseFrame<ListFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<ListFrame, UnsetIdling<ListFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<ListFrame, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<RouteFrame, UnsetIdling<RouteFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<RouteFrame, UnsetIdling<RouteFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<SightFrame, UnsetIdling<SightFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<SightFrame, UnsetIdling<SightFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<PositionFrame, UnsetIdling<PositionFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<PositionFrame, UnsetIdling<PositionFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<DrawPanel, UnsetIdling<DrawPanel>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<DrawPanel, UnsetIdling<DrawPanel>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<ChartFrame, UnsetIdling<ChartFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<ChartFrame, UnsetIdling<ChartFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<MyApp, ShowAll, CloseApp, CloseApp>>::operator()<wxCommandEvent>(wxCommandEvent&);
