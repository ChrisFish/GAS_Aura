// Copyright Downwind Cloud Software, LLC


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	//base class blank implementation. SUPER not called. Code here won't run.
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
