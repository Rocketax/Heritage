// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "KoBirdMovementComponent.generated.h"

//Class for the bird's movement component.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NAD_DIM_EQ_CHAOS_API UKoBirdMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKoBirdMovementComponent();
};
