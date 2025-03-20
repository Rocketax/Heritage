// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoBird.generated.h"

class UCapsuleComponent;

//Enum for feather color.
UENUM(BlueprintType)
enum class FeatherColor : uint8 {
	Red = 0 UMETA(DisplayName = "Red"),
	Blue = 1 UMETA(DisplayName = "Blue"),
	Yellow = 2 UMETA(DisplayName = "Yellow")
};

class UKoBirdMovementComponent;

//Class for the bird.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoBird : public APawn
{
	GENERATED_BODY()

	FTimerHandle TimerHandle;
	
public:

	AKoBird(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UKoBirdMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Flee")
	float MaxFleeingDistance = 3000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Flee")
	float FleeingDetectionArc = 15;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Guide")
	TArray<AActor*> GuidePathDestinations;

	UFUNCTION(BlueprintNativeEvent, Category="Capture")
	void OnCaptured();
	
	UFUNCTION(BlueprintNativeEvent, Category="Detection")
	void OnDetectionChanged(float Detection);

	UFUNCTION(BlueprintNativeEvent, Category="Guide")
	void OnFlyingAway();

	UFUNCTION()
	void ResetAddPoint();

	UPROPERTY()
	TArray<FVector> Path;

	UPROPERTY()
	int32 PathStep;
	
	UPROPERTY()
	FVector LastValidDirection;

	UPROPERTY()
	FVector StartingFleePosition;
	
	UPROPERTY()
	float TimeoutTimer = 0;
	
	const float TimeoutTime = 1;

	UPROPERTY()
	float BirdHalfHeight = 40;

	UPROPERTY()
	bool CanAddPoint = true;

	UPROPERTY()
	float PointTimeoutTimer = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wandering")
	bool IsWandering = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Flee")
	bool IsLanding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Seed")
	bool IsEating = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Feather")
	FeatherColor FeatherColor;

	UPROPERTY()
	bool NeedsNewPoint = false;
	
protected:
	virtual void BeginPlay() override;
};
