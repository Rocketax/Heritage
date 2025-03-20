// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KoInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractWithCurrentArea);


class AKoInteractionArea;

//Class for the interaction component. It is used to interact with birds and complete orders.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NAD_DIM_EQ_CHAOS_API UKoInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKoInteractionComponent();

	UFUNCTION(BlueprintCallable, Category="Interaction")
	APawn* GetPawn() const;

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractWithCurrentArea OnInteractWithCurrentArea;

	UPROPERTY()
	TArray<AKoInteractionArea*> CurrentInteractionAreas;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool TryInteractWithCurrentArea();
	
	void AddInteractionArea(AKoInteractionArea* InteractionArea);

	void RemoveInteractionArea(AKoInteractionArea* InteractionArea);
};
