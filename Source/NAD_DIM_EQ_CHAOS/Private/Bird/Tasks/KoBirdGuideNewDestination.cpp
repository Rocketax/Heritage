// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdGuideNewDestination.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Bird/KoBird.h"

EBTNodeResult::Type UKoBirdGuideNewDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);

	FVector Destination = GenerateNextDestination(OwnerComp.GetAIOwner()->GetPawn(), BlackboardComponent);
	
	BlackboardComponent->SetValueAsVector(DestinationKeySelector.SelectedKeyName, Destination);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FVector UKoBirdGuideNewDestination::GenerateNextDestination(AActor* Actor, UBlackboardComponent* BlackboardComponent)
{
	const auto Bird = Cast<AKoBird>(Actor);

	FVector NextDestination = Bird->GuidePathDestinations[Bird->PathStep]->GetActorLocation();

	FHitResult HitResult;
 	
	//We raycast down to find the ground height. We then set the Z coordinate of the destination (height) to the ground height.
	GetWorld()->LineTraceSingleByChannel(HitResult,
										NextDestination,
										NextDestination + -Bird->GetActorUpVector() * 5000,
										ECC_Visibility);

	NextDestination.Z = HitResult.Location.Z;
	
	Bird->PathStep++;
	
	//The bird guide is at the end of its set path.
	if(Bird->PathStep >= Bird->GuidePathDestinations.Num())
	{
		BlackboardComponent->SetValueAsBool(FName("GuideIsFinalStep"), true);
	}
	
	return NextDestination;
}

void UKoBirdGuideNewDestination::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}