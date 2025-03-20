// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KoSeed.generated.h"

UCLASS()
class NAD_DIM_EQ_CHAOS_API AKoSeed : public AActor
{
	GENERATED_BODY()

public:
	AKoSeed();

	UFUNCTION(BlueprintCallable, Category="Seed")
	void StartSelfDestructTimer();

	UFUNCTION()
	void SelfDestruct();

	UPROPERTY(BlueprintReadOnly, Category="Seed")
	bool IsTimerStarted = false;
};
