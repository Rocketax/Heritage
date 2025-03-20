// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "KoPlayerCharacter.generated.h"

class UKoInteractionComponent;
class UKoPlayerCollector;

class UKoBirdPerceptionStimuliComponent;

//Class for the player character.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoPlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	FGenericTeamId TeamId;

public:
	AKoPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UKoPlayerCollector* PlayerCollector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UKoInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UKoBirdPerceptionStimuliComponent* BirdPerceptionStimuliComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* Camera;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, Category="Camera")
	float CameraSpeedX = 1.f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float CameraSpeedY = 1.f; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	float InitialCameraSpringArmLength;

	UPROPERTY(EditDefaultsOnly, Category = "Seed")
	TSubclassOf<AActor> Seed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Seed")
	float SeedCooldown = 1.f;

	
protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FGenericTeamId GetGenericTeamId() const override;
};
