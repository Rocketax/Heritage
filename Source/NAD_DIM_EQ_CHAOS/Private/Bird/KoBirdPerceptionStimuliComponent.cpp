// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoBirdPerceptionStimuliComponent.h"

void UKoBirdPerceptionStimuliComponent::NotifyStimuliSensed(const AKoBird* Bird, const bool& SuccessfullySensed) const
{
	OnStimuliSensed.Broadcast(Bird, SuccessfullySensed);
}

UKoBirdPerceptionStimuliComponent::UKoBirdPerceptionStimuliComponent(const FObjectInitializer& ObjectInitializer)
	: UAIPerceptionStimuliSourceComponent(ObjectInitializer)
{
	bAutoRegisterAsSource = true;
}
