//
//  destroy_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "destroy_frame.h"

#include "all_routes.h"
#include "list_frame.h"
#include "position_frame.h"
#include "question_frame.h"
#include "route_frame.h"
#include "reset_list_frame.h"
#include "save_and_reset.h"
#include "sight_frame.h"
#include "some_routes.h"


template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

template class DestroyFrame<ListFrame>;
template class DestroyFrame<MessageFrame<UnsetIdling<ListFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<RouteFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<SightFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<PositionFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<DrawPanel>>>;
template class DestroyFrame<QuestionFrame<ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ShowAll, CloseApp, ShowAll>>;
template class DestroyFrame<QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<ChartFrame>>>;
template class DestroyFrame<QuestionFrame<DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>;
template class DestroyFrame<QuestionFrame<ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;



//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}

template void DestroyFrame<ListFrame>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<PositionFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<DrawPanel>>>::operator()();
template void DestroyFrame<QuestionFrame<ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ShowAll, CloseApp, ShowAll>>::operator()();
template void DestroyFrame<QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<ChartFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>::operator()();
template void DestroyFrame<QuestionFrame<ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();