// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoBird.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoBirdController.h"

#include "Bird/KoBirdMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/KoGameState.h"

AKoBird::AKoBird(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	//Setting up components.
	MovementComponent = CreateDefaultSubobject<UKoBirdMovementComponent>(TEXT("UKoBirdMovementComponent"));

	AIControllerClass = AKoBirdController::StaticClass();

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UCapsuleComponent"));

	CapsuleComponent->SetupAttachment(RootComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("USkeletalMeshComponent"));

	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	SkeletalMeshComponent->bReceivesDecals = false;
}

void AKoBird::OnCaptured_Implementation()
{
}

void AKoBird::OnFlyingAway_Implementation()
{
}

//This function resets the ability to add a point in the bird's obstacle avoidance pathing.
void AKoBird::ResetAddPoint()
{
	//It's been 0.5 second, we can add another point.
	if (!CanAddPoint && PointTimeoutTimer > 0.5f)
	{
		CanAddPoint = true;
		PointTimeoutTimer = 0;
	}
}

void AKoBird::OnDetectionChanged_Implementation(float Detection)
{
}

void AKoBird::BeginPlay()
{
	Super::BeginPlay();

	const auto* World = GetWorld();

	check(World);

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this);

	//Raycasting to find out bird half height. This height is used to determine the height of the landing point
	//to ensure the bird stays connected to the ground.
	World->LineTraceSingleByChannel(HitResult, this->GetActorLocation(), this->GetActorLocation() - FVector(0, 0, 100), ECC_Visibility, CollisionParams);

	BirdHalfHeight = this->GetActorLocation().Z - HitResult.ImpactPoint.Z;

	auto& TimerManager = World->GetTimerManager();

	FTimerDelegate Delegate;

	Delegate.BindUObject(this, &AKoBird::ResetAddPoint);

	TimerManager.SetTimer(TimerHandle, Delegate, 0.1, true);
}
