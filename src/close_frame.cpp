//
//  close_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "close_frame.h"

#include "all_routes.h"
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
template class CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<PositionFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<DrawPanel>>>;
template class CloseFrame<QuestionFrame<ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<ShowAll, CloseApp, CloseApp>>;
template class CloseFrame<QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<ChartFrame>>>;
template class CloseFrame<QuestionFrame<DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>;
template class CloseFrame<QuestionFrame<ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>;
template class CloseFrame<QuestionFrame<CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;


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
template void CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<PositionFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<PositionFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<DrawPanel>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<QuestionFrame<ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<DrawPanel>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ShowAll, CloseApp, CloseApp>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<ChartFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<ChartFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<QuestionFrame<CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
