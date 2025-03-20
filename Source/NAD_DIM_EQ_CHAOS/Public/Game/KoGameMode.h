// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KoGameMode.generated.h"

//Class for base gamemode. Handles level loading.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Levels")
	FName ShopLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Levels")
	FName HuntLevel;

	UFUNCTION(BlueprintCallable, Category="Levels")
	void LoadShopLevel();

	UFUNCTION(BlueprintCallable, Category="Levels")	
	void LoadHuntLevel();
};
