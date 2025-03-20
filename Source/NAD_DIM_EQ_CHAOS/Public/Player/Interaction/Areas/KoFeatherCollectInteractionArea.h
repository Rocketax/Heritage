// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Interaction/KoInteractionArea.h"
#include "KoFeatherCollectInteractionArea.generated.h"

//Class for the pickup area of the bird's feather.
UCLASS()
class NAD_DIM_EQ_CHAOS_API AKoFeatherCollectInteractionArea : public AKoInteractionArea
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class AKoBird* BirdReference;
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category="Feather")
	void OnFeatherCollected(bool Succeed);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:

	virtual void HandleArea(APawn* Pawn) override;

	UPROPERTY(EditAnywhere, Category="interaction")
	UObject* root;
	
};
