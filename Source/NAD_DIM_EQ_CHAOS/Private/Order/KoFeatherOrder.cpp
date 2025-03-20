// Fill out your copyright notice in the Description page of Project Settings.


#include "Order/KoFeatherOrder.h"

#include "Engine/ObjectLibrary.h"
#include "Game/KoGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/KoPlayerCharacter.h"
#include "Player/KoPlayerCollector.h"
#include "Player/KoPlayerController.h"


UKoFeatherOrder::UKoFeatherOrder()
{
	//We create a map that contains all the possible colors with their matching required amount.
	this->FeathersRequired = TMap<FeatherColor, int>();
		
	this->FeathersRequired.Add(FeatherColor::Red, 0);
	this->FeathersRequired.Add(FeatherColor::Blue, 0);
	this->FeathersRequired.Add(FeatherColor::Yellow, 0);
}

//This function takes the required feathers from the player's inventory and completes the order.
void UKoFeatherOrder::CompleteOrder(AKoPlayerCharacter* PlayerCharacter)
{
	//The player doesn't have the required feathers.
	if (!IsOrderCompletable(PlayerCharacter)) return;
	
	UKoPlayerCollector* PlayerCollector = Cast<UKoPlayerCollector>(PlayerCharacter->GetComponentByClass(UKoPlayerCollector::StaticClass())); 
	
	check(PlayerCollector)

	//Remove the feathers from the player's inventory
	for(const auto FeatherRequired : FeathersRequired)
	{
		*PlayerCollector->Inventory.Find(FeatherRequired.Key) -= FeatherRequired.Value;
	}
	
	AKoGameState* GameState = Cast<AKoGameState>(PlayerCharacter->GetWorld()->GetGameState());

	//Removing the completed order
	const int Index = GameState->ActiveOrders.Find(this);
	GameState->ActiveOrders.RemoveAt(Index);

	//Top off the active orders after completing an order.
	GameState->AddRandomActiveOrder(1);

	GameState->OnOrderCompleted.Broadcast();

	//There are no more active orders. All orders are completed.
	if (GameState->ActiveOrders.Num() < 1)
	{
		GameState->OnAllOrdersCompleted.Broadcast(GameState->CurrentZone);
		GameState->CurrentZone++;
	}
}

//This function checks if the player has all the required feathers for the order.
bool UKoFeatherOrder::IsOrderCompletable(AKoPlayerCharacter* PlayerCharacter)
{
	UKoPlayerCollector* PlayerCollector = Cast<UKoPlayerCollector>(PlayerCharacter->GetComponentByClass(UKoPlayerCollector::StaticClass()));
	
	check(PlayerCollector)

	return *PlayerCollector->Inventory.Find(FeatherColor::Red) >= *FeathersRequired.Find(FeatherColor::Red) &&
		*PlayerCollector->Inventory.Find(FeatherColor::Blue) >= *FeathersRequired.Find(FeatherColor::Blue) &&
		*PlayerCollector->Inventory.Find(FeatherColor::Yellow) >= *FeathersRequired.Find(FeatherColor::Yellow);
}
