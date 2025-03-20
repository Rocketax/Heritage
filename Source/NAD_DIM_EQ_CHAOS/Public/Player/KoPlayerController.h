// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player/Inputs/KoPlayerInputActionRegistry.h"
#include "InputMappingContext.h"
#include "KoPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaceSeedUnlocked);

//Class for the player controller. It handles all the inputs of the player.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION()
	void OnPauseInput(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnMoveInput(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnAimInput(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnInteractInput(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnPlaceSeedInput(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnCrouchInput(const FInputActionInstance& Instance);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inputs")
	bool CanCapture = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inputs")
	bool CanPlaceSeed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inputs")
	bool IsCrouched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inputs")
	UKoPlayerInputActionRegistry* InputActionRegistry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inputs")
	UInputMappingContext* InputMappingContext;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Seed")
	bool PlaceSeedUnlocked = false;

	UPROPERTY(BlueprintAssignable, Category="Seed")
	FOnPlaceSeedUnlocked OnPlaceSeedUnlocked;

	UFUNCTION(BlueprintCallable, Category="Seed")
	void UnlockPlaceSeed(int32 Zone);
	
	AKoPlayerController();

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Controller")
	void OnPause(bool IsGamePaused);

	UFUNCTION(BlueprintNativeEvent, Category="Controller")
	void OnAim(const FVector Direction);

	UFUNCTION(BlueprintNativeEvent, Category="Controller")
	void OnCapture();

	UFUNCTION(BlueprintNativeEvent, Category="Controller")
	void OnMove(const FVector Direction);

	UFUNCTION(BlueprintNativeEvent, Category="Controller")
	void OnPlaceSeed();

	UFUNCTION(BlueprintCallable, Category="Controller")
	void PlaceSeed();

	UFUNCTION(BlueprintCallable, Category="Controller")
	void CaptureBird();
};
