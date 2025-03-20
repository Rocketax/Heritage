// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KoPlayerState.generated.h"

//Class for the player state. We store data related to the feathers here.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Feathers")
	int MaxFeathersToCollect;
	
	void IncreaseFeathers(int32 ByValue);

protected:
	
	UFUNCTION(BlueprintNativeEvent, Category="Score")
	void OnFeathersChanged();
	
};
