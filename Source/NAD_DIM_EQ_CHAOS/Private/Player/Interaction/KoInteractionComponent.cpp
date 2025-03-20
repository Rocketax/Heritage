// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interaction/KoInteractionComponent.h"

#include "Player/KoPlayerController.h"
#include "Player/Interaction/KoInteractionArea.h"

UKoInteractionComponent::UKoInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

APawn* UKoInteractionComponent::GetPawn() const
{
	return Cast<APawn>(GetOwner());
}

//This function tries to interact with the area the player is overlapping with. It then returns a bool indicating success.
bool UKoInteractionComponent::TryInteractWithCurrentArea()
{
	if (CurrentInteractionAreas.Num() == 0) return false;

	CurrentInteractionAreas[0]->HandleArea(GetPawn());

	//The player isn't overlapping with any interaction area.
	if (CurrentInteractionAreas.Num() == 0)
	{
		Cast<AKoPlayerController>(GetPawn()->Controller)->CanCapture = false;
	}

	OnInteractWithCurrentArea.Broadcast();

	return true;
}


void UKoInteractionComponent::AddInteractionArea(AKoInteractionArea* InteractionArea)
{
	CurrentInteractionAreas.Push(InteractionArea);
	
}

void UKoInteractionComponent::RemoveInteractionArea(AKoInteractionArea* InteractionArea)
{
	CurrentInteractionAreas.Remove(InteractionArea);
}
