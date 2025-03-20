// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "KoPlayerInputActionRegistry.generated.h"

//Class to store input actions.
UCLASS(BlueprintType)
class NAD_DIM_EQ_CHAOS_API UKoPlayerInputActionRegistry : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* AimInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* MoveInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* PauseInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* InteractInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* PlaceSeedInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inputs")
	UInputAction* CrouchInput;
};
