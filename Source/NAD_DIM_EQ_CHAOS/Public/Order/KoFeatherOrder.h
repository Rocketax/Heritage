// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bird/KoBird.h"
#include "Player/KoPlayerCharacter.h"
#include "KoFeatherOrder.generated.h"

//This class is for the completable orders in the game. It stores the visuals and the requirements.
UCLASS(Blueprintable)
class NAD_DIM_EQ_CHAOS_API UKoFeatherOrder : public UObject
{
	GENERATED_BODY()
public:

	UKoFeatherOrder();

	UPROPERTY(BlueprintReadOnly, Category="Order")
	TMap<FeatherColor, int> FeathersRequired;

	UPROPERTY(BlueprintReadOnly, Category="Order")
	FString ClientImage;

	UPROPERTY(BlueprintReadOnly, Category="Order")
	FString PaperImage;
	
	UFUNCTION(BlueprintCallable)
	void CompleteOrder(AKoPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	bool IsOrderCompletable(AKoPlayerCharacter* PlayerCharacter);
};
