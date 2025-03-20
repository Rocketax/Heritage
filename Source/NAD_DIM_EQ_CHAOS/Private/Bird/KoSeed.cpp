// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoSeed.h"

AKoSeed::AKoSeed()
{
	PrimaryActorTick.bCanEverTick = true;
}

//This function starts a timer when the seed has been eaten. It destroys itself after a certain amount of time.
void AKoSeed::StartSelfDestructTimer()
{
	if (IsTimerStarted)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Initiating self destruct sequence!"))
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindUObject(this, &AKoSeed::SelfDestruct);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1, false, 5);

	IsTimerStarted = true;
}

void AKoSeed::SelfDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("BOOM! TEAM WIPED!"))
	Destroy();
}

