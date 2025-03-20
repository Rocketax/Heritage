// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KoPlayerCollector.h"

#include "Player/KoPlayerState.h"

UKoPlayerCollector::UKoPlayerCollector()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Adding the possible feather colors to the inventory (each color is essentially a slot).
	Inventory.Add(FeatherColor::Red ,0);
	Inventory.Add(FeatherColor::Blue ,0);
	Inventory.Add(FeatherColor::Yellow ,0);
}

int32 UKoPlayerCollector::GetFeathersCarried() const
{
	return FeathersCarried;
}

//This function tries to collect the feather. If the player isn't crouched, it fails.
bool UKoPlayerCollector::TryCollectFeather()
{
	if(IsCrouched)
	{
		FeathersCarried += 1;

		OnFeatherCollected.Broadcast(FeathersCarried);
		
		return true;
	}
	return false;
}

//This function is used to deposit feathers to complete orders.
void UKoPlayerCollector::DepositFeathers()
{
	if(const auto* Pawn = Cast<APawn>(GetOwner()))
	{
		auto* PlayerState = Pawn->GetPlayerStateChecked<AKoPlayerState>();
		
		PlayerState->IncreaseFeathers(GetFeathersCarried());

		FeathersCarried = 0;
	}
}

