// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "KoBirdPerceptionComponent.generated.h"

class AKoBird;

//Class for the bird's perception component. It is used to detect the player. It includes sight and hearing.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NAD_DIM_EQ_CHAOS_API UKoBirdPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus);

	float DetectedForce;

	float UndetectedTime;

	bool IsPlayerHeard;
	
	bool IsPlayerSeen;

	FAISenseID StimulusType;

	FTimerHandle TimerHandle;

	float StimulusStrength = 0.f;

	bool ShouldRunAway = false;

public:
	UKoBirdPerceptionComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AdjustDetection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detection")
	float DetectionThreshold = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detection")
	float UndetectionThreshold = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detection")
	float SightAmplifier = 3.f;
	
};
