// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdSetEatingTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoBird.h"
#include "Bird/KoSeed.h"

EBTNodeResult::Type UKoBirdSetEatingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKoBird* Bird = Cast<AKoBird>(OwnerComp.GetAIOwner()->GetPawn());

	Bird->IsEating = true;

	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	AKoSeed* Seed = Cast<AKoSeed>(Blackboard->GetValueAsObject(FName("Seed")));

	//We set the seed to be destroyed a certain time after it's been eaten.
	if (Seed) Seed->StartSelfDestructTimer();
	
	return EBTNodeResult::Succeeded;
}

void UKoBirdSetEatingTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
