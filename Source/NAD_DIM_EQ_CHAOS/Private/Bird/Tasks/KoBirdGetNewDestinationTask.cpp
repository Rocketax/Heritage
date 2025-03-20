// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdGetNewDestinationTask.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Bird/KoBird.h"

EBTNodeResult::Type UKoBirdGetNewDestinationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);

	const auto* Player = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("Player")));
	
	check(Player)

	const float StimuliStrength = BlackboardComponent->GetValueAsFloat(FName("StimuliStrength"));
	
	//We find a destination according to the stimuli strength. The higher it is, the further the bird flees.
	FVector Destination = GenerateRandomFleeingPoint(OwnerComp.GetAIOwner()->GetPawn(), Player->GetActorLocation(), StimuliStrength);
	
	BlackboardComponent->SetValueAsVector(DestinationKeySelector.SelectedKeyName, Destination);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

//This function finds a fleeing point at a distance proportional to the stimuli strength.
FVector UKoBirdGetNewDestinationTask::GenerateRandomFleeingPoint(AActor* Actor, FVector StimuliLocation, float StimuliStrength)
{
	const auto Bird = Cast<AKoBird>(Actor);
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	FNavLocation FleeingLocation;

	bool IsLocationValid, IsDoneLooking;
	
	int IterationCounter = 0;

	int Side = 1;

	do
	{
		const float FleeingDistance = StimuliStrength * Bird->MaxFleeingDistance;

		const int MinAngle = (IterationCounter * Bird->FleeingDetectionArc) * Side;

		const int MaxAngle = (MinAngle + Bird->FleeingDetectionArc) * Side;

		IsDoneLooking = MaxAngle >= 180;

		//We look for a random direction to flee. The fleeing cone is initially facing the opposing direction of the stimuli location
		//and the angle extends every iteration until it becomes a full circle around the bird. This makes it so the bird prioritizes
		//going away from the player and, if it is trapped in a corner for instance, it will still be able to escape.
		const float RandomAngle = FMath::FRandRange(MinAngle, MaxAngle);

		FVector FleeingDirection = (StimuliLocation - Bird->GetActorLocation()).RotateAngleAxis(180 + RandomAngle, FVector(0, 0, 1));

		FleeingDirection = FVector(FleeingDirection.X, FleeingDirection.Y, 0).GetSafeNormal();

		FVector FleeingDestination = Bird->GetActorLocation() + FleeingDirection * FleeingDistance;
		
		bool IsLocationOnNav = NavSystem->ProjectPointToNavigation(FleeingDestination, FleeingLocation);

		const UNavigationPath* PathToDestination = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(),
		Bird->GetActorLocation(), FleeingLocation.Location);

		//The location is valid if it is able to find a path to the destination and the destination is on the navigation mesh.
		IsLocationValid = PathToDestination != nullptr && PathToDestination->IsValid() && !PathToDestination->IsPartial() && IsLocationOnNav;

		//Side is used so we check both sides of the cone. If we draw a line in front of the bird, the cone is placed so the line
		//splits it in half. Therefore, side = 1 is one half and side = -1 is the other half. When we reach 180 degrees,
		//both halves meet and we are done looking.
		if (Side > 0)
		{
			Side *= -1;
		}
		else
		{
			Side *= -1;
			IterationCounter++;
		}
	}
	//We keep looking until we find a valid fleeing point or have checked all around the bird.
	while (!IsLocationValid && !IsDoneLooking);
	
	
	
	return IsLocationValid ? FleeingLocation.Location : Bird->GetActorLocation();
}
	
void UKoBirdGetNewDestinationTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}


