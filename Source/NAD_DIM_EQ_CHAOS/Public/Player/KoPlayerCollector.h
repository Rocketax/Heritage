// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bird/KoBird.h"
#include "Components/ActorComponent.h"
#include "KoPlayerCollector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFeatherCarryLimitReached);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeatherCollected, const int32&, Feathers);

//Class for the component that handles the feather collection.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NAD_DIM_EQ_CHAOS_API UKoPlayerCollector : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintGetter=GetFeathersCarried)
	int32 FeathersCarried = 0;

public:
	UKoPlayerCollector();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Carry", meta=(ClampMin=1))
	int32 FeatherCarryLimit = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Carry")
	bool IsCrouched = false;

	UPROPERTY(BlueprintReadOnly, Category="Carry")
	TMap<FeatherColor, int> Inventory;
	
	UFUNCTION(BlueprintCallable, Category="Carry")
	bool TryCollectFeather();

	UFUNCTION(BlueprintCallable, Category="Carry")
	void DepositFeathers();

	UFUNCTION(BlueprintGetter, Category="Carry")
	int32 GetFeathersCarried() const;

protected:
	UPROPERTY(BlueprintAssignable, Category="Carry")
	FOnFeatherCollected OnFeatherCollected;
};
