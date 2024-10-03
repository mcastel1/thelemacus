//
//  destroy_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "destroy_frame.h"

#include "all_routes.h"
#include "animate_to_object.h"
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
template class DestroyFrame<MessageFrame<ListFrame, UnsetIdling<ListFrame>>>;
template class DestroyFrame<MessageFrame<RouteFrame, UnsetIdling<RouteFrame>>>;
template class DestroyFrame<MessageFrame<SightFrame, UnsetIdling<SightFrame>>>;
template class DestroyFrame<MessageFrame<PositionFrame, UnsetIdling<PositionFrame>>>;
template class DestroyFrame<MessageFrame<DrawPanel, UnsetIdling<DrawPanel>>>;
template class DestroyFrame<MessageFrame<ChartFrame, UnsetIdling<ChartFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>;
template class DestroyFrame<QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>;
template class DestroyFrame<QuestionFrame<MyApp, ShowAll, CloseApp, CloseApp>>;


//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}

template void DestroyFrame<ListFrame>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<QuestionFrame<MyApp, ShowAll, CloseApp, CloseApp>>::operator()();
