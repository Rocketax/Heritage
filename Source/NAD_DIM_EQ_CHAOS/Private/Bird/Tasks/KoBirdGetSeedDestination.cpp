// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdGetSeedDestination.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoSeed.h"

EBTNodeResult::Type UKoBirdGetSeedDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent)
	
	const auto* Seed = Cast<AKoSeed>(BlackboardComponent->GetValueAsObject(SeedKeySelector.SelectedKeyName));

	FNavLocation NavLocation;

	const auto* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	//We find a random point accessible within a certain radius of the seed.
	NavSystem->GetRandomPointInNavigableRadius(Seed->GetActorLocation(), 50, NavLocation);

	const auto Location = NavLocation.Location;

	BlackboardComponent->SetValueAsVector(FName("SeedDestination"), Location);

	//If the bird is far enough from the seed, it should fly to get to it.
	if (OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Z - Location.Z > 50)
	{
		BlackboardComponent->SetValueAsBool(FName("ShouldFly"), true);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UKoBirdGetSeedDestination::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
