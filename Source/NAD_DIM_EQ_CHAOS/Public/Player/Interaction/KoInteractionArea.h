// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KoInteractionArea.generated.h"

class USphereComponent;
struct FInputActionInstance;

//Base class for the interaction areas.
UCLASS()
class NAD_DIM_EQ_CHAOS_API AKoInteractionArea : public AActor
{
	GENERATED_BODY()

public:
	AKoInteractionArea();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Area")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Area")
	UDecalComponent* DecalComponent;
	
	virtual void HandleArea(APawn* Pawn);

protected:

	UFUNCTION(BlueprintNativeEvent, Category="Area")
	void OnEnterArea(APawn* Pawn);

	UFUNCTION(BlueprintNativeEvent, Category="Area")
	void OnLeaveArea(APawn* Pawn);

	UFUNCTION(BlueprintNativeEvent, Category="Area")
	void OnHandleArea(APawn* Pawn, bool Success);
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
