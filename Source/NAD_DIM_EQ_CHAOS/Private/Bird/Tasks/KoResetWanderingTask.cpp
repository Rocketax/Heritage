// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoResetWanderingTask.h"

#include "AIController.h"
#include "Bird/KoBird.h"

EBTNodeResult::Type UKoBirdResetWanderingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKoBird* Bird = Cast<AKoBird>(OwnerComp.GetAIOwner()->GetPawn());

	Bird->IsWandering = false;
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UKoBirdResetWanderingTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
