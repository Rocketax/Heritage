// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdResetEatingTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoBird.h"

EBTNodeResult::Type UKoBirdResetEatingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKoBird* Bird = Cast<AKoBird>(OwnerComp.GetAIOwner()->GetPawn());

	check(Bird)

	//Once the bird is done eating, we reset it to its normal state.
	Bird->IsEating = false;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	BlackboardComponent->ClearValue(DestinationKeySelector.SelectedKeyName);

	BlackboardComponent->SetValueAsVector(FName("SeedDestination"), FAISystem::InvalidLocation);

	BlackboardComponent->SetValueAsBool(FName("ShouldFly"), false);
	
	return EBTNodeResult::Succeeded;
}

void UKoBirdResetEatingTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
