// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoBird.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "UObject/Object.h"
#include "KoBirdPerceptionStimuliComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStimuliSensed, const AKoBird*, Bird, const bool, SuccessfullySensed);

//Class for the perception stimuli component of the bird. This is placed on actors to make them sensible to the perception component.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdPerceptionStimuliComponent : public UAIPerceptionStimuliSourceComponent
{
	GENERATED_BODY()
	
	void NotifyStimuliSensed(const AKoBird* Bird, const bool& SuccessfullySensed) const;
	
protected:
	UPROPERTY(BlueprintAssignable, Category="Stimuli")
	FOnStimuliSensed OnStimuliSensed;
		
public:
	friend class UKoBirdPerceptionComponent;
	
	UKoBirdPerceptionStimuliComponent(const FObjectInitializer& ObjectInitializer);
	
};
