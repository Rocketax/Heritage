// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interaction/Areas/KoFeatherCollectInteractionArea.h"

#include "Bird/KoBird.h"
#include "Player/KoPlayerCharacter.h"
#include "Player/KoPlayerCollector.h"
#include "Player/KoPlayerController.h"
#include "Player/Interaction/KoInteractionComponent.h"


void AKoFeatherCollectInteractionArea::OnFeatherCollected_Implementation(bool Succeed)
{
}


void AKoFeatherCollectInteractionArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//The player is in range to capture the bird.
	if (OtherActor->IsA(AKoPlayerCharacter::StaticClass()))
	{
		const APawn* Pawn = Cast<APawn>(OtherActor);
		
		Cast<AKoPlayerController>(Pawn->Controller)->CanCapture = true;
	}
}

void AKoFeatherCollectInteractionArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	//The player is no longer in range to capture the bird.
	if (OtherActor->IsA(AKoPlayerCharacter::StaticClass()))
	{
		const APawn* Pawn = Cast<APawn>(OtherActor);
		
		const auto* AreaComponent = Cast<UKoInteractionComponent>(Pawn->GetComponentByClass(UKoInteractionComponent::StaticClass()));

		//The player isn't in range to capture any bird.
		if (AreaComponent->CurrentInteractionAreas.Num() == 0)
		{
			Cast<AKoPlayerController>(Pawn->Controller)->CanCapture = false;
		}
	}
}

//This function is used to handle the interaction with the area. In this case, it is the feather collection.
void AKoFeatherCollectInteractionArea::HandleArea(APawn* Pawn)
{
	if (auto* Collector = Pawn->FindComponentByClass<UKoPlayerCollector>())
	{
		const auto& Succeed = Collector->TryCollectFeather();
		
		OnHandleArea(Pawn, Succeed);

		if (Succeed)
        {
            if (BirdReference)
            {
            	//Increase the feather count for the matching color of the bird.
            	int32* Value = Collector->Inventory.Find(BirdReference->FeatherColor);
            	if (Value)
            	{
            		(*Value)++; 
            	}
            	
            	auto* InteractionComponent = Cast<UKoInteractionComponent>(Cast<AKoPlayerCharacter>(Pawn)->GetComponentByClass(UKoInteractionComponent::StaticClass()));

            	InteractionComponent->RemoveInteractionArea(this);
            	
				//Handle bird capture.
            	BirdReference->OnCaptured();
            	
                BirdReference->Destroy();
            }
        }
		OnFeatherCollected(Succeed);
	}
}
