// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KoGameState.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOrderCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterStore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewOrdersObtained);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllOrdersCompleted, int32, Zone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOrdersGenerated, int32, NumOfOrders, int32, Zone);

class UKoFeatherOrder;
class AKoBird;
enum class FeatherColor : uint8;

//Class for the game state. It handles a lot of the game logic such as orders, feathers and game completion.
UCLASS(Abstract)
class NAD_DIM_EQ_CHAOS_API AKoGameState : public AGameStateBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UFUNCTION()
	void GenerateOrders(int32 Zone);

public:

	UPROPERTY(BlueprintReadOnly, Category="Zone")
	int32 CurrentZone = 1;
	
	UPROPERTY(BlueprintReadOnly, Category="Order")
	TArray<UKoFeatherOrder*> FirstOrderList;
	
	UPROPERTY(BlueprintReadOnly, Category="Order")
	TArray<UKoFeatherOrder*> SecondOrderList;
	
	UPROPERTY(BlueprintReadOnly, Category="Order")
	TArray<UKoFeatherOrder*> OrderList;

	UPROPERTY(BlueprintReadWrite, Category="Order")
	int32 BirdCount;

	UPROPERTY(BlueprintReadWrite, Category="Order")
	TArray<FeatherColor> FirstFeatherList;
	
	UPROPERTY(BlueprintReadWrite, Category="Order")
	TArray<FeatherColor> SecondFeatherList;

	UPROPERTY(BlueprintReadOnly, Category="Order")
	TArray<UKoFeatherOrder*> ActiveOrders;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Order")
	int MaxActiveOrders = 3;

	UPROPERTY(BlueprintAssignable, Category="Order")
	FOnOrderCompleted OnOrderCompleted;
	
	UPROPERTY(BlueprintAssignable, Category="Order")
	FOnOrdersGenerated OnOrdersGenerated;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Order")
	FOnEnterStore OnEnterStore;

	UPROPERTY(BlueprintAssignable, Category="Order")
	FOnAllOrdersCompleted OnAllOrdersCompleted;

	UPROPERTY(BlueprintAssignable, Category="Order")
	FOnNewOrdersObtained OnNewOrdersObtained;
	
	UPROPERTY(BlueprintReadWrite, Category="Order")
	TArray<AActor*> BirdsFirstZone;
	
	UPROPERTY(BlueprintReadWrite, Category="Order")
	TArray<AActor*> BirdsSecondZone;

	UPROPERTY(BlueprintReadWrite, Category="Order")
	int32 FirstZoneOrderLimit = 999;

	UPROPERTY(BlueprintReadWrite, Category="Order")
	int32 SecondZoneOrderLimit = 3;
	
	UFUNCTION(BlueprintCallable, Category="Order")
	void AddBird();

	UFUNCTION(BlueprintCallable, Category="Order")
	void AddActiveOrder(UKoFeatherOrder* Order);

	UFUNCTION(BlueprintCallable, Category="Order")
	void AddRandomActiveOrder(int Amount = 1);

	UFUNCTION(BlueprintCallable, Category="Order")
	void BirdsQueried(int32 Zone);

	UFUNCTION()
	TArray<FString> GetAllClientsImages() const;

	UFUNCTION()
	TArray<FString> GetAllPaperImages() const;

	UFUNCTION()
	void GetNewOrders();

	UFUNCTION()
	void OrderCompleted();
};
