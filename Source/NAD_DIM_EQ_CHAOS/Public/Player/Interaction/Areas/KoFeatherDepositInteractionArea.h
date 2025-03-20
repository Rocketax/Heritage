// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Interaction/KoInteractionArea.h"
#include "KoFeatherDepositInteractionArea.generated.h"

//Class for feather deposit interaction area. It is placed where the orders are and opens the order menu when entered.
UCLASS()
class NAD_DIM_EQ_CHAOS_API AKoFeatherDepositInteractionArea : public AKoInteractionArea
{
	GENERATED_BODY()

	virtual void HandleArea(APawn* Pawn) override;
};
