// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interaction/KoInteractionArea.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Player/KoPlayerCharacter.h"
#include "Player/KoPlayerController.h"
#include "Player/Interaction/KoInteractionComponent.h"

struct FInputActionInstance;

AKoInteractionArea::AKoInteractionArea()
{
	PrimaryActorTick.bCanEverTick = false;

	//Setting up components.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));

	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SphereComponent->SetCollisionObjectType(ECC_Pawn);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("UDecalComponent"));

	DecalComponent->SetupAttachment(SphereComponent);
}

void AKoInteractionArea::OnEnterArea_Implementation(APawn* Pawn)
{
}

void AKoInteractionArea::OnLeaveArea_Implementation(APawn* Pawn)
{
}

void AKoInteractionArea::HandleArea(APawn* Pawn)
{
	OnHandleArea(Pawn, true);
}

void AKoInteractionArea::OnHandleArea_Implementation(APawn* Pawn, bool Success)
{
}

void AKoInteractionArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto* Pawn = Cast<APawn>(OtherActor);

	if (Pawn->FindComponentByClass<UKoInteractionComponent>())
	{
		auto* AreaComponent = Pawn->FindComponentByClass<UKoInteractionComponent>();

		//The overlapping actor has an interaction component to interact with the area.
		if (AreaComponent)
		{
			AreaComponent->AddInteractionArea(this);

			OnEnterArea(Pawn);
		}
	}
}

void AKoInteractionArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	auto* Pawn = Cast<APawn>(OtherActor);

	if (Pawn)
	{
		auto* AreaComponent = Pawn->FindComponentByClass<UKoInteractionComponent>();

		//The overlapping actor has an interaction component to interact with the area.
		if (AreaComponent)
		{
			AreaComponent->RemoveInteractionArea(this);

			OnLeaveArea(Pawn);
		}
	}
}
