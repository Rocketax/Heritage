// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interaction/Areas/KoFeatherDepositInteractionArea.h"

#include "Player/KoPlayerCollector.h"

//This function handles the interaction with the area. In this case, it handles the deposit of the feathers.
void AKoFeatherDepositInteractionArea::HandleArea(APawn* Pawn)
{
	Super::HandleArea(Pawn);
	
	if(auto* Collector = Pawn->FindComponentByClass<UKoPlayerCollector>())
	{
		Collector->DepositFeathers();
	}
}
